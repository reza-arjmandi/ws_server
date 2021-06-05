#pragma once

#include <memory>
#include <functional>

#include <boost/asio.hpp>

#include "IPipelineFactory.h"
#include "Pipeline.h"
#include "PipelineDependencies.h"

using namespace std;

class PipelineFactory : public IPipelineFactory 
{

    using tcp_endpoint = boost::asio::ip::tcp::endpoint;

public:

    PipelineFactory(tcp_endpoint endpoint)
    {
        _deps = make_shared<PipelineDependencies>(endpoint);
    }

	shared_ptr<IPipeline> create(function<void()> push_buffer) final
    {
        return make_shared<Pipeline>(_deps);
    }

private:

    shared_ptr<PipelineDependencies> _deps;

};
