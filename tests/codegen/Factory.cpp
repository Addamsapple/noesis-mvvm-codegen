#include "Factory.h"

#include "model/League.h"
#include "model/Team.h"
#include "model/Player.h"
#include "model/Goalkeeper.h"
#include "model/Outfielder.h"
#include "model/Midfielder.h"

#include "viewmodel/League.h"
#include "viewmodel/Team.h"
#include "viewmodel/Player.h"
#include "viewmodel/Goalkeeper.h"
#include "viewmodel/Outfielder.h"
#include "viewmodel/Midfielder.h"

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
