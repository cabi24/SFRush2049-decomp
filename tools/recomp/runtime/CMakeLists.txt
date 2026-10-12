# Rush 2049 recomp host (Phase 1): links N64Recomp's output for the four code
# populations against N64ModernRuntime (librecomp + ultramodern) with a null
# renderer. Nothing here is ROM-derived; the recompiler output and the
# synthetic ROM come from build/recomp (tools/recomp/gen_config.py + N64Recomp).
#
#   cmake -S tools/recomp/runtime -B build/recomp/host -G Ninja \
#         -DN64MODERNRUNTIME_DIR=~/rush2049/recomp/src/N64ModernRuntime \
#         -DRECOMP_OUT=$PWD/build/recomp
#   ninja -C build/recomp/host
cmake_minimum_required(VERSION 3.20)
project(rush2049_recomp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(N64MODERNRUNTIME_DIR "" CACHE PATH "N64ModernRuntime checkout (recursive)")
set(RECOMP_OUT "" CACHE PATH "gen_config/N64Recomp output dir (build/recomp)")
if(NOT EXISTS "${N64MODERNRUNTIME_DIR}/librecomp/CMakeLists.txt")
    message(FATAL_ERROR "set -DN64MODERNRUNTIME_DIR to an N64ModernRuntime checkout")
endif()
if(NOT EXISTS "${RECOMP_OUT}/RecompiledFuncs/recomp_overlays.inl")
    message(FATAL_ERROR "set -DRECOMP_OUT to build/recomp after running N64Recomp")
endif()

# The runtime must define every shim and hook the generator recorded.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/check_contract.py
            --manifest ${RECOMP_OUT}/manifest.json --runtime ${CMAKE_CURRENT_SOURCE_DIR}/src
            --n64modernruntime ${N64MODERNRUNTIME_DIR}
            --extra-src ${RECOMP_OUT}/replace/native
    RESULT_VARIABLE contract_rc)
if(NOT contract_rc EQUAL 0)
    message(FATAL_ERROR "runtime contract check failed (see above)")
endif()

add_subdirectory(${N64MODERNRUNTIME_DIR} ${CMAKE_BINARY_DIR}/N64ModernRuntime)

# Recompiled game: every funcs_*.c from N64Recomp (lookup.cpp and
# recomp_overlays.inl are consumed by src/register_overlays.cpp).
file(GLOB RECOMP_C_SOURCES ${RECOMP_OUT}/RecompiledFuncs/*.c)
add_library(RecompiledFuncs STATIC ${RECOMP_C_SOURCES} ${RECOMP_OUT}/RecompiledFuncs/lookup.cpp)
target_compile_options(RecompiledFuncs PRIVATE
    -fno-strict-aliasing -w
    $<$<COMPILE_LANGUAGE:C>:-Wno-implicit-function-declaration>)
target_include_directories(RecompiledFuncs PRIVATE
    ${N64MODERNRUNTIME_DIR}/N64Recomp/include
    ${N64MODERNRUNTIME_DIR}/ultramodern/include
    ${N64MODERNRUNTIME_DIR}/librecomp/include)

# Phase 2 native replacements (tools/recomp/replace.py mode "native"): host C
# bodies of game functions and their o32 wrappers, generated per build.
file(GLOB NATIVE_REPL_SOURCES ${RECOMP_OUT}/replace/native/*.c)
if(NATIVE_REPL_SOURCES)
    add_library(NativeRepl STATIC ${NATIVE_REPL_SOURCES})
    target_compile_options(NativeRepl PRIVATE -w -fno-strict-aliasing -funsigned-char -fwrapv)
    target_include_directories(NativeRepl PRIVATE
        ${N64MODERNRUNTIME_DIR}/N64Recomp/include ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)
    set(NATIVE_REPL_LIB NativeRepl)
endif()

add_executable(rush2049_recomp
    src/main.cpp
    src/null_renderer.cpp
    src/register_overlays.cpp
    src/hooks.cpp
    src/shims.cpp
    src/trace.cpp
    src/oracle.cpp
    src/lockstep.cpp)
target_include_directories(rush2049_recomp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${RECOMP_OUT}
    ${N64MODERNRUNTIME_DIR}/N64Recomp/include
    ${N64MODERNRUNTIME_DIR}/thirdparty)
# --clock frames replaces the wall clock the game reads (src/oracle.cpp) and
# delivers VI/AI/SP/DP events in lockstep (src/lockstep.cpp).
target_link_options(rush2049_recomp PRIVATE
    -Wl,--wrap=osGetCount_recomp -Wl,--wrap=osGetTime_recomp
    -Wl,--wrap=osViSetEvent_recomp -Wl,--wrap=osSetEventMesg_recomp
    -Wl,--wrap=osSpTaskStartGo_recomp -Wl,--wrap=osViSwapBuffer_recomp
    -Wl,--wrap=osViGetCurrentFramebuffer_recomp -Wl,--wrap=osViGetNextFramebuffer_recomp)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
# The recompiled code and the runtime reference each other: group the archives.
target_link_libraries(rush2049_recomp PRIVATE
    -Wl,--start-group RecompiledFuncs ${NATIVE_REPL_LIB} librecomp ultramodern -Wl,--end-group
    Threads::Threads ${CMAKE_DL_LIBS})
