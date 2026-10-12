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
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <vector>

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
    rush::trace_event(rush::lockstep_enabled() ? "idle thread parked (lockstep: virtual VIs)"
                                               : "idle thread parked (pause_self)");
    rush::lockstep_idle(rdram);
}

// rush_hook_spin: inserted before the loop branch of every libultra-free
// busy-wait of the game (patches.json "hook" entries with "site_arg"; the
// list and how it was found: tools/recomp/README.md "Phase 5"). On the
// console the variable such a loop polls is changed by another thread or an
// interrupt while it spins; under ultramodern game threads switch only inside
// libultra calls, so the thread that would change it (the scheduler, woken by
// the renderer's asynchronous SP/DP completion, or by a VI) never runs. Same
// recipe as rush_hook_park_idle, one turn at a time: wait up to 1 ms for an
// external message (VI/AI/SP/DP/SI), deliver it, and let a woken
// higher-priority thread run; then the loop re-reads its variable.
// With --clock frames (lockstep) it does nothing: every event there is
// delivered synchronously at fixed points, so the loops never spin, and a
// no-op keeps the oracle's verdicts unchanged.
extern "C" void yield_self_1ms(uint8_t* rdram);
static void rush_burst_restart();

namespace {
std::atomic<uint64_t> spin_yields{0};
std::mutex spin_sites_mutex;
std::vector<uint32_t> spin_sites_seen;
bool spin_yield_enabled = true;
}

void rush::set_spin_yield(bool on) { spin_yield_enabled = on; }

uint64_t rush::spin_yield_count() { return spin_yields.load(); }

extern "C" void rush_hook_spin(uint8_t* rdram, recomp_context* ctx, uint32_t site) {
    (void)ctx;
    if (rush::lockstep_enabled() || !spin_yield_enabled) return;
    {
        std::lock_guard<std::mutex> lock(spin_sites_mutex);
        if (std::find(spin_sites_seen.begin(), spin_sites_seen.end(), site) == spin_sites_seen.end()) {
            spin_sites_seen.push_back(site);
            rush::trace_event("busy-wait at 0x%08X: yielding (rush_hook_spin)", site);
        }
    }
    spin_yields++;
    yield_self_1ms(rdram);
    rush_burst_restart();   // a yield is a switch point too (run-burst census below)
}

// Run-burst census (Phase 6). ultramodern switches game threads only inside
// libultra calls, so a long computation without one starves every other game
// thread in wall clock, as the busy-waits above did. The message calls are the
// switch points the game uses (osRecvMesg blocks, osSendMesg wakes): the time a
// thread runs between returning from one and entering the next is its burst.
// The longest burst so far and its end (the caller's ra) are reported when they
// pass 4 ms and at exit (status field max_burst_ms=). Wall clock only. The first
// 2 s are left out: the boot inflates and checks the game and image A in one
// ~25 ms run, before any other thread has work.
#include "rush_wrap.h"
extern "C" void osRecvMesg_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osSendMesg_recomp(uint8_t* rdram, recomp_context* ctx);
namespace {
std::mutex burst_mutex;
double burst_max_ms = 0;
const auto burst_epoch = std::chrono::steady_clock::now();
thread_local std::chrono::steady_clock::time_point burst_start{};
thread_local bool burst_running = false;
thread_local uint32_t burst_from = 0;   // ra of the call the burst started after

void burst_enter(recomp_context* ctx, const char* call) {
    if (!burst_running || rush::lockstep_enabled()) return;
    auto now = std::chrono::steady_clock::now();
    if (burst_start - burst_epoch < std::chrono::seconds(2)) return;
    double ms = std::chrono::duration<double, std::milli>(now - burst_start).count();
    if (ms <= burst_max_ms) return;
    std::lock_guard<std::mutex> lock(burst_mutex);
    if (ms <= burst_max_ms) return;
    bool report = ms >= 4.0 && ms >= 1.25 * burst_max_ms;
    burst_max_ms = ms;
    if (report)
        rush::trace_event("longest game-thread run without a message call so far: %.1f ms, from the call at "
                          "ra 0x%08X to %s at ra 0x%08X", ms, burst_from, call, (uint32_t)ctx->r31);
}
void burst_leave(uint32_t from) {
    burst_from = from;
    burst_start = std::chrono::steady_clock::now();
    burst_running = true;
}
}

static void rush_burst_restart() {
    if (burst_running) burst_leave(0xFFFFFFFF);   // ra unknown here: a busy-wait yield
}

double rush::max_burst_ms() {
    std::lock_guard<std::mutex> lock(burst_mutex);
    return burst_max_ms;
}

extern "C" void __wrap_osRecvMesg_recomp(uint8_t* rdram, recomp_context* ctx) {
    burst_enter(ctx, "osRecvMesg");
    burst_running = false;
    uint32_t ra = (uint32_t)ctx->r31;
    __real_osRecvMesg_recomp(rdram, ctx);
    burst_leave(ra);
}

extern "C" void __wrap_osSendMesg_recomp(uint8_t* rdram, recomp_context* ctx) {
    burst_enter(ctx, "osSendMesg");
    burst_running = false;
    uint32_t ra = (uint32_t)ctx->r31;
    __real_osSendMesg_recomp(rdram, ctx);
    burst_leave(ra);
}
