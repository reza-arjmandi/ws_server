#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IStage.h"

class MockStage : public IStage
{

public:

    MOCK_METHOD(void, process, (shared_ptr<IBufferView> buffer));
    MOCK_METHOD(void, stop, ());

};