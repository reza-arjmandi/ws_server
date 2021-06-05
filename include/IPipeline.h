#pragma once

#include "IStage.h"

class IPipeline 
{

public:

		virtual ~IPipeline() = default;

		virtual shared_ptr<IStage> front() = 0;
		virtual void stop() = 0;

};
