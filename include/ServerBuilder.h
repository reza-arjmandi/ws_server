#pragma once

#include <string>

#include "Config.h"
#include "WSServer.h"

using namespace std;

class ServerBuilder
{

public:

    ServerBuilder& address(string address)
    {
        _address = address;
        return *this;
    }

    ServerBuilder& port(int port)
    {
        _port = port;
        return *this;
    }

    shared_ptr<WSServer> build()
    {
        auto address{boost::asio::ip::make_address(_address)};
        auto config{ make_shared<Config>(Config::tcp_endpoint(address, _port)) };
        return make_shared<WSServer>(config);
    }

private:

    string _address{ "127.0.0.1" };
    int _port{ 1111 };
};
