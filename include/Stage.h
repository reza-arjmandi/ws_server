#pragma once

#include <memory>

#include "Active.h"
#include "IWork.h"
#include "IStage.h"

using namespace std;

class Stage : public IStage
{

public:

	Stage(shared_ptr<IWork> work, size_t num_threads = 1)
		: _work{ work },
		_active{ num_threads }
	{
	}

	void process(shared_ptr<IBufferView> buffer) final 
	{
		_active.send([this, buffer]() mutable {
				_work->exec(buffer);
		});
	}

	void stop() final 
	{
		_active.stop();
	}

private:

	shared_ptr<IWork> _work;
	Active _active;

};
