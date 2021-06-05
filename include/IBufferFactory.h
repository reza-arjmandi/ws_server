#pragma once

#include <memory>

#include "IBuffer.h"

using namespace std;

class IBufferFactory
{

public:

    virtual ~IBufferFactory() = default;

    virtual shared_ptr<IBuffer> create() = 0;

};