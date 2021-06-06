#pragma once

#include <boost/asio.hpp>

#include "IBufferFactory.h"
#include "IPipelineFactory.h"
#include "Types.h"

class ITCPServerDependencies
{
    
public:

    virtual ~ITCPServerDependencies() = default;

    virtual void set_endpoint(tcp_endpoint) = 0;
    virtual tcp_endpoint get_endpoint() const = 0;
    virtual IBufferFactory& get_buffer_factory() = 0;
    virtual IPipelineFactory& get_pipeline_factory() = 0;

};