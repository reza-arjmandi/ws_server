#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IPipelineDependencies.h"

class MockPipelineDependencies : public IPipelineDependencies
{

public:

    MOCK_METHOD(shared_ptr<IWork>, get_session_factory, (), (const));
    MOCK_METHOD(shared_ptr<IWork>, get_authenticator, (), (const));
    MOCK_METHOD(shared_ptr<IWork>, get_acceptor, (), (const));
    MOCK_METHOD(IStageFactory&, get_stage_factory, ());

};