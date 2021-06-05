#pragma once

#include "IBuffer.h"

class MockBuffer : public IBuffer
{

public:

    MOCK_METHOD(shared_ptr<IBufferView>, next_view, ());

};