// [[patches.hook]] callees (tools/recomp/patches.json "hook").
//
// rush_hook_inflate: runs at the entry of the recompiled inflate_entry(src, dst, n).
// Every compressed code image goes through it: the game image at boot
// (game_init: src 0xB0CB10 -> 0x80086A50) and overlay image A or B
// (PrevMaxPath -> inflate_decompress: src = D_8002B020 / D_8002B024 ->
// 0x8038A400). The recompiled inflate still runs and writes the bytes into
// rdram; the hook only tells librecomp which recompiled section now lives at
// dst, so LOOKUP_FUNC / get_function dispatch to it. Images A and B share
// 0x8038A400: whatever is loaded there is unloaded first.
#include <cstdio>

#include "recomp.h"
#include "librecomp/overlays.hpp"
#include "rush2049_images.h"   // generated: stream ROM offset -> section
#include "rush_runtime.h"

extern "C" void rush_hook_inflate(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    uint32_t src = (uint32_t)ctx->r4;
    uint32_t dst = (uint32_t)ctx->r5;
    for (const RushImage& img : rush_images) {
        if (img.size == 0 || img.stream_rom != src) {
            continue;
        }
        if (dst != img.vram) {
            fprintf(stderr, "[rush] inflate of %s to 0x%08X, recompiled for 0x%08X\n", img.name, dst, img.vram);
        }
        // Drop every section loaded at the destination (A <-> B swap): the
        // span is the largest image sharing this vram, so neither is partial.
        uint32_t span = 0;
        for (const RushImage& other : rush_images) {
            if (other.vram == img.vram && other.size > span) span = other.size;
        }
        unload_overlays((int32_t)dst, span);
        load_overlays(img.section_rom, (int32_t)dst, img.size);
        rush::trace_overlay(img.name, src, dst);
        return;
    }
    rush::trace_event("inflate src 0x%06X -> 0x%08X (data, no code image)", src, dst);
}

// rush_hook_park_idle: replaces idle_thread_entry's final `for (;;) {}`
// (0x80002218) with ultramodern's pause_self: the idle thread stays the
// current thread but waits for external events (VI, SP, SI, timers) and
// swaps to whichever higher-priority thread they wake, instead of spinning
// (ultramodern switches threads only inside libultra calls).
extern "C" void pause_self(uint8_t* rdram);

extern "C" void rush_hook_park_idle(uint8_t* rdram, recomp_context* ctx) {
    (void)ctx;
    rush::trace_event("idle thread parked (pause_self)");
    pause_self(rdram);
}
