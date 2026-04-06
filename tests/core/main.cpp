#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <NsCore/Init.h>

int main(int argc, char ** argv) {
    Noesis::Init();

    doctest::Context context;
    context.applyCommandLine(argc, argv);
    auto result = context.run();

    Noesis::Shutdown();

    return result;
}

