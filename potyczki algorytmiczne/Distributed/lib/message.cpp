#include "message.h"
#include "node.h"
#include "runner.h"

extern "C"
{
    int NumberOfNodes()
    {
        return Runner::getInstance().nodes.size();
    }

    int MyNodeId()
    {
        Node &node = Runner::getInstance().getNode();
        return node.id;
    }

    void PutChar(int target, char value)
    {
        Node &node = Runner::getInstance().getNode();
        node.getOutputBuffer(target)->put(value);
    }

    void PutInt(int target, int value)
    {
        Node &node = Runner::getInstance().getNode();
        node.getOutputBuffer(target)->put(value);
    }

    void PutLL(int target, long long value)
    {
        Node &node = Runner::getInstance().getNode();
        node.getOutputBuffer(target)->put(value);
    }

    void Send(int target)
    {
        Node &node = Runner::getInstance().getNode();
        node.send(target);
    }

    int Receive(int source)
    {
        Node &node = Runner::getInstance().getNode();
        if(source == -1)
            return node.recv();
        else
        {
            node.recv(source);   
            return source;
        }
    }


    char GetChar(int source)
    {
        Node &node = Runner::getInstance().getNode();
        return node.getInputBuffer(source)->get<char>();
    }

    int GetInt(int source)
    {
        Node &node = Runner::getInstance().getNode();
        return node.getInputBuffer(source)->get<int>();
    }

    long long GetLL(int source)
    {
        Node &node = Runner::getInstance().getNode();
        return node.getInputBuffer(source)->get<long long>();
    }

}