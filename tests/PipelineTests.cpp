#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Pipeline.h"

#include "tests/MockPipelineDependencies.h"
#include "tests/MockStageFactory.h"
#include "tests/MockWork.h"
#include "tests/MockStage.h"
#include "tests/MockBufferView.h"

using namespace std;
using namespace testing;

class PipelineTests : public Test
{

public:

    void SetUp() final
    {
        EXPECT_CALL(*_deps, get_session_factory()).WillOnce(
            Return(_session_factory));

        EXPECT_CALL(*_deps, get_stage_factory()).WillOnce(
            ReturnRef(_stage_factory));

        EXPECT_CALL(_stage_factory, create(Eq(_session_factory))).WillOnce(
            Return(_create_session_stage));

        EXPECT_CALL(*_deps, get_authenticator()).WillOnce(
            Return(_authenticator));
        EXPECT_CALL(*_authenticator, next(_)).WillOnce(SaveArg<0>(&_next_auth));

        EXPECT_CALL(_stage_factory, create(Eq(_authenticator))).WillOnce(
            Return(_auth_stage));

        EXPECT_CALL(*_deps, get_acceptor()).WillOnce(
            Return(_acceptor));

        EXPECT_CALL(*_acceptor, next(_)).WillOnce(SaveArg<0>(&_next_accept));

        EXPECT_CALL(_stage_factory, create(Eq(_acceptor))).WillOnce(
            Return(_accept_stage));

        _pipeline = make_unique<Pipeline>(_deps, _push_buffer.AsStdFunction());
    }
    
    void TearDown() final
    {
        if(_is_stopped)
        {
            return;
        }

        EXPECT_CALL(*_accept_stage, stop()).Times(1);
        EXPECT_CALL(*_auth_stage, stop()).Times(1);
        EXPECT_CALL(*_create_session_stage, stop()).Times(1);
    }

    MockStageFactory _stage_factory;
    bool _is_stopped{ false };

    MockFunction<void()> _push_buffer;
    shared_ptr<MockPipelineDependencies> _deps{ 
        make_shared<MockPipelineDependencies>() };
    shared_ptr<MockWork> _session_factory{ 
        make_shared<MockWork>() };
    shared_ptr<MockWork> _authenticator{ 
        make_shared<MockWork>() };
    shared_ptr<MockWork> _acceptor{ 
        make_shared<MockWork>() };
    shared_ptr<MockStage> _create_session_stage{ 
        make_shared<MockStage>() };
    shared_ptr<MockStage> _auth_stage{ 
        make_shared<MockStage>() };
    shared_ptr<MockStage> _accept_stage{ 
        make_shared<MockStage>() };
    shared_ptr<MockBufferView> _buffer_view{ 
        make_shared<MockBufferView>() };
    function<void(shared_ptr<IBufferView>)> _next_accept;
    function<void(shared_ptr<IBufferView>)> _next_auth;

    unique_ptr<Pipeline> _pipeline;

};

TEST_F(PipelineTests, ctor)
{

}

TEST_F(PipelineTests, test_front)
{
    ASSERT_THAT(_pipeline->front(), Eq(_accept_stage));
}

TEST_F(PipelineTests, test_stop)
{
    EXPECT_CALL(*_accept_stage, stop()).Times(1);
    EXPECT_CALL(*_auth_stage, stop()).Times(1);
    EXPECT_CALL(*_create_session_stage, stop()).Times(1);
    _pipeline->stop();
    _is_stopped = true;
}

TEST_F(PipelineTests, test_stop_when_is_called_more_than_once)
{
    EXPECT_CALL(*_accept_stage, stop()).Times(1);
    EXPECT_CALL(*_auth_stage, stop()).Times(1);
    EXPECT_CALL(*_create_session_stage, stop()).Times(1);
    _pipeline->stop();
    _is_stopped = true;
    _pipeline->stop();
}

TEST_F(PipelineTests, test_next_of_accpet)
{
    EXPECT_CALL(*_auth_stage, process(Eq(_buffer_view))).Times(1);
    EXPECT_CALL(_push_buffer, Call()).Times(1);

    _next_accept(_buffer_view);
}

TEST_F(PipelineTests, test_next_of_authenticated)
{
    EXPECT_CALL(*_buffer_view, is_authenticated()).WillOnce(Return(true));
    EXPECT_CALL(*_create_session_stage, process(Eq(_buffer_view))).Times(1);

    _next_auth(_buffer_view);
}

TEST_F(PipelineTests, test_next_of_not_authenticated)
{
    EXPECT_CALL(*_buffer_view, is_authenticated()).WillOnce(Return(false));

    _next_auth(_buffer_view);
}

