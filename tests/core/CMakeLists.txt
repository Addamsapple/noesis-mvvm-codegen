add_executable(core_tests
    main.cpp

    Model.tests.cpp
    ModelCollection.tests.cpp
    ViewModel.tests.cpp
    ViewModelCollection.tests.cpp

    ${MVVM_SOURCES}
)

target_include_directories(core_tests
    PRIVATE
        ${MVVM_INCLUDE_DIR}
        ${NOESIS_INCLUDE_DIR}
)

target_link_libraries(core_tests
    PRIVATE
        ${libNoesis}
        doctest::doctest
)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

doctest_discover_tests(core_tests TEST_PREFIX [CORE])
