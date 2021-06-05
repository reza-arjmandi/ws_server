#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IStageFactory.h"

class MockStageFactory : public IStageFactory
{

public:

    MOCK_METHOD(shared_ptr<IStage>, create, (shared_ptr<IWork> work));

};