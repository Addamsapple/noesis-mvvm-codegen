#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <NsCore/Init.h>
#include <NsCore/TypeOf.h>

#include "generated/viewmodel/League.h"
#include "generated/viewmodel/Team.h"
#include "generated/viewmodel/Player.h"
#include "generated/viewmodel/Goalkeeper.h"
#include "generated/viewmodel/Outfielder.h"
#include "generated/viewmodel/Midfielder.h"

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
