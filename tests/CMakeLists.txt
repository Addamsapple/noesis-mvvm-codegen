include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(doctest)

add_library(Noesis SHARED IMPORTED)

if(WIN32)
    set_target_properties(Noesis PROPERTIES
        IMPORTED_IMPLIB ${NOESIS_DIR}/lib/Noesis.lib
        IMPORTED_LOCATION ${NOESIS_DIR}/bin/Noesis.dll
        INTERFACE_INCLUDE_DIRECTORIES ${NOESIS_DIR}/include
    )
else()
    set_target_properties(Noesis PROPERTIES
        IMPORTED_LOCATION ${NOESIS_DIR}/lib/libNoesis.so
        INTERFACE_INCLUDE_DIRECTORIES ${NOESIS_DIR}/include
    )
endif()

add_library(core STATIC ${MVVM_SOURCES})

target_include_directories(core
    PUBLIC
        ${MVVM_INCLUDE_DIR}
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
        Noesis
)

add_subdirectory(core)
add_subdirectory(reflection)
add_subdirectory(codegen)
