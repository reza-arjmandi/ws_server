#pragma once

#include <boost/asio.hpp>

#include "Session.h"

class IBufferView
{
    
public:

    using tcp_socket = boost::asio::ip::tcp::socket; 

    virtual ~IBufferView() = default;
    
    virtual bool is_authenticated() const = 0;
    virtual tcp_socket& get_socket() = 0;
    virtual Session& get_session() = 0;

};
