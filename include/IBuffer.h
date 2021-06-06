#pragma once

#include <memory>
#include <vector>

#include "IBufferView.h"
#include "Types.h"

using namespace std;

class IBuffer
{
    
public:

    virtual ~IBuffer() = default;

    virtual shared_ptr<IBufferView> next_view() = 0;
    virtual vector<io_context>& get_ioc_list() = 0;

};