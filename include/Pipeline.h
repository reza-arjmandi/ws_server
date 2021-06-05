#pragma once

#include "IPipeline.h"
#include "IPipelineDependencies.h"

using namespace std;

class Pipeline : public IPipeline 
{

public:

		Pipeline(shared_ptr<IPipelineDependencies> config)
		{
			auto& stage_facctory{ config->get_stage_factory() };
			_create_session_stage = 
				stage_facctory.create(config->get_session_factory());
			auto authenticator{ config->get_authenticator() };
			authenticator->next([&](auto buff){
				if(buff->is_authenticated())
				{
					_create_session_stage->process(buff);
				}
			});
			_auth_stage = stage_facctory.create(authenticator);
			auto acceptor{ config->get_acceptor() };
			acceptor->next([&](auto buff){
				_auth_stage->process(buff);
			});
			_accept_stage = stage_facctory.create(acceptor);
		}

		~Pipeline()
		{
			stop();
		}

		shared_ptr<IStage> front() final
        {
			return _accept_stage;
        }

		void stop() final
        {
			if(_is_stopped)
			{
				return;
			}
			_accept_stage->stop();
			_auth_stage->stop();
			_create_session_stage->stop();
			_is_stopped = true;
        }

    private:

		bool _is_stopped{ false };
		shared_ptr<IStage> _create_session_stage;
		shared_ptr<IStage> _auth_stage;
		shared_ptr<IStage> _accept_stage;

	};