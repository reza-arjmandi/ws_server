#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IPipeline.h"

class MockPipeline : public IPipeline
{

public:

    MOCK_METHOD(shared_ptr<IStage>, front, ());
    MOCK_METHOD(void, stop, ());

};