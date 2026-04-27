#include "Factory.h"

#include "generated/model/League.h"
#include "generated/model/Team.h"
#include "generated/model/Player.h"
#include "generated/model/Goalkeeper.h"
#include "generated/model/Outfielder.h"
#include "generated/model/Midfielder.h"

#include "generated/viewmodel/League.h"
#include "generated/viewmodel/Team.h"
#include "generated/viewmodel/Player.h"
#include "generated/viewmodel/Goalkeeper.h"
#include "generated/viewmodel/Outfielder.h"
#include "generated/viewmodel/Midfielder.h"

namespace {

mvvm::ViewModelFactory MakeTestFactory() {
    mvvm::ViewModelFactory::Builder builder;
    builder.Register<viewmodel::League>();
    builder.Register<viewmodel::Team>();
    builder.Register<viewmodel::Player>();
    builder.Register<viewmodel::Goalkeeper>();
    builder.Register<viewmodel::Outfielder>();
    builder.Register<viewmodel::Midfielder>();
    return std::move(builder).Build();
}

}

const mvvm::ViewModelFactory TEST_FACTORY = MakeTestFactory();
