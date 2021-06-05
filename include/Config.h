#pragma once

#include <boost/asio.hpp>

#include "IConfig.h"
#include "BufferFactory.h"
#include "PipelineFactory.h"
#include "StageFactory.h"

class Config : public IConfig
{

public:

    using tcp_endpoint = boost::asio::ip::tcp::endpoint;

    Config(
        tcp_endpoint endpoint, 
        shared_ptr<IWork> session_executor,
        shared_ptr<IWork> authenticator)
    : _pipeline_factory{ endpoint, session_executor, authenticator}
    {
    }

    void set_endpoint(tcp_endpoint end_point) final
    {
        _end_point = end_point;
    }

    tcp_endpoint get_endpoint() const final
    {
        return _end_point;
    }

    IBufferFactory& get_buffer_factory() final
    {
        return _buffer_factory;
    }

    IPipelineFactory& get_pipeline_factory() final
    {
        return _pipeline_factory;
    }

private:

    tcp_endpoint _end_point;
    BufferFactory _buffer_factory;
    PipelineFactory _pipeline_factory;
    StageFactory _stage_factory;

};