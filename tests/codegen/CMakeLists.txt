find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(VENV_DIR ${CMAKE_CURRENT_BINARY_DIR}/venv)

if(WIN32)
    set(VENV_PYTHON ${VENV_DIR}/Scripts/python)
else()
    set(VENV_PYTHON ${VENV_DIR}/bin/python)
endif()

set(PYTHON_REQUIREMENTS ${CODEGEN_DIR}/requirements.txt)

add_custom_command(
    OUTPUT ${VENV_DIR}/../venv.stamp
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${VENV_DIR}/../venv.stamp
    COMMENT "Creating a virtual Python environment..."
)

add_custom_command(
    OUTPUT ${VENV_DIR}/../pip.stamp
    DEPENDS ${VENV_DIR}/../venv.stamp ${PYTHON_REQUIREMENTS}
    COMMAND ${VENV_PYTHON} -m pip install --quiet --upgrade pip
    COMMAND ${VENV_PYTHON} -m pip install --quiet -r ${PYTHON_REQUIREMENTS}
    COMMAND ${CMAKE_COMMAND} -E touch ${VENV_DIR}/../pip.stamp
    COMMENT "Installing required Python packages..."
)

add_custom_target(codegen_venv DEPENDS ${VENV_DIR}/../pip.stamp)

set(MODEL_CODEGEN_TEMPLATES
    ${CODEGEN_DIR}/templates/utils.jinja
    ${CODEGEN_DIR}/templates/model/utils.jinja

    ${CODEGEN_DIR}/templates/model/header/aliases.jinja
    ${CODEGEN_DIR}/templates/model/header/forwards.jinja
    ${CODEGEN_DIR}/templates/model/header/getter.jinja
    ${CODEGEN_DIR}/templates/model/header/includes.jinja
    ${CODEGEN_DIR}/templates/model/header/members.jinja
    ${CODEGEN_DIR}/templates/model/header/properties.jinja
    ${CODEGEN_DIR}/templates/model/header/setter.jinja
    ${CODEGEN_DIR}/templates/model/header/skeleton.jinja

    ${CODEGEN_DIR}/templates/model/source/getter.jinja
    ${CODEGEN_DIR}/templates/model/source/includes.jinja
    ${CODEGEN_DIR}/templates/model/source/properties.jinja
    ${CODEGEN_DIR}/templates/model/source/reflection.jinja
    ${CODEGEN_DIR}/templates/model/source/setter.jinja
    ${CODEGEN_DIR}/templates/model/source/skeleton.jinja
)

set(VIEWMODEL_CODEGEN_TEMPLATES
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

add_subdirectory(gentime)
add_subdirectory(runtime)
