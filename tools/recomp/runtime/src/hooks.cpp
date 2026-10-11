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
#include "librecomp/game.hpp"
#include "librecomp/overlays.hpp"
#include "rush2049_images.h"   // generated: stream ROM offset -> section
#include "rush_runtime.h"

// The previous code image is checked against the synthetic ROM's copy (what
// N64Recomp compiled) at the next inflate call: by then the recompiled inflate
// kernels have finished writing it. Data words inside the image may already
// have been written by the game, so a few differences are expected; a broken
// inflate shows as almost everything differing.
static const RushImage* pending_check = nullptr;

static void check_previous_image(uint8_t* rdram) {
    if (pending_check == nullptr) return;
    const RushImage& img = *pending_check;
    pending_check = nullptr;
    std::span<const uint8_t> rom = recomp::get_rom();
    if (rom.size() < img.section_rom + img.size) return;
    uint32_t same = 0, words = img.size / 4;
    for (uint32_t i = 0; i < words; i++) {
        const uint8_t* b = rom.data() + img.section_rom + i * 4;
        uint32_t want = (uint32_t)b[0] << 24 | (uint32_t)b[1] << 16 | (uint32_t)b[2] << 8 | b[3];
        same += (uint32_t)MEM_W(i * 4, (gpr)(int32_t)img.vram) == want;
    }
    rush::trace_event("inflated %s image check: %u/%u words equal the compiled image", img.name, same, words);
}

extern "C" void rush_hook_inflate(uint8_t* rdram, recomp_context* ctx) {
    check_previous_image(rdram);
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
        if (dst == img.vram) pending_check = &img;
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
