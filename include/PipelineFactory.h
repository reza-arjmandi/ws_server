#pragma once

#include <memory>
#include <functional>

#include <boost/asio.hpp>

#include "IPipelineFactory.h"
#include "Pipeline.h"
#include "PipelineDependencies.h"
#include "Types.h"

using namespace std;

class PipelineFactory : public IPipelineFactory 
{

public:

    PipelineFactory(
        tcp_endpoint endpoint, 
        shared_ptr<IWork> session_executor,
        shared_ptr<IWork> authenticator)
    {
        _deps = make_shared<PipelineDependencies>(
            endpoint, 
            session_executor,
            authenticator);
    }

	shared_ptr<IPipeline> create(function<void()> push_buffer) final
    {
        return make_shared<Pipeline>(_deps, push_buffer);
    }

private:

    shared_ptr<PipelineDependencies> _deps;

};
