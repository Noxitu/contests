#include "runner.h"
#include <iostream>

Runner *Runner::instance;

Runner::Runner(int node_count, int worker_count, std::function<void()> func) : workers(worker_count)
{
    if(instance)
        throw std::logic_error("Second Runner created");

    instance = this;

    for(int i = 0; i < node_count; ++i)
    {
        Node_sp node(new Node(*this, func, node_count));
        node->id = i;
        thread2id[node->thread.get_id()] = node.get();
        nodes.emplace_back(std::move(node));
    }
}

Runner::~Runner()
{
    for(auto &node : nodes)
        node->thread.join();

    instance = nullptr;
}

#include <iostream>

void Runner::run()
{
    int still_running;
    do
    {
        Lock lock(mutex);
        if(workers == 0)
        {
            worker_freed.wait(lock);
        }
        //std::cerr << "runner loop tick" << std::endl;

        still_running = 0;

        for(auto &node : nodes)
        {
            if(node->status != Node::Finished)
                ++still_running;
        }

        bool started = false;
        for(auto &node : nodes)
        {
            if(node->can_awake())
            {
                started = true;
                --workers;
                node->awake();
                break;
            }
        }

        if(still_running and not started)
            io.wait(lock);
    } while(still_running > 0);
}

void Runner::free_worker()
{
    Lock lock(mutex);
    ++workers;
    worker_freed.notify_one();
}

void Runner::io_update()
{
    Lock lock(mutex);
    io.notify_one();
}

Node& Runner::getNode() const
{
    return *thread2id.at(std::this_thread::get_id());
}

Runner& Runner::getInstance()
{
    if(not instance)
        throw std::logic_error("Runner doesn't exist");

    return *instance;
}

void Runner::print_stats(std::ostream &out)
{
    for(auto &node : nodes)
    {
        out << "Node #" << node->id << '\n';
        out << "\ttime     = " << std::chrono::duration_cast<std::chrono::milliseconds>(node->stats.time).count() << "ms\n";
        out << "\tsent     = " << node->stats.messages_sent << " \t(" << node->stats.bytes_sent << " bytes)\n";
        out << "\treceived = " << node->stats.messages_received << " \t(" << node->stats.bytes_received << " bytes)\n";
    }
    out << std::flush;
}