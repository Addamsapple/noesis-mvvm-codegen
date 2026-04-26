find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(VENV_DIR ${CMAKE_CURRENT_BINARY_DIR}/venv)
set(VENV_PYTHON ${VENV_DIR}/bin/python)
set(PYTHON_REQUIREMENTS ${CODEGEN_DIR}/requirements.txt)

add_custom_command(
    OUTPUT ${VENV_DIR}
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_DIR}
    COMMENT "Creating a virtual Python environment..."
)

add_custom_command(
    OUTPUT ${VENV_DIR}/venv.stamp
    DEPENDS ${VENV_DIR} ${PYTHON_REQUIREMENTS}
    COMMAND ${VENV_PYTHON} -m pip install --quiet --upgrade pip
    COMMAND ${VENV_PYTHON} -m pip install --quiet -r ${PYTHON_REQUIREMENTS}
    COMMAND ${CMAKE_COMMAND} -E touch ${VENV_DIR}/venv.stamp
    COMMENT "Installing required Python packages..."
)

add_custom_target(codegen_venv DEPENDS ${VENV_DIR}/venv.stamp)

set(MODEL_CODEGEN_DEPENDENCIES
    codegen_venv

    ${CMAKE_CURRENT_SOURCE_DIR}/models.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/.clang-format

    ${CODEGEN_DIR}/templates/utils.jinja

    ${CODEGEN_DIR}/templates/model/header/forwards.jinja
    ${CODEGEN_DIR}/templates/model/header/getter.jinja
    ${CODEGEN_DIR}/templates/model/header/includes.jinja
    ${CODEGEN_DIR}/templates/model/header/members.jinja
    ${CODEGEN_DIR}/templates/model/header/properties.jinja
    ${CODEGEN_DIR}/templates/model/header/setter.jinja
    ${CODEGEN_DIR}/templates/model/header/skeleton.jinja

    ${CODEGEN_DIR}/templates/model/source/getter.jinja
    ${CODEGEN_DIR}/templates/model/source/includes.jinja
    ${CODEGEN_DIR}/templates/model/source/setter.jinja
    ${CODEGEN_DIR}/templates/model/source/skeleton.jinja
)

set(VIEWMODEL_CODEGEN_DEPENDENCIES
    codegen_venv

    ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/.clang-format

    ${CODEGEN_DIR}/templates/utils.jinja
    ${CODEGEN_DIR}/templates/viewmodel/utils.jinja

    ${CODEGEN_DIR}/templates/viewmodel/header/forwards.jinja
    ${CODEGEN_DIR}/templates/viewmodel/header/getter.jinja
    ${CODEGEN_DIR}/templates/viewmodel/header/includes.jinja
    ${CODEGEN_DIR}/templates/viewmodel/header/properties.jinja
    ${CODEGEN_DIR}/templates/viewmodel/header/setter.jinja
    ${CODEGEN_DIR}/templates/viewmodel/header/skeleton.jinja

    ${CODEGEN_DIR}/templates/viewmodel/source/getter.jinja
    ${CODEGEN_DIR}/templates/viewmodel/source/handler.jinja
    ${CODEGEN_DIR}/templates/viewmodel/source/includes.jinja
    ${CODEGEN_DIR}/templates/viewmodel/source/observation.jinja
    ${CODEGEN_DIR}/templates/viewmodel/source/reflection.jinja
    ${CODEGEN_DIR}/templates/viewmodel/source/setter.jinja
    ${CODEGEN_DIR}/templates/viewmodel/source/skeleton.jinja

    ${CODEGEN_DIR}/templates/viewmodel/inc.jinja
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp
    DEPENDS ${MODEL_CODEGEN_DEPENDENCIES}
    COMMAND ${VENV_PYTHON} ${CODEGEN_DIR}/codegen.py
        --input ${CMAKE_CURRENT_SOURCE_DIR}/models.xml
        --template_dir ${CODEGEN_DIR}/templates
        --many model/header/skeleton.jinja generated/model/{props['name']}.h
        --many model/source/skeleton.jinja generated/model/{props['name']}.cpp
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating models..."
    USES_TERMINAL
    VERBATIM
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp
    DEPENDS ${VIEWMODEL_CODEGEN_DEPENDENCIES}
    COMMAND ${VENV_PYTHON} ${CODEGEN_DIR}/codegen.py
        --input ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels.xml
        --template_dir ${CODEGEN_DIR}/templates
        --many viewmodel/header/skeleton.jinja generated/viewmodel/{props['name']}.h
        --many viewmodel/source/skeleton.jinja generated/viewmodel/{props['name']}.cpp
        --many viewmodel/inc.jinja generated/viewmodel/{props['name']}.inc
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating viewmodels..."
    USES_TERMINAL
    VERBATIM
)

add_custom_target(model_codegen DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp)
add_custom_target(viewmodel_codegen DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp)

set(GENERATED_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/League.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Team.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Player.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Goalkeeper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Outfielder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Midfielder.h

    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/League.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Team.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Goalkeeper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Outfielder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/model/Midfielder.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/League.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Team.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Player.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Goalkeeper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Outfielder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Midfielder.h

    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/League.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Team.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Goalkeeper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Outfielder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Midfielder.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/League.inc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Team.inc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Player.inc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Goalkeeper.inc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Outfielder.inc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/viewmodel/Midfielder.inc
)

set_source_files_properties(${GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

add_executable(codegen_tests
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

    ${MVVM_SOURCES}
    ${GENERATED_SOURCES}
)

find_library(libNoesis Noesis REQUIRED)

target_include_directories(codegen_tests
    PRIVATE
        generated
        ${MVVM_INCLUDE_DIR}
        ${NOESIS_INCLUDE_DIR}
)

target_link_libraries(codegen_tests
    PRIVATE
        ${libNoesis}
        doctest::doctest
)

add_dependencies(codegen_tests model_codegen viewmodel_codegen)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

doctest_discover_tests(codegen_tests TEST_PREFIX [CODEGEN])
