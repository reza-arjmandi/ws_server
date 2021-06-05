#pragma once

#include <memory>

#include "IStage.h"
#include "IWork.h"

using namespace std;

class IStageFactory
{

public:

    virtual ~IStageFactory() = default;

    virtual shared_ptr<IStage> create(shared_ptr<IWork> work) = 0;

};