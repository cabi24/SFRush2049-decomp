// Host fault report (Phase 9, lane w29r): when the recompiled game (usually a
// draft_check replacement) makes the host fault, say WHERE instead of dying
// silently with -11.
//
// A replacement body runs with exact N64 memory semantics inside the host:
// an N64 load/store becomes `rdram + (addr - 0x80000000)`. A wild N64 pointer
// (a draft reading the wrong struct field and dereferencing what it found,
// 0x01010000 in w28b's func_800C813C draft) lands outside the host mapping and
// the host gets SIGSEGV; on the console it would be a TLB exception, so it is
// a divergence of the draft, not of the harness. The handler prints, on
// stderr, lines starting `[rush] FAULT` that tools/recomp/oracle.py parses
// (fault_report):
//
//   [rush] FAULT signal=11 access=read n64=0x01010000 host=0x... frame=F thread=game7
//   [rush] FAULT active=<id=mode:function> fn=<index> seq=<call-log seq> depth=<open replaced calls on the thread>
//   [rush] FAULT pc=0x... bt=0x...,0x...  exe_base=0x...
//   [rush] FAULT regs at=... v0=... ... ra=...   (the thread's recomp context: values may lag the host registers)
//
// oracle.py symbolises pc/bt with addr2line against the host binary and maps
// each recompiled-C line back to the N64 instruction N64Recomp wrote above it
// (`// 0x8080013C: lw $v0, 0x0($s2)`). The call log and oracle log are flushed
// so the calls before the fault can still be compared. Then the default action
// runs (exit status -signal, as before).
//
// "Active" = the innermost replaced/probed function whose generated hook is
// open on the faulting thread (rush_cap_enter/rush_cap_exit call
// rush_fault_enter/rush_fault_exit, capture or not). POSIX only.
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "recomp.h"
#include "rush_runtime.h"
#include "rush2049_repl.h"

#if !defined(_WIN32)
#include <execinfo.h>
#include <pthread.h>
#include <signal.h>
#include <ucontext.h>
#include <unistd.h>

#include <atomic>

extern "C" char __executable_start;

namespace {

constexpr int kMaxActive = 64;
std::atomic<uint8_t*> g_rdram{nullptr};

struct Active {
    int fn;
    uint64_t seq;
    recomp_context* ctx;
};
thread_local Active tl_active[kMaxActive];
thread_local int tl_n = 0;

void put(const char* s) {
    size_t n = strlen(s);
    while (n) {
        ssize_t w = write(2, s, n);
        if (w <= 0) return;
        s += w;
        n -= (size_t)w;
    }
}

const char* signame(int sig) {
    switch (sig) {
        case SIGSEGV: return "SIGSEGV";
        case SIGBUS: return "SIGBUS";
        case SIGFPE: return "SIGFPE";
        case SIGILL: return "SIGILL";
        case SIGABRT: return "SIGABRT";
        default: return "signal";
    }
}

std::atomic<int> reporting{0};

void on_fault(int sig, siginfo_t* si, void* uctx) {
    // Restore the default action first: whatever happens below, the next
    // fault (or the re-raise) terminates with the original status.
    signal(sig, SIG_DFL);
    if (reporting.fetch_add(1) == 0) {
        char buf[1024];
        uintptr_t a = (uintptr_t)si->si_addr;
        uint8_t* base = g_rdram.load();
        const char* access = "?";
        uintptr_t pc = 0;
#if defined(__x86_64__) && defined(__linux__)
        ucontext_t* uc = (ucontext_t*)uctx;
        pc = (uintptr_t)uc->uc_mcontext.gregs[REG_RIP];
        if (sig == SIGSEGV) access = (uc->uc_mcontext.gregs[REG_ERR] & 2) ? "write" : "read";
#elif defined(__aarch64__) && defined(__linux__)
        ucontext_t* uc = (ucontext_t*)uctx;
        pc = (uintptr_t)uc->uc_mcontext.pc;
#else
        (void)uctx;
#endif
        char n64[24] = "-";
        if (base && (sig == SIGSEGV || sig == SIGBUS) && a >= (uintptr_t)base &&
            a - (uintptr_t)base <= 0xFFFFFFFFull)
            snprintf(n64, sizeof n64, "0x%08X", (uint32_t)(a - (uintptr_t)base + 0x80000000u));
        char tname[32] = "?";
        pthread_getname_np(pthread_self(), tname, sizeof tname);
        snprintf(buf, sizeof buf, "[rush] FAULT signal=%d (%s) access=%s n64=%s host=0x%llx frame=%llu thread=%s\n",
                 sig, signame(sig), access, n64, (unsigned long long)a,
                 (unsigned long long)rush::oracle_frames(), tname);
        put(buf);
        int top = tl_n > kMaxActive ? kMaxActive : tl_n;
        if (top > 0) {
            const Active& c = tl_active[top - 1];
            const char* nm = (c.fn >= 0 && c.fn < RUSH_REPL_COUNT) ? rush_repl_names[c.fn] : "?";
            snprintf(buf, sizeof buf, "[rush] FAULT active=%s fn=%d seq=%llu depth=%d\n", nm, c.fn,
                     (unsigned long long)c.seq, tl_n);
        } else {
            snprintf(buf, sizeof buf, "[rush] FAULT active=- fn=-1 seq=0 depth=0\n");
        }
        put(buf);
        void* bt[24];
        int n = backtrace(bt, 24);
        int len = snprintf(buf, sizeof buf, "[rush] FAULT pc=0x%llx exe_base=0x%llx bt=", (unsigned long long)pc,
                           (unsigned long long)(uintptr_t)&__executable_start);
        for (int i = 0; i < n && len < (int)sizeof buf - 24; i++)
            len += snprintf(buf + len, sizeof buf - len, "%s0x%llx", i ? "," : "",
                            (unsigned long long)(uintptr_t)bt[i]);
        snprintf(buf + len, sizeof buf - len, "\n");
        put(buf);
        if (top > 0 && tl_active[top - 1].ctx) {
            static const char* const names[32] = {
                "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
            const gpr* r = &tl_active[top - 1].ctx->r0;
            len = snprintf(buf, sizeof buf, "[rush] FAULT regs");
            for (int i = 1; i < 32 && len < (int)sizeof buf - 24; i++) {
                if (i == 26 || i == 27) continue;
                len += snprintf(buf + len, sizeof buf - len, " %s=%08X", names[i], (uint32_t)r[i]);
            }
            snprintf(buf + len, sizeof buf - len, "\n");
            put(buf);
        }
        // Keep what the logs have so far (the calls before the fault compare).
        rush::capture_fault_flush();
        rush::oracle_fault_flush();
    }
    if (sig == SIGABRT) raise(sig);
    // SIGSEGV/SIGBUS/SIGFPE/SIGILL: returning re-executes the faulting
    // instruction under the default action.
}

} // namespace

void rush::fault_setup() {
    void* warm[2];
    backtrace(warm, 2);   // loads the unwinder now, not inside the handler
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_fault;
    sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigemptyset(&sa.sa_mask);
    for (int sig : {SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT}) sigaction(sig, &sa, nullptr);
}

void rush::fault_note_rdram(uint8_t* rdram) { g_rdram.store(rdram); }

extern "C" void rush_fault_enter(uint8_t* rdram, recomp_context* ctx, int fn, uint64_t seq) {
    if (g_rdram.load() == nullptr) g_rdram.store(rdram);
    if (tl_n < kMaxActive) tl_active[tl_n] = Active{fn, seq, ctx};
    tl_n++;
}

extern "C" void rush_fault_exit(void) {
    if (tl_n > 0) tl_n--;
}

#else  // _WIN32: no report (the host dies as before)

void rush::fault_setup() {}
void rush::fault_note_rdram(uint8_t*) {}
extern "C" void rush_fault_enter(uint8_t*, recomp_context*, int, uint64_t) {}
extern "C" void rush_fault_exit(void) {}

#endif
