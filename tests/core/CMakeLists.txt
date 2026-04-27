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

add_custom_command(TARGET core_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t
        $<TARGET_FILE_DIR:core_tests>
        $<TARGET_RUNTIME_DLLS:core_tests>
    COMMAND_EXPAND_LISTS
)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

doctest_discover_tests(core_tests TEST_PREFIX [CORE])
