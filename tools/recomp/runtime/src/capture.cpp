// Per-call capture (Phase 3): what each call of a replaced function did, in
// both builds, so tools/recomp/oracle.py can name the FIRST differing call
// instead of the first differing frame.
//
// The generated entry hooks (gen_config.replacement_hooks) call
// rush_cap_enter before and rush_cap_exit after the body: in the replaced
// build around the replacement, in the stock capture build
// (gen_config --capture-set) around the recompiled original, which the hook
// re-enters with a one-shot bypass (rush_cap_bypass). With --call-log PATH
// the host writes
//
//   c <seq> <frame> <fn> <arg>,<arg>,...     entry: the argument registers /
//                                            stack words the prototype uses
//   C <seq> <frame> <fn> r=<ret> w=<n>:<xxh64>   exit: return register(s) and
//                                            the call's EFFECT on rdram
//   W <seq> <addr> <old> <new>               (--call-detail SEQ) every word of
//                                            that call's effect
//
// seq = global entry order (1-based), fn = replaced-function index (header
// "# fn" lines name them).
//
// Effect = every N64 word of the 8 MiB whose value differs between the
// call's entry and exit, found by WRITE TRACKING BY PAGE: at entry the 8 MiB
// of rdram is mprotect'ed read-only; the first write to a 4 KiB page faults,
// the SIGSEGV handler copies the page's pre-write contents for every open
// window and makes the page writable again. At exit each copied page is
// compared word by word with its current contents. Host pages are 4 KiB and
// the runtime keeps rdram word-swapped within each word, so an N64 page is a
// host page and a host 32-bit word is the N64 word at the same offset.
// Excluded from the effect: the calling thread's stack below the sp at entry
// (the callee's own dead frame), other threads' stacks, and the OSThread
// host-pointer words (oracle.cpp). Writes to the replacement image above
// 8 MiB are not tracked (static binding puts matched bodies' statics at
// their retail addresses; replace.py). Writes by other threads while a
// window is open (a body that blocks in osRecvMesg) count for that window;
// both builds behave the same way, so the logs stay comparable.
//
// Portability (Phase 4): write tracking uses POSIX mprotect/SIGSEGV. The CMake
// option RUSH_CAPTURE (default ON except on Windows) selects it; without it the
// hooks stay transparent (the stock capture build still re-enters the
// original) and --call-log is refused.
#ifndef RUSH_CAPTURE
#define RUSH_CAPTURE 1
#endif
#if RUSH_CAPTURE
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>

#include "recomp.h"
#include "xxHash/xxh3.h"
#include "rush_runtime.h"
#include "rush2049_repl.h"

namespace {

constexpr uint32_t kRamBytes = 8u << 20;
constexpr uint32_t kPage = 4096;
constexpr uint32_t kPages = kRamBytes / kPage;
constexpr int kMaxWindows = 32;
constexpr uint32_t kPoolPages = 4096;  // 16 MiB of pre-write page copies

bool cap_on = false;
FILE* cap_file = nullptr;
uint64_t detail_seq = 0;
std::mutex cap_mu;                   // log + window bookkeeping (not the signal handler)
uint64_t seq_counter = 0;

uint8_t* ram = nullptr;              // rdram host base (first enter)
uint8_t* pool = nullptr;
std::atomic<uint32_t> pool_used{0};
std::atomic_flag handler_lock = ATOMIC_FLAG_INIT;

struct Window {
    bool open = false;
    bool overflow = false;
    int fn = 0;
    uint64_t seq = 0;
    uint64_t frame = 0;
    uint32_t sp = 0;
    uint32_t floor = 0;              // own-stack words below this are dead frames
    uint32_t npages = 0;
    int32_t slot[kPages];            // pool slot of the pre-window copy, -1 = none
    uint16_t pages[kPages];
};
Window* windows = nullptr;           // kMaxWindows, allocated when capture starts
std::atomic<int> open_windows{0};

thread_local int bypass_fn = -1;
thread_local int tl_stack[kMaxWindows];
thread_local int tl_depth = 0;

struct sigaction old_segv;

void spin_lock() { while (handler_lock.test_and_set(std::memory_order_acquire)) {} }
void spin_unlock() { handler_lock.clear(std::memory_order_release); }

void on_segv(int sig, siginfo_t* si, void* uctx) {
    uintptr_t a = (uintptr_t)si->si_addr;
    if (ram == nullptr || a < (uintptr_t)ram || a >= (uintptr_t)ram + kRamBytes) {
        // not ours: behave as before the handler was installed
        sigaction(SIGSEGV, &old_segv, nullptr);
        (void)sig; (void)uctx;
        return;  // the access faults again under the previous disposition
    }
    uint32_t page = (uint32_t)((a - (uintptr_t)ram) / kPage);
    spin_lock();
    int32_t slot = -1;
    for (int w = 0; w < kMaxWindows; w++) {
        Window& win = windows[w];
        if (!win.open || win.slot[page] >= 0) continue;
        if (slot < 0) {
            uint32_t s = pool_used.fetch_add(1);
            if (s >= kPoolPages) { win.overflow = true; continue; }
            slot = (int32_t)s;
            memcpy(pool + (size_t)slot * kPage, ram + (size_t)page * kPage, kPage);
        }
        win.slot[page] = slot;
        win.pages[win.npages++] = (uint16_t)page;
    }
    mprotect(ram + (size_t)page * kPage, kPage, PROT_READ | PROT_WRITE);
    spin_unlock();
}

void install_handler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_segv;
    sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, &old_segv);
}

void cap_log(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
void cap_log(const char* fmt, ...) {
    if (!cap_file) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(cap_file, fmt, ap);
    va_end(ap);
}

uint32_t stack_word(uint8_t* rdram, recomp_context* ctx, int off) {
    return (uint32_t)MEM_W(off, ctx->r29);
}

gpr reg(recomp_context* ctx, int n) {
    gpr* r = &ctx->r0;
    return r[n];
}

fpr freg(recomp_context* ctx, int n) {
    fpr* f = &ctx->f0;
    return f[n];
}

std::string format_args(uint8_t* rdram, recomp_context* ctx, int fn) {
    std::string s;
    char buf[32];
    int n = rush_cap_locs[fn][0];
    for (int i = 0; i < n; i++) {
        int code = rush_cap_locs[fn][i + 1];
        if (i) s += ",";
        if (code >= 2000) {
            snprintf(buf, sizeof buf, "%08X:%08X", stack_word(rdram, ctx, code - 2000),
                     stack_word(rdram, ctx, code - 2000 + 4));
        } else if (code >= 1000) {
            snprintf(buf, sizeof buf, "%08X", stack_word(rdram, ctx, code - 1000));
        } else if (code >= 300) {
            snprintf(buf, sizeof buf, "%08X:%08X", (uint32_t)reg(ctx, code - 300), (uint32_t)reg(ctx, code - 299));
        } else if (code >= 200) {
            fpr f = freg(ctx, code - 200);
            fpr g = freg(ctx, code - 199);
            snprintf(buf, sizeof buf, "%08X:%08X:%08X", f.u32l, f.u32h, g.u32l);
        } else if (code >= 100) {
            snprintf(buf, sizeof buf, "%08X", freg(ctx, code - 100).u32l);
        } else {
            snprintf(buf, sizeof buf, "%08X", (uint32_t)reg(ctx, code));
        }
        s += buf;
    }
    return s.empty() ? "-" : s;
}

std::string format_ret(recomp_context* ctx, int fn) {
    char buf[48];
    switch (rush_cap_ret[fn]) {
        case 0: return "-";
        case 2: snprintf(buf, sizeof buf, "%08X", (uint32_t)ctx->r2); break;
        case 3: snprintf(buf, sizeof buf, "%08X:%08X", (uint32_t)ctx->r2, (uint32_t)ctx->r3); break;
        case 100: snprintf(buf, sizeof buf, "%08X", ctx->f0.u32l); break;
        case 200: snprintf(buf, sizeof buf, "%08X:%08X:%08X", ctx->f0.u32l, ctx->f0.u32h, ctx->f1.u32l); break;
        default: return "?";
    }
    return buf;
}

// Is the N64 address (offset into rdram) left out of a call's effect?
// sp_off = sp at the window's entry, floor = sp at entry of the outermost
// replaced (non-probe) window open on this thread: below it lie the frames
// of the replaced bodies, whose layout differs whenever their code does.
bool excluded(uint32_t off, uint32_t sp_off, uint32_t floor) {
    for (const auto& r : rush::oracle_stack_ranges()) {
        if (off < r.first || off >= r.second) continue;
        if (sp_off >= r.first && sp_off <= r.second) return off < floor;   // own stack: dead frames
        return true;                                                       // another thread's stack
    }
    return rush::oracle_word_masked(off);
}

} // namespace

void rush::capture_start(const std::string& path, uint64_t detail) {
    if (path.empty()) return;
    if (sysconf(_SC_PAGESIZE) != (long)kPage) {
        fprintf(stderr, "[rush] --call-log needs 4 KiB host pages (have %ld)\n", sysconf(_SC_PAGESIZE));
        std::exit(2);
    }
    cap_file = fopen(path.c_str(), "w");
    if (!cap_file) {
        fprintf(stderr, "[rush] cannot write call log %s\n", path.c_str());
        std::exit(2);
    }
    detail_seq = detail;
    windows = new Window[kMaxWindows];
    for (int w = 0; w < kMaxWindows; w++) std::fill(windows[w].slot, windows[w].slot + kPages, -1);
    pool = static_cast<uint8_t*>(mmap(nullptr, (size_t)kPoolPages * kPage, PROT_READ | PROT_WRITE,
                                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (pool == MAP_FAILED) { fprintf(stderr, "[rush] capture pool mmap failed\n"); std::exit(2); }
    install_handler();
    cap_on = true;
    cap_log("# rush2049 call log v2: effect = words changed between entry and exit (page write tracking; replaced frames floor)\n");
    for (int i = 0; i < RUSH_REPL_COUNT; i++)
        cap_log("# fn %d %s %s probe=%d\n", i, rush_repl_names[i], rush_cap_labels[i], rush_cap_probe[i]);
}

void rush::capture_finish() {
    if (!cap_file) return;
    std::lock_guard<std::mutex> lock(cap_mu);
    cap_log("END %llu calls\n", (unsigned long long)seq_counter);
    fflush(cap_file);
}

// Stock capture hook: the hook re-enters the recompiled function with the
// bypass armed for that function; the inner entry consumes it and runs the body.
extern "C" int rush_cap_bypass(int fn) {
    if (bypass_fn == fn) { bypass_fn = -1; return 1; }
    return 0;
}

extern "C" void rush_cap_enter(uint8_t* rdram, recomp_context* ctx, int fn) {
    if (!cap_on) {
        bypass_fn = fn;  // stock capture build without --call-log: still transparent
        return;
    }
    std::lock_guard<std::mutex> lock(cap_mu);
    if (ram == nullptr) ram = rdram + 0;  // rdram host base = N64 0x80000000
    uint64_t seq = ++seq_counter;
    uint64_t frame = rush::oracle_frames();
    std::string args = format_args(rdram, ctx, fn);
    cap_log("c %llu %llu %d %s\n", (unsigned long long)seq, (unsigned long long)frame, fn, args.c_str());
    int w = 0;
    spin_lock();
    while (w < kMaxWindows && windows[w].open) w++;
    if (w == kMaxWindows || tl_depth == kMaxWindows) {
        spin_unlock();
        fprintf(stderr, "[rush] capture: more than %d nested windows\n", kMaxWindows);
        std::_Exit(3);
    }
    Window& win = windows[w];
    win.open = true;
    win.overflow = false;
    win.fn = fn;
    win.seq = seq;
    win.frame = frame;
    win.sp = (uint32_t)ctx->r29 & 0x00FFFFFFu;
    win.floor = win.sp;
    for (int i = 0; i < tl_depth; i++) {            // outermost replaced window on this thread
        const Window& o = windows[tl_stack[i]];
        if (!rush_cap_probe[o.fn]) { win.floor = std::max(win.floor, o.sp); break; }
    }
    win.npages = 0;
    open_windows++;
    spin_unlock();
    tl_stack[tl_depth++] = w;
    mprotect(ram, kRamBytes, PROT_READ);  // (re-)arm: every page faults once more
    bypass_fn = fn;
}

extern "C" void rush_cap_exit(uint8_t* rdram, recomp_context* ctx, int fn) {
    if (!cap_on) return;
    std::lock_guard<std::mutex> lock(cap_mu);
    if (tl_depth == 0) { fprintf(stderr, "[rush] capture: exit without entry\n"); std::_Exit(3); }
    int w = tl_stack[--tl_depth];
    spin_lock();
    Window& win = windows[w];
    win.open = false;
    spin_unlock();
    uint64_t h = 0x9E3779B97F4A7C15ull;
    uint32_t n = 0;
    std::vector<uint16_t> pages(win.pages, win.pages + win.npages);
    std::sort(pages.begin(), pages.end());
    for (uint16_t p : pages) {
        const uint32_t* before = reinterpret_cast<const uint32_t*>(pool + (size_t)win.slot[p] * kPage);
        const uint32_t* after = reinterpret_cast<const uint32_t*>(ram + (size_t)p * kPage);
        for (uint32_t i = 0; i < kPage / 4; i++) {
            if (before[i] == after[i]) continue;
            uint32_t off = p * kPage + i * 4;
            if (excluded(off, win.sp, win.floor)) continue;
            uint32_t rec[2] = {off, after[i]};
            h = XXH3_64bits_withSeed(rec, sizeof rec, h);
            n++;
            if (detail_seq == win.seq)
                cap_log("W %llu %08X %08X %08X\n", (unsigned long long)win.seq, 0x80000000u + off, before[i],
                        after[i]);
        }
        win.slot[p] = -1;
    }
    win.npages = 0;
    std::string ret = format_ret(ctx, fn);
    cap_log("C %llu %llu %d r=%s w=%u:%016llx%s\n", (unsigned long long)win.seq,
            (unsigned long long)rush::oracle_frames(), fn, ret.c_str(), n, (unsigned long long)h,
            win.overflow ? " overflow" : "");
    if (--open_windows == 0) {
        mprotect(ram, kRamBytes, PROT_READ | PROT_WRITE);
        pool_used = 0;
    }
    (void)rdram;
}

#else  // !RUSH_CAPTURE: transparent hooks, no call log

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "recomp.h"
#include "rush_runtime.h"

namespace { int bypass_fn = -1; }

void rush::capture_start(const std::string& path, uint64_t) {
    if (path.empty()) return;
    fprintf(stderr, "[rush] --call-log is not available in this build (configured with RUSH_CAPTURE=OFF; "
                    "per-call capture needs POSIX mprotect/SIGSEGV)\n");
    std::exit(2);
}

void rush::capture_finish() {}

extern "C" int rush_cap_bypass(int fn) {
    if (bypass_fn == fn) { bypass_fn = -1; return 1; }
    return 0;
}

extern "C" void rush_cap_enter(uint8_t*, recomp_context*, int fn) { bypass_fn = fn; }

extern "C" void rush_cap_exit(uint8_t*, recomp_context*, int) {}

#endif  // RUSH_CAPTURE
