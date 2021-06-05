#pragma once

#include <memory>

#include "IBufferView.h"

#include <vector>

using namespace std;

class IBuffer
{
    
public:

    using io_context = boost::asio::io_context; 

    virtual ~IBuffer() = default;

    virtual shared_ptr<IBufferView> next_view() = 0;
    virtual vector<io_context>& get_ioc_list() = 0;

};