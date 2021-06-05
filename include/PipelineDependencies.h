#pragma once

#include <boost/asio.hpp>

#include "IPipelineDependencies.h"
#include "StageFactory.h"
#include "Acceptor.h"
#include "SessionExecutor.h"
#include "NoAuth.h"

class PipelineDependencies : public IPipelineDependencies
{
    
    using tcp_endpoint = boost::asio::ip::tcp::endpoint;

public:

    PipelineDependencies(tcp_endpoint endpoint)
    {
        _acceptor = make_shared<Acceptor>(endpoint);
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
    shared_ptr<Acceptor> _acceptor;
    shared_ptr<SessionExecutor> _session_executor{
        make_shared<SessionExecutor>()};
    shared_ptr<NoAuth> _auth{ make_shared<NoAuth>() };

};