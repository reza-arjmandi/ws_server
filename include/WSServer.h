#pragma once

#include <memory>

#include "IConfig.h"

using namespace std;

class WSServer
{

public:

    WSServer(shared_ptr<IConfig> config)
    {
        _buffer = config->get_buffer_factory().create();
        _pipeline = config->get_pipeline_factory().create(
            [&](){push_buffer();});
    }

    ~WSServer()
    {
        stop();
    }

    void run()
    {
        if(_is_run)
        {
            return;
        }
        _is_run = true;
        push_buffer();
    }

    void stop()
    {
        if(!_is_run)
        {
            return;
        }
        _pipeline->stop();
        _is_run = false;
    }

    void push_buffer()
    {
        if(!_is_run)
        {
            return;
        }
        _pipeline->front()->process(
            _buffer->next_view());
    }

private:

    bool _is_run{false};
    shared_ptr<IBuffer> _buffer;
    shared_ptr<IPipeline> _pipeline;

};