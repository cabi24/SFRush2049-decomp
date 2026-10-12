// Lockstep mode (Phase 2 frame oracle, `--clock frames`): every event the
// game can observe is delivered at a deterministic point of its own
// execution instead of on wall time, so two runs of the same build produce
// the same frames (tools/recomp/oracle.py).
//
// Wall-time sources in N64ModernRuntime and what replaces them here (each
// libultra entry point is wrapped with `-Wl,--wrap=NAME_recomp`; without
// lockstep every wrapper calls the runtime's own function):
//   VI retrace / AI messages  ultramodern's VI thread (60 Hz wall clock).
//                             osViSetEvent / osSetEventMesg(AI) register NULL
//                             queues with the runtime and keep the real ones;
//                             the idle thread (which runs only when every
//                             other game thread is blocked) delivers one
//                             virtual VI per turn (VI message every
//                             retrace_count VIs, AI message every VI).
//   SP/DP task completion     the runtime's task/gfx threads. osSpTaskStartGo
//                             completes the task synchronously (no microcode:
//                             the null renderer executes nothing either) and
//                             sends the SP (and for graphics tasks the DP)
//                             message from the calling game thread.
//   current/next framebuffer  swapped by the VI thread; modelled here (next =
//                             osViSwapBuffer's, current = next at each VI).
//   osGetCount / osGetTime    virtual: 1/60 s per virtual VI plus 64 ticks per
//                             read (oracle.cpp virtual_count).
// With a real renderer (--renderer rt64, Phase 4) the graphics tasks are also
// MIRRORED to it: the runtime's osSpTaskStartGo queues the task for the
// renderer thread, whose SP/DP completion messages go to a private dummy
// queue (the game's SP/DP queues only receive the synchronous messages
// above), and the idle thread paces virtual VIs to 60 per wall-clock second so
// the pictures can be watched. The game's state stays deterministic as long as
// the renderer does not write rdram (RT64 framebuffer write-back would: see
// oracle.py --mask-renderer); the oracle's verdicts are defined on the null
// renderer. The game thread waits (host-side, invisible to the game) until the
// renderer has interpreted the mirrored display list before it sees SP/DP:
// otherwise the game rebuilds the list while RT64 still reads it (Phase 5:
// segfault in RT64::Interpreter at the race start with a slow software GPU).
// PI and SI completions are already sent synchronously by the calling game
// thread (librecomp pi.cpp, ultramodern send_si_message) and delivered at its
// next osRecvMesg, so they need nothing.
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <thread>
#include <mutex>
#include <vector>

#include "recomp.h"
#include "ultramodern/ultra64.h"
#include "ultramodern/ultramodern.hpp"
#include "rush_runtime.h"
#include "rush_wrap.h"

extern "C" s32 osSendMesg(RDRAM_ARG PTR(OSMesgQueue) mq, OSMesg msg, s32 flags);
extern "C" void pause_self(uint8_t* rdram);
extern "C" void osCreateMesgQueue(RDRAM_ARG PTR(OSMesgQueue) mq, PTR(OSMesg) msg, s32 count);
void dequeue_external_messages(uint8_t* rdram);  // ultramodern mesgqueue.cpp

namespace {

bool lockstep = false;
bool mirror = false;                       // forward gfx tasks to the real renderer
constexpr int32_t kDummyMq = (int32_t)0x80F00000u;   // above the game's 8 MiB (runtime rdram is 512 MiB)
constexpr int32_t kDummyMsgs = (int32_t)0x80F00040u;
constexpr int kDummyCount = 64;
bool dummy_ready = false;
std::chrono::steady_clock::time_point pace_t0;

void dummy_reset(uint8_t* rdram) {
    osCreateMesgQueue(rdram, kDummyMq, kDummyMsgs, kDummyCount);
    dummy_ready = true;
}

struct Event { int32_t mq = 0; uint32_t msg = 0; };
Event vi_ev, ai_ev, sp_ev, dp_ev;
uint32_t vi_retrace_count = 1, vi_remaining = 1;
uint32_t fb_next = 0, fb_cur = 0;
std::atomic<uint64_t> virtual_vis{0};
std::vector<Event> pending;  // messages a full queue refused; retried by the idle thread

// Mirrored gfx tasks: sent by the game thread, interpreted by the renderer thread.
std::mutex mirror_mu;
std::condition_variable mirror_cv;
uint64_t mirror_sent = 0, mirror_done = 0;

void send(uint8_t* rdram, const Event& e) {
    if (e.mq == 0) return;
    if (osSendMesg(rdram, e.mq, (OSMesg)e.msg, OS_MESG_NOBLOCK) != 0) pending.push_back(e);
}

void retry_pending(uint8_t* rdram) {
    std::vector<Event> again;
    again.swap(pending);
    for (const Event& e : again) send(rdram, e);
}

} // namespace

void rush::lockstep_mirror_done() {
    {
        std::lock_guard<std::mutex> lock(mirror_mu);
        mirror_done++;
    }
    mirror_cv.notify_all();
}

void rush::set_lockstep(bool on) { lockstep = on; }
void rush::set_lockstep_mirror(bool on) { mirror = on; }
bool rush::lockstep_enabled() { return lockstep; }
uint64_t rush::lockstep_vis() { return virtual_vis.load(); }

extern "C" {
void __real_osViSetEvent_recomp(uint8_t* rdram, recomp_context* ctx);
void __real_osSetEventMesg_recomp(uint8_t* rdram, recomp_context* ctx);
void __real_osSpTaskStartGo_recomp(uint8_t* rdram, recomp_context* ctx);
void __real_osViSwapBuffer_recomp(uint8_t* rdram, recomp_context* ctx);
void __real_osViGetCurrentFramebuffer_recomp(uint8_t* rdram, recomp_context* ctx);
void __real_osViGetNextFramebuffer_recomp(uint8_t* rdram, recomp_context* ctx);

void __wrap_osViSetEvent_recomp(uint8_t* rdram, recomp_context* ctx) {
    if (lockstep) {
        vi_ev = {(int32_t)ctx->r4, (uint32_t)ctx->r5};
        vi_retrace_count = vi_remaining = (uint32_t)ctx->r6 ? (uint32_t)ctx->r6 : 1;
        ctx->r4 = 0;  // the runtime's VI thread sends nothing
    }
    __real_osViSetEvent_recomp(rdram, ctx);
}

void __wrap_osSetEventMesg_recomp(uint8_t* rdram, recomp_context* ctx) {
    if (lockstep) {
        Event e{(int32_t)ctx->r5, (uint32_t)ctx->r6};
        switch ((uint32_t)ctx->r4) {
            case OS_EVENT_AI: ai_ev = e; ctx->r5 = 0; break;
            case OS_EVENT_SP: sp_ev = e; break;
            case OS_EVENT_DP: dp_ev = e; break;
            default: break;
        }
        if (mirror && ((uint32_t)ctx->r4 == OS_EVENT_SP || (uint32_t)ctx->r4 == OS_EVENT_DP)) {
            if (!dummy_ready) dummy_reset(rdram);
            ctx->r5 = kDummyMq;   // the renderer thread's completions go nowhere the game looks
        }
    }
    __real_osSetEventMesg_recomp(rdram, ctx);
}

void __wrap_osSpTaskStartGo_recomp(uint8_t* rdram, recomp_context* ctx) {
    if (!lockstep) {
        __real_osSpTaskStartGo_recomp(rdram, ctx);
        return;
    }
    const OSTask* task = TO_PTR(OSTask, (int32_t)ctx->r4);
    if (task->t.type == M_GFXTASK) {
        rush::trace_gfx_task(task);
        if (mirror) {   // the renderer draws it; wait until it has read the display list
            uint64_t n;
            {
                std::lock_guard<std::mutex> lock(mirror_mu);
                n = ++mirror_sent;
            }
            __real_osSpTaskStartGo_recomp(rdram, ctx);
            std::unique_lock<std::mutex> lock(mirror_mu);
            if (!mirror_cv.wait_for(lock, std::chrono::seconds(10), [n] { return mirror_done >= n; })) {
                fprintf(stderr, "[rush] lockstep: renderer did not take gfx task %llu within 10 s\n",
                        (unsigned long long)n);
                mirror_done = n;
            }
        }
        send(rdram, sp_ev);
        send(rdram, dp_ev);
    } else {
        rush::trace_rsp_task(task->t.type);
        send(rdram, sp_ev);
    }
}

void __wrap_osViSwapBuffer_recomp(uint8_t* rdram, recomp_context* ctx) {
    if (lockstep) fb_next = (uint32_t)ctx->r4;
    __real_osViSwapBuffer_recomp(rdram, ctx);
}

void __wrap_osViGetCurrentFramebuffer_recomp(uint8_t* rdram, recomp_context* ctx) {
    if (!lockstep) { __real_osViGetCurrentFramebuffer_recomp(rdram, ctx); return; }
    ctx->r2 = (gpr)(int32_t)fb_cur;
}

void __wrap_osViGetNextFramebuffer_recomp(uint8_t* rdram, recomp_context* ctx) {
    if (!lockstep) { __real_osViGetNextFramebuffer_recomp(rdram, ctx); return; }
    ctx->r2 = (gpr)(int32_t)fb_next;
}
} // extern "C"

// The idle thread's turn: every other game thread is blocked. Deliver one
// virtual VI (the woken threads run inside osSendMesg's check_running_queue
// and the call returns when all of them block again), then repeat.
void rush::lockstep_idle(uint8_t* rdram) {
    if (!lockstep) {
        pause_self(rdram);
        return;
    }
    pace_t0 = std::chrono::steady_clock::now();
    for (;;) {
        dequeue_external_messages(rdram);
        retry_pending(rdram);
        if (mirror) {
            if (dummy_ready) dummy_reset(rdram);   // drop the renderer's SP/DP completions
            std::this_thread::sleep_until(pace_t0 + std::chrono::microseconds(
                (int64_t)(virtual_vis.load() + 1) * 1000000 / 60));
        }
        virtual_vis++;
        fb_cur = fb_next;
        Event ai = ai_ev;
        if (--vi_remaining == 0) {
            vi_remaining = vi_retrace_count;
            send(rdram, vi_ev);
        }
        send(rdram, ai);
        ultramodern::check_running_queue(rdram);
    }
}
