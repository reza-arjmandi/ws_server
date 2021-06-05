#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IPipelineFactory.h"

class MockPipelineFactory : public IPipelineFactory
{
    
public:

    MOCK_METHOD(shared_ptr<IPipeline>, create, (function<void()> push_buffer));

};