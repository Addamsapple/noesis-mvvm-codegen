add_executable(reflection_tests
    main.cpp

    Model.tests.cpp
    ModelCollection.tests.cpp
    Property.tests.cpp
    PropertyList.tests.cpp

    ${MVVM_SOURCES}
)

# TODO: shouldn't need Noesis

target_include_directories(reflection_tests
    PRIVATE
        ${MVVM_INCLUDE_DIR}
        ${NOESIS_INCLUDE_DIR}
)

target_link_libraries(reflection_tests
    PRIVATE
        ${libNoesis}
        doctest::doctest
)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

doctest_discover_tests(reflection_tests TEST_PREFIX [REFLECTION])
