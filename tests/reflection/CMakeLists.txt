add_executable(reflection_tests
    main.cpp

    Model.tests.cpp
    ModelCollection.tests.cpp
    Property.tests.cpp
    PropertyList.tests.cpp
)

target_precompile_headers(reflection_tests REUSE_FROM core)

target_link_libraries(reflection_tests
    PRIVATE
        core
        doctest::doctest
)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

doctest_discover_tests(reflection_tests TEST_PREFIX [REFLECTION])
