#pragma once

#include <boost/asio.hpp>

#include "IPipelineDependencies.h"
#include "StageFactory.h"
#include "TCPAcceptor.h"
#include "NoAuth.h"

class PipelineDependencies : public IPipelineDependencies
{
    
public:

    PipelineDependencies(
        tcp_endpoint endpoint, 
        shared_ptr<IWork> session_executor,
        shared_ptr<IWork> authenticator)
        :_session_executor{ session_executor },
        _auth{ authenticator }
    {
        _acceptor = make_shared<TCPAcceptor>(endpoint);
    }

    shared_ptr<IWork> get_session_factory() const final
    {
        return _session_executor;
    }

    shared_ptr<IWork> get_authenticator() const final
    {
        return _auth;
    }

    shared_ptr<IWork> get_acceptor() const final
    {
        return _acceptor;
    }

    IStageFactory& get_stage_factory() final
    {
        return _stage_factory;
    }

private:

    StageFactory _stage_factory;
    shared_ptr<TCPAcceptor> _acceptor;
    shared_ptr<IWork> _session_executor;
    shared_ptr<IWork> _auth;

};