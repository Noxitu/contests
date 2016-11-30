#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <functional>
#include <chrono>
#include "buffer.h"

class Runner;

struct NodeStatistics
{
    size_t messages_sent = 0;
    size_t messages_received = 0;
    size_t bytes_sent = 0;
    size_t bytes_received = 0;
    std::chrono::duration<double> time = std::chrono::milliseconds{0};
};

class Node
{
    Runner &runner;
    std::mutex mutex;
    std::vector<Buffer_sp> out_buffers, in_buffers;
    std::vector<std::list<Buffer_sp>> in_queue;

    std::chrono::high_resolution_clock::time_point running_since;
    void reset_timecounter() { running_since = std::chrono::high_resolution_clock::now(); }
    void apply_timecounter() { stats.time += std::chrono::high_resolution_clock::now() - running_since; }

    int receiving_from;
public:
    NodeStatistics stats;

    std::thread thread;

    Node(Runner &runner, std::function<void()> func, int nodes);

    enum Status
    {
        Running, IO, Idle, Finished
    };

    int id;
    Status status = Status::Idle;

    Buffer_sp& getOutputBuffer(int node);
    Buffer_sp& getInputBuffer(int node);
    void send(int target);
    int recv(int source = -1);

    bool can_read();
    bool can_awake();

    void main(std::function<void()> func);
    void awake();
    void suspend();
    void end();
};

typedef std::unique_ptr<Node> Node_sp;