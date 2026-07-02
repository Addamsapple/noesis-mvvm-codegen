#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <NsCore/Init.h>
#include <NsCore/TypeOf.h>

#include "viewmodel/League.h"
#include "viewmodel/Team.h"
#include "viewmodel/Player.h"
#include "viewmodel/Goalkeeper.h"
#include "viewmodel/Outfielder.h"
#include "viewmodel/Midfielder.h"

int main(int argc, char ** argv) {
    Noesis::Init();

    Noesis::TypeOf<viewmodel::League>();
    Noesis::TypeOf<viewmodel::Team>();
    Noesis::TypeOf<viewmodel::Player>();
    Noesis::TypeOf<viewmodel::Goalkeeper>();
    Noesis::TypeOf<viewmodel::Outfielder>();
    Noesis::TypeOf<viewmodel::Midfielder>();

    doctest::Context context;
    context.applyCommandLine(argc, argv);
    auto result = context.run();

    Noesis::Shutdown();

    return result;
}
