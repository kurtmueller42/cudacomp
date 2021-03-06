cmake_minimum_required(VERSION 2.6)
project(comp)

##############################################
# CONFIGURATION                              #
##############################################

if (NOT (DEFINED LLVM_PATH))
message(FATAL_ERROR "LLVM_PATH variable is required. See README.txt for help.")
endif ()

set(LLVM_LLC     "${LLVM_PATH}/llc")
set(LLVM_CONFIG  "${LLVM_PATH}/llvm-config")

##############################################
# END CONFIGURATION                          #
##############################################

# RPath
set(CMAKE_INSTALL_RPATH "\$ORIGIN/")
set(CMAKE_INSTALL_NAME_DIR "@executable_path/")

execute_process(COMMAND ${LLVM_CONFIG} --version OUTPUT_VARIABLE LLVM_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(LLVM_LIBS "libLLVM-${LLVM_VERSION}.dylib")
else ()
set(LLVM_LIBS "libLLVM-${LLVM_VERSION}.so")
endif ()

include_directories(include)

find_package(FLEX)
find_package(BISON)

BISON_TARGET(parser src/parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp VERBOSE parser.txt)
FLEX_TARGET(lexer src/parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Configure main compiler executable

add_executable(cc
    src/ast/astnode.cpp
    src/ast/astprint.cpp
    src/ast/astseqnode.cpp
    src/ast/astvisitor.cpp
    src/ast/expr/astallocarrayexp.cpp
    src/ast/expr/astallocexp.cpp
    src/ast/expr/astbinopexp.cpp
    src/ast/expr/astbooleanexp.cpp
    src/ast/expr/astcallexp.cpp
    src/ast/expr/astderefexp.cpp
    src/ast/expr/astexpnode.cpp
    src/ast/expr/astfloatexp.cpp
    src/ast/expr/astidentifierexp.cpp
    src/ast/expr/astindexexp.cpp
    src/ast/expr/astintegerexp.cpp
    src/ast/expr/astnullexp.cpp
    src/ast/expr/astrecordaccessexp.cpp
    src/ast/expr/astternopexp.cpp
    src/ast/expr/astunopexp.cpp
    src/ast/stmt/astassertstmt.cpp
    src/ast/stmt/astassignstmt.cpp
    src/ast/stmt/astexprstmt.cpp
    src/ast/stmt/astifstmt.cpp
    src/ast/stmt/astforstmt.cpp
    src/ast/stmt/astreturnstmt.cpp
    src/ast/stmt/astscopestmt.cpp
    src/ast/stmt/aststmtnode.cpp
    src/ast/stmt/astvardeclstmt.cpp
    src/ast/stmt/astwhilestmt.cpp
    src/ast/decl/astfundecl.cpp
    src/ast/decl/astdeclnode.cpp
    src/ast/decl/astrecorddecl.cpp
    src/ast/decl/asttypedecl.cpp
    src/ast/type/astargnode.cpp
    src/ast/type/astarrtype.cpp
    src/ast/type/astbooleantype.cpp
    src/ast/type/astfloattype.cpp
    src/ast/type/astfuntype.cpp
    src/ast/type/astidtype.cpp
    src/ast/type/astintegertype.cpp
    src/ast/type/astptrtype.cpp
    src/ast/type/astrecordtype.cpp
    src/ast/type/asttypenode.cpp
    src/ast/type/astvoidtype.cpp
    src/codegen/codegen.cpp
    src/codegen/codegenctx.cpp
    src/codegen/converttype.cpp
    src/main.cpp
    src/options.cpp
    src/parser/parse.cpp
    src/statics/functioninfo.cpp
    src/statics/moduleinfo.cpp
    src/statics/returncheck.cpp
    src/statics/statics.cpp
    src/statics/symbolcheck.cpp
    src/statics/symboltable.cpp
    src/statics/typecheck.cpp
    src/statics/typeresolve.cpp
    src/statics/funcheck.cpp
    src/statics/exceptions.cpp
    src/statics/symbolmangle.cpp
    src/statics/cudapartition.cpp
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
)

# TODO: -fexceptions hack might break LLVM exceptions
set_target_properties(cc PROPERTIES COMPILE_FLAGS "-std=c++11 -Wno-deprecated-register ${LLVM_CXX_FLAGS} -fexceptions")

target_link_libraries(cc
    ${FLEX_LIBRARIES}
    ${BISON_LIBRARIES}
    ${LLVM_LD_FLAGS}
    ${LLVM_LIBS}
)

# Configure host and device runtime libraries

add_library(host_rt
    runtime/host_rt.c
    runtime/host_main.c
)
# Configure documentation

find_package(Doxygen)

if (DOXYGEN_FOUND)
# Substitutes macros into Doxyfile to set up the build paths
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_Configured)

# Creates a 'make doc' target
add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_Configured
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building API documentation" VERBATIM)
endif (DOXYGEN_FOUND)

# Configure test driver
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/driver/driver.py
    ${CMAKE_CURRENT_BINARY_DIR}/driver.py)

# Install everything

install(TARGETS cc DESTINATION bin/)
install(TARGETS host_rt DESTINATION lib/)

# Autolab support

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runtime/host_rt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/l4lib.c)

add_custom_target(clean
    COMMAND rm -rf bin/ lexer.cpp parser.output cc driver.py
        lib parser.cpp Doxyfile_Configured libhost_rt.a parser.hpp l4lib.bc
)
