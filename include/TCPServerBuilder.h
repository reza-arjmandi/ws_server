#pragma once

#include <string>

#include "TCPServerDependencies.h"
#include "TCPServer.h"
#include "NoAuth.h"
#include "Types.h"

using namespace std;

class TCPServerBuilder
{

public:

    TCPServerBuilder& address(string address)
    {
        _address = address;
        return *this;
    }

    TCPServerBuilder& port(int port)
    {
        _port = port;
        return *this;
    }

    TCPServerBuilder& session_executor(shared_ptr<IWork> executor)
    {
        _session_executor = executor;
        return *this;
    }

    TCPServerBuilder& authenticator(shared_ptr<IWork> auth)
    {   
        _authenticator = auth;
        return *this;
    }

    shared_ptr<TCPServer> build()
    {
        auto address{ net::ip::make_address(_address) };
        auto config{ make_shared<TCPServerDependencies>(
            tcp_endpoint(address, _port), 
            _session_executor, 
            _authenticator) };
        return make_shared<TCPServer>(config);
    }

private:

    string _address{ "127.0.0.1" };
    int _port{ 1111 };
    shared_ptr<IWork> _session_executor;
    shared_ptr<IWork> _authenticator{ 
        make_shared<NoAuth>() };

};
