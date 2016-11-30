#pragma once
#include "node.h"
#include <condition_variable>

typedef std::unique_lock<std::mutex> Lock;

class Runner
{
    std::unordered_map<std::thread::id, Node*> thread2id;
    int workers;
    std::mutex mutex;
    std::condition_variable worker_freed;
    std::condition_variable io;

    static Runner *instance;
public:
    std::vector<Node_sp> nodes;
    Lock lock() { return Lock(mutex); }

    Runner(int node_count, int worker_count, std::function<void()>);
    ~Runner();

    void run();
    void free_worker();
    void io_update();
    void print_stats(std::ostream &out);

    Node& getNode() const;
    static Runner& getInstance();
};