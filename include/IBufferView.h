#pragma once

#include <boost/asio.hpp>

class IBufferView
{
    
public:

    using tcp_socket = boost::asio::ip::tcp::socket; 
    using io_context = boost::asio::io_context; 

    virtual ~IBufferView() = default;
    
    virtual bool is_authenticated() const = 0;
    virtual tcp_socket get_socket() = 0;
    virtual tcp_socket& get_socket_ref() = 0;

};
