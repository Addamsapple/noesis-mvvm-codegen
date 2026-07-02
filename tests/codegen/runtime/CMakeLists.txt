add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp
    DEPENDS
        codegen_venv
        ${MODEL_CODEGEN_TEMPLATES}
        ${CMAKE_CURRENT_SOURCE_DIR}/models.xml
    COMMAND ${VENV_PYTHON} ${CODEGEN_DIR}/codegen.py
        --input ${CMAKE_CURRENT_SOURCE_DIR}/models.xml
        --template_dir ${CODEGEN_DIR}/templates
        --many model/header/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/model/{props['name']}.h
        --many model/source/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/model/{props['name']}.cpp
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating models..."
    USES_TERMINAL
    VERBATIM
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp
    DEPENDS
        codegen_venv
        ${VIEWMODEL_CODEGEN_TEMPLATES}
        ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels.xml
    COMMAND
        ${VENV_PYTHON} ${CODEGEN_DIR}/codegen.py
        --input ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels.xml
        --template_dir ${CODEGEN_DIR}/templates
        --many viewmodel/header/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/{props['name']}.h
        --many viewmodel/source/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/{props['name']}.cpp
        --many viewmodel/inc.jinja ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/{props['name']}.inc
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating viewmodels..."
    USES_TERMINAL
    VERBATIM
)

add_custom_target(model_codegen DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp)
add_custom_target(viewmodel_codegen DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp)

set(GENERATED_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/model/League.h
    ${CMAKE_CURRENT_BINARY_DIR}/model/Team.h
    ${CMAKE_CURRENT_BINARY_DIR}/model/Player.h
    ${CMAKE_CURRENT_BINARY_DIR}/model/Goalkeeper.h
    ${CMAKE_CURRENT_BINARY_DIR}/model/Outfielder.h
    ${CMAKE_CURRENT_BINARY_DIR}/model/Midfielder.h

    ${CMAKE_CURRENT_BINARY_DIR}/model/League.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/model/Team.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/model/Player.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/model/Goalkeeper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/model/Outfielder.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/model/Midfielder.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/League.h
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Team.h
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Player.h
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Goalkeeper.h
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Outfielder.h
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Midfielder.h

    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/League.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Team.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Player.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Goalkeeper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Outfielder.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Midfielder.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/League.inc
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Team.inc
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Player.inc
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Goalkeeper.inc
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Outfielder.inc
    ${CMAKE_CURRENT_BINARY_DIR}/viewmodel/Midfielder.inc
)

set_source_files_properties(${GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

add_executable(codegen_runtime_tests
    main.cpp

    Factory.h
    Factory.cpp

    Compare.h
    Compare.cpp

    League.tests.cpp
    Team.tests.cpp
    Player.tests.cpp
    Goalkeeper.tests.cpp
    Outfielder.tests.cpp
    Midfielder.tests.cpp

    ${GENERATED_SOURCES}
)

target_precompile_headers(codegen_runtime_tests REUSE_FROM core)

target_include_directories(codegen_runtime_tests
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(codegen_runtime_tests
    PRIVATE
        core
        doctest::doctest
)

add_dependencies(codegen_runtime_tests model_codegen viewmodel_codegen)

add_custom_command(TARGET codegen_runtime_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t
        $<TARGET_FILE_DIR:codegen_runtime_tests>
        $<TARGET_RUNTIME_DLLS:codegen_runtime_tests>
    COMMAND_EXPAND_LISTS
)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

doctest_discover_tests(codegen_runtime_tests TEST_PREFIX [CODEGEN][RUNTIME])
