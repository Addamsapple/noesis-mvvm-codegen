include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(doctest)

find_library(libNoesis Noesis REQUIRED)

add_subdirectory(core)
add_subdirectory(reflection)
add_subdirectory(codegen)
