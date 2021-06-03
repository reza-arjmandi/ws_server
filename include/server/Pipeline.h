#pragma once

#include "async/IPipeline.h"

namespace Server 
{

    using namespace std;
	using namespace Async;

	template<class StageType>
	class Pipeline : public IPipeline<StageType> {

	public:

		template<>
		Pipeline()
		: _create_session_stage{
			[](auto buffer){

			}
		}
		_accept_stage{
			[](auto buffer){

			}
		}
		{
		}

		StageType& front() final
        {
        }

		void stop() final
        {
        }

    private:

		StageType _create_session_stage;
		StageType _auth_stage;
		StageType _accept_stage;

	};

}