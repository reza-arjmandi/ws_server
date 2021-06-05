#pragma once

#include <memory>

#include "IBufferView.h"

using namespace std;

class IBuffer
{
    
public:

    virtual ~IBuffer() = default;

    virtual shared_ptr<IBufferView> next_view() = 0;

};