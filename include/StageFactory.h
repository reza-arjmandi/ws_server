#pragma once

#include <memory>

#include "IStageFactory.h"
#include "Stage.h"

using namespace std;

class StageFactory : public IStageFactory
{

public:

    shared_ptr<IStage> create(shared_ptr<IWork> work)
    {
        return make_shared<Stage>(work);
    }

};