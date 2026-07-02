add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp
    DEPENDS
        codegen_venv
        ${MODEL_CODEGEN_TEMPLATES}
        ${CMAKE_CURRENT_SOURCE_DIR}/models.xml
    COMMAND ${VENV_PYTHON} ${CODEGEN_DIR}/codegen.py
        --input ${CMAKE_CURRENT_SOURCE_DIR}/models.xml
        --template_dir ${CODEGEN_DIR}/templates
        --many model/header/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/{props['test']}/{props['name']}.h
        --many model/source/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/{props['test']}/{props['name']}.cpp
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
    COMMAND ${VENV_PYTHON} ${CODEGEN_DIR}/codegen.py
        --input ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels.xml
        --template_dir ${CODEGEN_DIR}/templates
        --many viewmodel/header/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/{props['test']}/{props['name']}.h
        --many viewmodel/source/skeleton.jinja ${CMAKE_CURRENT_BINARY_DIR}/{props['test']}/{props['name']}.cpp
        --many viewmodel/inc.jinja             ${CMAKE_CURRENT_BINARY_DIR}/{props['test']}/{props['name']}.inc
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating viewmodels..."
    USES_TERMINAL
    VERBATIM
)

add_custom_target(codegen_gentime_tests ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/model_codegen.stamp
        ${CMAKE_CURRENT_BINARY_DIR}/viewmodel_codegen.stamp
)

# --diff-filter=DM => diff all snapshot files, ignore extra build files
set(GIT_DIFF_COMMAND git diff --no-index --no-renames --diff-filter=DM --ignore-cr-at-eol)

add_test(
    NAME "[CODEGEN][GENTIME]Class without properties matches snapshot"
    COMMAND git diff --no-index --no-renames --diff-filter=DM --ignore-cr-at-eol
        ${CMAKE_CURRENT_SOURCE_DIR}/NoProps
        ${CMAKE_CURRENT_BINARY_DIR}/NoProps
)

add_test(
    NAME "[CODEGEN][GENTIME]Class with many properties matches snapshot"
    COMMAND ${GIT_DIFF_COMMAND}
        ${CMAKE_CURRENT_SOURCE_DIR}/ManyProps
        ${CMAKE_CURRENT_BINARY_DIR}/ManyProps
)
