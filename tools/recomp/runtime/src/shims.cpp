// Host shims: functions the recompiled code calls that neither N64Recomp
// emits nor N64ModernRuntime implements. The generator records the required
// set in build/recomp/manifest.json "runtime_contract.shims";
// check_contract.py (run by CMake) fails the build if one is missing here.
//
// Three kinds:
//   FORWARD  the callee is a libultra function the runtime implements under
//            its canonical name; the shim calls the runtime's version.
//   HOST     faithful host C (64-bit conversions, cop0 reads).
//   INERT    hardware/interrupt internals whose only callers are libultra
//            paths the runtime replaces (dead under the runtime) or that
//            have no host meaning; the shim logs its first call (with the
//            return address) and returns 0 / does nothing.
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "recomp.h"
#include "librecomp/game.hpp"
#include "rush_runtime.h"

#define SHIM(name) extern "C" void name(uint8_t* rdram, recomp_context* ctx)
#define FIRST_CALL(name) rush::trace_shim(#name, (uint32_t)ctx->r31)

// Runtime (librecomp/ultramodern) implementations the forwards call.
extern "C" {
void osPiStartDma_recomp(uint8_t* rdram, recomp_context* ctx);
}

// ---------------------------------------------------------------- HOST

// long long __fixdfdi(double) (lib_a5f0.c): N64Recomp has no trunc.l.d.
// o32: the double arrives in $f12/$f13, the 64-bit result returns in v0:v1.
SHIM(__fixdfdi) {
    (void)rdram;
    int64_t v = (int64_t)ctx->f12.d;
    ctx->r2 = (gpr)(int32_t)(uint32_t)((uint64_t)v >> 32);
    ctx->r3 = (gpr)(int32_t)(uint32_t)(uint64_t)v;
}

// long long __fixsfdi(float) (lib_a5f0.c): no trunc.l.s.
SHIM(__fixsfdi) {
    (void)rdram;
    int64_t v = (int64_t)ctx->f12.fl;
    ctx->r2 = (gpr)(int32_t)(uint32_t)((uint64_t)v >> 32);
    ctx->r3 = (gpr)(int32_t)(uint32_t)(uint64_t)v;
}

// u32 osCauseGet(void): mfc0 Cause. Its one caller (lib_8a80) spins forever
// on bit 12 (a pending pre-NMI interrupt); the host has none.
SHIM(osCauseGet) {
    (void)rdram;
    ctx->r2 = 0;
}

// ---------------------------------------------------------------- FORWARD

// Inventory name __osPiRawStartDma (0x80008630); body is osPiStartDma(mb, pri,
// dir, devAddr, dram, size, mq) (src/rom/lib_9230.c): same arguments.
SHIM(__osPiRawStartDma_recomp) {
    osPiStartDma_recomp(rdram, ctx);
}

// ---------------------------------------------------------------- INERT

#define INERT(name)        \
    SHIM(name) {           \
        (void)rdram;       \
        FIRST_CALL(name);  \
        ctx->r2 = 0;       \
    }

// Interrupt/exception internals (the runtime replaces the exception path).
INERT(__osSendInterrupt)
INERT(__osCleanupThread_recomp)
INERT(__osSetCompare_recomp)
INERT(__osSetGlobalIntMask_recomp)
INERT(__osResetGlobalIntMask_recomp)
INERT(__osViSwapContext_recomp)
// RSP/RDP status registers (the runtime runs tasks itself).
INERT(__osSpDeviceBusy_recomp)
INERT(__osSpSetStatus_recomp)
// PI/SI raw access (callers are replaced or dead under the runtime).
INERT(__osEPiRawReadIo_recomp)
INERT(__osEPiRawWriteIo_recomp)
INERT(__osSiDeviceBusy_recomp)
INERT(__osSiGetAccess_recomp)
INERT(__osSiRelAccess_recomp)
INERT(__osSiRawStartDma_recomp)
INERT(__osPackReadData_recomp)
// Controller pak: no pak is reported connected; fail with PFS_ERR_NOPACK (1).
#define NOPACK(name)       \
    SHIM(name) {           \
        (void)rdram;       \
        FIRST_CALL(name);  \
        ctx->r2 = 1;       \
    }
NOPACK(__osContRamRead_recomp)
NOPACK(__osContRamWrite_recomp)
NOPACK(__osPfsRWInode_recomp)
NOPACK(__osPfsSelectBank_recomp)

// ---------------------------------------------------------------- RUNTIME GAPS
// Names on N64Recomp's reimplemented list that N64ModernRuntime does not define.

// s32 osPiReadIo(u32 devAddr, u32* data): one PI word read. Cartridge reads
// come from the ROM; anything else (PIF RAM, flash status) reads as 0.
SHIM(osPiReadIo_recomp) {
    FIRST_CALL(osPiReadIo_recomp);
    uint32_t dev = (uint32_t)ctx->r4 & 0x1FFFFFFF;
    if (dev >= 0x10000000 && dev < 0x1FC00000) {
        recomp::do_rom_pio(rdram, ctx->r5, dev);
    } else {
        MEM_W(0, ctx->r5) = 0;
    }
    ctx->r2 = 0;
}
