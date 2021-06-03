#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "server/Pipeline.h"

#include "async/Stage.h"

using namespace std;
using namespace testing;
using namespace Async;
using namespace Server;

class MockBufferView
{
};

TEST(ServerPipelineTests, test_1)
{
	using BufferViewType = shared_ptr<MockBufferView>;
	using WorkType = function<void(BufferViewType)>;
	using StageType = Stage<WorkType>;

    Pipeline<StageType> pipeline;

}