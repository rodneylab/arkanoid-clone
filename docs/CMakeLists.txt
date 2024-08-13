option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which enables building the docs together with the application
    add_custom_target(
        doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else(DOXYGEN_FOUND)
    message(
        "Doxygen needs to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)

find_package(Sphinx REQUIRED)
set(SPHINX_SOURCE ${CMAKE_CURRENT_BINARY_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc_doxygen)
add_custom_command(
    TARGET doc_doxygen
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../docs/index.rst
        ${CMAKE_CURRENT_BINARY_DIR}/index.rst
    COMMAND
        ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../docs/conf.py
        ${CMAKE_CURRENT_BINARY_DIR}/conf.py)
add_custom_target(
    Sphinx ALL
    COMMAND
        ${SPHINX_EXECUTABLE} -b html
        -Dbreathe_projects.Arkanoid=${DOXYGEN_OUTPUT_DIR}/xml ${SPHINX_SOURCE}
        ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Sphinx")
