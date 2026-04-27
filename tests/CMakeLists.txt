include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(doctest)

find_library(libNoesis Noesis REQUIRED)

add_library(core STATIC ${MVVM_SOURCES})

target_include_directories(core
    PUBLIC
        ${MVVM_INCLUDE_DIR}
        ${NOESIS_INCLUDE_DIR}
)

target_precompile_headers(core
    PRIVATE
        "<mvvm/Model.h>"
        "<mvvm/ModelCollection.h>"
        "<mvvm/PropertyList.h>"
        "<mvvm/ViewModel.h>"
        "<mvvm/ViewModelCollection.h>"
)

target_link_libraries(core
    PUBLIC
        ${libNoesis}
)

add_subdirectory(core)
add_subdirectory(reflection)
add_subdirectory(codegen)
