#pragma once

#include <boost/asio.hpp>

#include "Types.h"

class IBufferView
{
    
public:

    virtual ~IBufferView() = default;
    
    virtual bool is_authenticated() const = 0;
    virtual void set_authentication_result(bool result) = 0;
    virtual tcp_socket get_socket() = 0;
    virtual tcp_socket& get_socket_ref() = 0;

};
