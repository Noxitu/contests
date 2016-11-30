#include "node.h"
#include "runner.h"
#include <iostream>

Node::Node(Runner &runner, std::function<void()> func, int nodes) : runner(runner), out_buffers(nodes), in_buffers(nodes), in_queue(nodes)
{
    mutex.lock();
    thread = std::thread(&main, this, func);
}

void Node::main(std::function<void()> func)
{
    try
    {
        mutex.lock();
        reset_timecounter();
        func();
        apply_timecounter();
        end();
    }
    catch(std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        throw;
    }
    catch(...)
    {
        std::cerr << "Unknown exception" << std::endl;
        throw;
    }
}

Buffer_sp& Node::getOutputBuffer(int node)
{
    if(not out_buffers.at(node))
        out_buffers.at(node) = Buffer_sp(new Buffer());

    return out_buffers.at(node);
}

Buffer_sp& Node::getInputBuffer(int node)
{
    return in_buffers.at(node);
}

void Node::send(int target_id)
{
    apply_timecounter();
    Node &target = *runner.nodes.at(target_id);
    Buffer_sp buffer = std::move(getOutputBuffer(target_id));

    {
        Lock lock = runner.lock();
        stats.messages_sent++;
        stats.bytes_sent += buffer->size();
        buffer->timestamp = stats.time;
        target.in_queue.at(id).push_back(std::move(buffer));
    }

    runner.io_update();
    reset_timecounter();
}

int Node::recv(int source)
{
    apply_timecounter();
    receiving_from = source;
    bool block;
    {
        Lock lock = runner.lock();
        block = not can_read();
    }
    if(block)
    {
        suspend();
    }

    source = receiving_from;

    Lock lock = runner.lock();
    Buffer_sp buffer = std::move(in_queue.at(source).front());

    stats.messages_received++;
    stats.bytes_received += buffer->size();
    stats.time = max(stats.time, buffer->timestamp);

    in_queue.at(source).pop_front();
    in_buffers.at(source) = std::move(buffer);
    in_buffers.at(source)->reset();

    reset_timecounter();
    return source;
}

bool Node::can_read()
{
    if(receiving_from != -1)
    {
        if(not in_queue.at(receiving_from).empty())
            return true;
    }
    else
    {
        int ret = 0;
        for(auto &q : in_queue)
        {
            if(not q.empty())
            {
                receiving_from = ret;
                return true;
            }
            ++ret;
        }
    }

    return false;
}

bool Node::can_awake()
{
    if(status == Idle)
        return true;

    if(status == IO and can_read())
        return true;

    return false;
}

void Node::awake()
{
    //std::cout << "awekening " << id << std::endl;
    status = Status::Running;
    mutex.unlock();
}

void Node::suspend()
{
    //std::cout << "suspending " << id << std::endl;
    {
        auto lock = runner.lock();
        status = Status::IO;
    }
    runner.free_worker();
    mutex.lock();
}

void Node::end()
{
    //std::cout << "end " << id << std::endl;
    {
        auto lock = runner.lock();
        status = Status::Finished;
    }
    runner.free_worker();
    runner.io_update();
}