add_executable(core_tests
    main.cpp

    Model.tests.cpp
    ModelCollection.tests.cpp
    ViewModel.tests.cpp
    ViewModelCollection.tests.cpp
)

target_precompile_headers(core_tests REUSE_FROM core)

target_link_libraries(core_tests
    PRIVATE
        core
        doctest::doctest
)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

doctest_discover_tests(core_tests TEST_PREFIX [CORE])
