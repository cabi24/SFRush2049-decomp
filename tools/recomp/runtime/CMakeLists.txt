# Rush 2049 recomp host: links N64Recomp's output for the four code
# populations against N64ModernRuntime (librecomp + ultramodern). Nothing here
# is ROM-derived; the recompiler output and the synthetic ROM come from
# build/recomp (tools/recomp/gen_config.py + N64Recomp).
#
#   cmake -S tools/recomp/runtime -B build/recomp/host -G Ninja \
#         -DN64MODERNRUNTIME_DIR=~/rush2049/recomp/src/N64ModernRuntime \
#         -DRECOMP_OUT=$PWD/build/recomp [-DRUSH_RENDERER=rt64 -DRT64_DIR=path/to/rt64]
#   ninja -C build/recomp/host
#
# Options (Phase 4):
#   RUSH_RENDERER  null (default: headless, scripted input; the oracle's reference)
#                  | rt64 (adds an SDL2 window, RT64 and live controller input;
#                  --renderer null still selects the null renderer at run time)
#   RT64_DIR       RT64 checkout (recursive) at the commit in tools/recomp/VERSIONS.md
#   RUSH_CAPTURE   per-call capture (--call-log); POSIX only, default OFF on Windows
#
# Toolchains: Linux clang/clang++ (gcc untested); Windows clang-cl from Visual
# Studio 2022 (LLVM) with Ninja, as Zelda64Recomp builds.
cmake_minimum_required(VERSION 3.20)
project(rush2049_recomp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(RUSH_RENDERER "null" CACHE STRING "null | rt64")
set_property(CACHE RUSH_RENDERER PROPERTY STRINGS null rt64)
set(RT64_DIR "" CACHE PATH "RT64 checkout (recursive) for RUSH_RENDERER=rt64")
if(WIN32)
    option(RUSH_CAPTURE "per-call capture (POSIX mprotect/SIGSEGV)" OFF)
else()
    option(RUSH_CAPTURE "per-call capture (POSIX mprotect/SIGSEGV)" ON)
endif()
if(NOT RUSH_RENDERER STREQUAL "null" AND NOT RUSH_RENDERER STREQUAL "rt64")
    message(FATAL_ERROR "RUSH_RENDERER must be null or rt64")
endif()
if(MSVC AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "use clang-cl (Visual Studio's LLVM toolset), not MSVC cl.exe: "
                        "the recompiled C relies on clang extensions")
endif()
if(WIN32)
    add_compile_definitions(NOMINMAX _CRT_SECURE_NO_WARNINGS _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)
endif()
message(STATUS "Rush 2049 host: renderer ${RUSH_RENDERER}, capture ${RUSH_CAPTURE}")

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

if(RUSH_RENDERER STREQUAL "rt64")
    if(NOT EXISTS "${RT64_DIR}/CMakeLists.txt")
        message(FATAL_ERROR "RUSH_RENDERER=rt64: set -DRT64_DIR to an RT64 checkout (git clone --recursive)")
    endif()
    set(RT64_STATIC TRUE)
    set(RT64_SDL_WINDOW_VULKAN TRUE)
    add_compile_definitions(HLSL_CPU)
    add_subdirectory(${RT64_DIR} ${CMAKE_BINARY_DIR}/rt64)
    target_include_directories(rt64 PRIVATE ${CMAKE_BINARY_DIR}/rt64/src)
    # RT64 builds with -Werror; newer clang (21: -Wuninitialized-const-pointer in
    # its bundled imgui) adds warnings the pinned commit predates.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(rt64 PRIVATE -Wno-error)
    endif()
endif()

add_subdirectory(${N64MODERNRUNTIME_DIR} ${CMAKE_BINARY_DIR}/N64ModernRuntime)

# Runtime-reimplemented libultra calls the host intercepts (lockstep.cpp,
# oracle.cpp): every call in the recompiled code is renamed to __wrap_NAME by a
# compile definition; src/rush_wrap.h maps __real_NAME back. Portable (no
# linker --wrap). Keep in sync with src/rush_wrap.h.
set(RUSH_WRAPPED
    osGetCount_recomp osGetTime_recomp osViSetEvent_recomp osSetEventMesg_recomp
    osSpTaskStartGo_recomp osViSwapBuffer_recomp osViGetCurrentFramebuffer_recomp
    osViGetNextFramebuffer_recomp osCreateThread_recomp osRecvMesg_recomp osSendMesg_recomp)
set(RUSH_WRAP_DEFS "")
foreach(name ${RUSH_WRAPPED})
    list(APPEND RUSH_WRAP_DEFS "${name}=__wrap_${name}")
endforeach()
# Controller pak (Phase 6, src/pak.cpp): the runtime's osPfs* are "no pak" stubs;
# the recompiled code calls rush_NAME instead (the stubs stay linked, unused).
set(RUSH_PAK_RENAMED
    osPfsInitPak_recomp osPfsRepairId_recomp osPfsChecker_recomp osPfsFreeBlocks_recomp
    osPfsNumFiles_recomp osPfsFindFile_recomp osPfsAllocateFile_recomp osPfsDeleteFile_recomp
    osPfsFileState_recomp osPfsReadWriteFile_recomp)
foreach(name ${RUSH_PAK_RENAMED})
    list(APPEND RUSH_WRAP_DEFS "${name}=rush_${name}")
endforeach()

# Recompiled game: every funcs_*.c from N64Recomp (lookup.cpp and
# recomp_overlays.inl are consumed by src/register_overlays.cpp).
file(GLOB RECOMP_C_SOURCES ${RECOMP_OUT}/RecompiledFuncs/*.c)
add_library(RecompiledFuncs STATIC ${RECOMP_C_SOURCES} ${RECOMP_OUT}/RecompiledFuncs/lookup.cpp)
target_compile_definitions(RecompiledFuncs PRIVATE ${RUSH_WRAP_DEFS})
if(MSVC)
    target_compile_options(RecompiledFuncs PRIVATE /W0 /clang:-fno-strict-aliasing
        $<$<COMPILE_LANGUAGE:C>:-Wno-implicit-function-declaration>)
else()
    target_compile_options(RecompiledFuncs PRIVATE
        -fno-strict-aliasing -w
        $<$<COMPILE_LANGUAGE:C>:-Wno-implicit-function-declaration>)
endif()
target_include_directories(RecompiledFuncs PRIVATE
    ${N64MODERNRUNTIME_DIR}/N64Recomp/include
    ${N64MODERNRUNTIME_DIR}/ultramodern/include
    ${N64MODERNRUNTIME_DIR}/librecomp/include)

# Phase 2 native replacements (tools/recomp/replace.py mode "native"): host C
# bodies of game functions and their o32 wrappers, generated per build.
file(GLOB NATIVE_REPL_SOURCES ${RECOMP_OUT}/replace/native/*.c)
if(NATIVE_REPL_SOURCES)
    add_library(NativeRepl STATIC ${NATIVE_REPL_SOURCES})
    if(MSVC)
        target_compile_options(NativeRepl PRIVATE /W0 /clang:-fno-strict-aliasing /J /clang:-fwrapv)
    else()
        target_compile_options(NativeRepl PRIVATE -w -fno-strict-aliasing -funsigned-char -fwrapv)
    endif()
    target_include_directories(NativeRepl PRIVATE
        ${N64MODERNRUNTIME_DIR}/N64Recomp/include ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)
    set(NATIVE_REPL_LIB NativeRepl)
endif()

# Audio microcode (Phase 5): RSPRecomp output of the game's audio ucode
# (gen_config.py writes rush2049.rsp_audio.toml; `RSPRecomp rush2049.rsp_audio.toml`
# in RECOMP_OUT writes rsp/rush_audio_ucode.cpp). Without it audio tasks
# complete without running (silence), as before.
set(RUSH_AUDIO_UCODE "${RECOMP_OUT}/rsp/rush_audio_ucode.cpp")
if(EXISTS "${RUSH_AUDIO_UCODE}")
    add_library(RushRspUcode STATIC ${RUSH_AUDIO_UCODE})
    target_include_directories(RushRspUcode PRIVATE
        ${N64MODERNRUNTIME_DIR}/librecomp/include ${N64MODERNRUNTIME_DIR}/ultramodern/include
        ${N64MODERNRUNTIME_DIR}/N64Recomp/include)
    # The RSP vector unit implementation uses SSE4.1 intrinsics (Zelda64Recomp: -march=nehalem).
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        target_compile_options(RushRspUcode PRIVATE -march=nehalem)
    endif()
    if(MSVC)
        target_compile_options(RushRspUcode PRIVATE /W0)
    else()
        target_compile_options(RushRspUcode PRIVATE -w)
    endif()
    set(RUSH_RSP_LIB RushRspUcode)
    set(RUSH_AUDIO_UCODE_STATUS "recompiled audio ucode")
else()
    set(RUSH_RSP_LIB "")
    set(RUSH_AUDIO_UCODE_STATUS "no audio ucode (silent; run RSPRecomp rush2049.rsp_audio.toml in RECOMP_OUT)")
endif()
message(STATUS "Rush 2049 host: ${RUSH_AUDIO_UCODE_STATUS}")

set(RUSH_SOURCES
    src/main.cpp
    src/null_renderer.cpp
    src/register_overlays.cpp
    src/hooks.cpp
    src/shims.cpp
    src/trace.cpp
    src/oracle.cpp
    src/capture.cpp
    src/fault.cpp
    src/lockstep.cpp
    src/pak.cpp)
if(RUSH_RENDERER STREQUAL "rt64")
    list(APPEND RUSH_SOURCES src/rt64_renderer.cpp src/sdl_frontend.cpp)
endif()
add_executable(rush2049_recomp ${RUSH_SOURCES})
target_compile_definitions(rush2049_recomp PRIVATE RUSH_CAPTURE=$<BOOL:${RUSH_CAPTURE}>)
if(RUSH_RSP_LIB)
    target_compile_definitions(rush2049_recomp PRIVATE RUSH_HAVE_AUDIO_UCODE)
endif()
target_include_directories(rush2049_recomp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${RECOMP_OUT}
    ${N64MODERNRUNTIME_DIR}/N64Recomp/include
    ${N64MODERNRUNTIME_DIR}/thirdparty)
# --clock frames replaces the wall clock the game reads (src/oracle.cpp) and
# delivers VI/AI/SP/DP events in lockstep (src/lockstep.cpp); the calls are
# renamed in RecompiledFuncs (RUSH_WRAPPED above).
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
if(MSVC)
    # link.exe / lld-link search static libraries repeatedly: no grouping needed.
    target_link_libraries(rush2049_recomp PRIVATE RecompiledFuncs ${NATIVE_REPL_LIB} ${RUSH_RSP_LIB} librecomp ultramodern)
else()
    # The recompiled code and the runtime reference each other: group the archives.
    target_link_libraries(rush2049_recomp PRIVATE
        -Wl,--start-group RecompiledFuncs ${NATIVE_REPL_LIB} ${RUSH_RSP_LIB} librecomp ultramodern -Wl,--end-group
        Threads::Threads ${CMAKE_DL_LIBS})
endif()

if(RUSH_RENDERER STREQUAL "rt64")
    target_compile_definitions(rush2049_recomp PRIVATE RUSH_HAVE_RT64)
    target_include_directories(rush2049_recomp PRIVATE
        ${RT64_DIR}/src ${RT64_DIR}/src/contrib ${RT64_DIR}/src/contrib/hlslpp/include
        ${RT64_DIR}/src/contrib/dxc/inc ${RT64_DIR}/src/rhi ${RT64_DIR}/src/render
        ${CMAKE_BINARY_DIR}/rt64/src)
    target_link_libraries(rush2049_recomp PRIVATE rt64)
    if(WIN32)
        # SDL2 2.26.3 ships with RT64 (src/contrib/mupen64plus-win32-deps); the DLLs
        # the executable needs are copied next to it.
        set(RUSH_SDL2 "${RT64_DIR}/src/contrib/mupen64plus-win32-deps/SDL2-2.26.3")
        target_include_directories(rush2049_recomp PRIVATE ${RUSH_SDL2}/include)
        target_link_directories(rush2049_recomp PRIVATE ${RUSH_SDL2}/lib/x64)
        target_link_libraries(rush2049_recomp PRIVATE SDL2 Winmm.lib)
        add_custom_command(TARGET rush2049_recomp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${RUSH_SDL2}/lib/x64/SDL2.dll"
                "${RT64_DIR}/src/contrib/dxc/bin/x64/dxil.dll"
                "${RT64_DIR}/src/contrib/dxc/bin/x64/dxcompiler.dll"
                $<TARGET_FILE_DIR:rush2049_recomp>)
    else()
        find_package(SDL2 REQUIRED)
        target_compile_definitions(rush2049_recomp PRIVATE RT64_SDL_WINDOW_VULKAN)
        target_include_directories(rush2049_recomp PRIVATE ${SDL2_INCLUDE_DIRS})
        target_link_libraries(rush2049_recomp PRIVATE SDL2::SDL2)
    endif()
endif()
