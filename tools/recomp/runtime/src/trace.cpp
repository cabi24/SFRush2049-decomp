// Boot instrumentation for the headless host: counters, a once-per-second
// status line, first-occurrence events, framebuffer statistics (and optional
// PPM dumps), scripted controller input, and the run timer.
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "recomp.h"
#include "rush_runtime.h"

namespace {

using clock_type = std::chrono::steady_clock;
clock_type::time_point t0 = clock_type::now();

std::atomic<uint64_t> n_vi{0}, n_gfx{0}, n_audio_task{0}, n_other_task{0}, n_swap{0},
    n_audio_samples{0}, n_overlay{0}, n_audio_nonzero{0};
std::atomic<int> audio_peak{0};
std::atomic<uint32_t> last_origin{0}, last_dl{0}, last_dl_size{0}, max_dl_size{0};
std::atomic<uint64_t> fb_nonzero{0}, fb_hash{0};
std::atomic<uint64_t> fb_distinct{0};
std::mutex mu;
std::set<std::string> seen_shims;
std::set<uint64_t> fb_hashes;
std::string fb_dump_dir;
uint32_t fb_dump_every = 30;   // --fb-every N: sample every N screen updates
std::string current_image = "none";

struct Press { double t, dur; uint16_t mask; float x, y; int port; };  // port 0-3
int max_port = 0;
std::vector<Press> presses;

double now_s() { return std::chrono::duration<double>(clock_type::now() - t0).count(); }

// Scripted input clock: wall time (Phase 1 default) or game frames / 30
// (--clock frames: deterministic replay for the frame oracle).
bool frame_clock = false;
rush::live_input_fn live_input = nullptr;
rush::live_connected_fn live_connected = nullptr;
double input_time() { return frame_clock ? rush::oracle_frames() / 30.0 : now_s(); }

uint16_t button_mask(const std::string& b, float& x, float& y) {
    static const struct { const char* n; uint16_t m; } table[] = {
        {"A", 0x8000}, {"B", 0x4000}, {"Z", 0x2000}, {"START", 0x1000},
        {"UP", 0x0800}, {"DOWN", 0x0400}, {"LEFT", 0x0200}, {"RIGHT", 0x0100},
        {"L", 0x0020}, {"R", 0x0010}, {"CU", 0x0008}, {"CD", 0x0004}, {"CL", 0x0002}, {"CR", 0x0001}};
    for (auto& e : table) if (b == e.n) return e.m;
    if (b == "STICKUP") { y = 1.0f; return 0; }
    if (b == "STICKDOWN") { y = -1.0f; return 0; }
    if (b == "STICKLEFT") { x = -1.0f; return 0; }
    if (b == "STICKRIGHT") { x = 1.0f; return 0; }
    fprintf(stderr, "[rush] unknown button %s\n", b.c_str());
    std::exit(2);
}

void parse_presses(const std::string& spec) {
    std::stringstream ss(spec);
    std::string item;
    while (std::getline(ss, item, ',')) {
        if (item.empty()) continue;
        // T:BUTTON[@PORT][:DUR]  (PORT 1-4, default 1; tools/recomp/scripts/*.press)
        Press p{0, 0.2, 0, 0, 0, 0};
        std::stringstream is(item);
        std::string t, b, d;
        std::getline(is, t, ':');
        std::getline(is, b, ':');
        if (std::getline(is, d, ':')) p.dur = std::stod(d);
        p.t = std::stod(t);
        auto at = b.find('@');
        if (at != std::string::npos) {
            p.port = std::stoi(b.substr(at + 1)) - 1;
            b = b.substr(0, at);
            if (p.port < 0 || p.port > 3) {
                fprintf(stderr, "[rush] controller port must be 1-4 in %s\n", item.c_str());
                std::exit(2);
            }
        }
        max_port = std::max(max_port, p.port);
        p.mask = button_mask(b, p.x, p.y);
        presses.push_back(p);
    }
}

void status_line(const char* tag) {
    fprintf(stderr,
        "[rush] %s t=%.1fs vi=%llu gfx_tasks=%llu audio_tasks=%llu other_tasks=%llu swaps=%llu "
        "origin=0x%08X fb_nonzero=%llu fb_distinct=%llu last_dl=0x%08X(%u B) audio_samples=%llu audio_nonzero=%llu audio_peak=%d image=%s spin_yields=%llu pak_writes=%llu max_burst_ms=%.1f\n",
        tag, now_s(), (unsigned long long)n_vi, (unsigned long long)n_gfx,
        (unsigned long long)n_audio_task, (unsigned long long)n_other_task,
        (unsigned long long)n_swap, last_origin.load(), (unsigned long long)fb_nonzero,
        (unsigned long long)fb_distinct, last_dl.load(), last_dl_size.load(),
        (unsigned long long)n_audio_samples, (unsigned long long)n_audio_nonzero, audio_peak.load(),
        current_image.c_str(),
        (unsigned long long)rush::spin_yield_count(), (unsigned long long)rush::pak_writes(),
        rush::max_burst_ms());
    if (rush::lockstep_enabled() && std::strcmp(tag, "final") == 0)   // virtual time (lockstep)
        fprintf(stderr, "[rush] lockstep: %llu virtual VIs (%.2f s), %llu audio frames = %.1f%% of %u Hz\n",
                (unsigned long long)rush::lockstep_vis(), rush::lockstep_vis() / 60.0,
                (unsigned long long)(n_audio_samples / 2),
                rush::lockstep_vis() ? 100.0 * (n_audio_samples / 2) / (rush::lockstep_vis() / 60.0 * 22050) : 0.0,
                22050u);
}

} // namespace

void rush::trace_event(const char* fmt, ...) {
    char buf[512];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    fprintf(stderr, "[rush] t=%.2fs %s\n", now_s(), buf);
}

void rush::trace_start(double seconds, const std::string& press, const std::string& fb_dump) {
    parse_presses(press);
    fb_dump_dir = fb_dump;
    if (!fb_dump_dir.empty()) std::filesystem::create_directories(fb_dump_dir);
    std::thread([seconds] {
        while (seconds <= 0 || now_s() < seconds) {   // --seconds 0: until the window closes
            std::this_thread::sleep_for(std::chrono::seconds(1));
            status_line("status");
        }
        status_line("final");
        rush::oracle_finish("seconds");
        rush::run_exit_hook();
        fflush(stderr);
        std::_Exit(0);
    }).detach();
}

void rush::trace_rsp_task(uint32_t type) {
    if (type == M_AUDTASK) {
        if (n_audio_task++ == 0) trace_event("first audio RSP task");
    } else {
        if (n_other_task++ == 0) trace_event("first non-gfx/non-audio RSP task type %u", type);
    }
}

void rush::trace_gfx_task(const OSTask* task) {
    last_dl = (uint32_t)task->t.data_ptr;
    last_dl_size = task->t.data_size;
    if (task->t.data_size > max_dl_size) {
        max_dl_size = task->t.data_size;
        trace_event("largest display list so far: %u B at 0x%08X (gfx task %llu)", task->t.data_size,
                    (uint32_t)task->t.data_ptr, (unsigned long long)n_gfx.load());
    }
    if (n_gfx++ == 0) {
        trace_event("first gfx task: ucode 0x%08X data_ptr 0x%08X size %u", (uint32_t)task->t.ucode,
                    (uint32_t)task->t.data_ptr, task->t.data_size);
    }
}

void rush::trace_screen_update(uint8_t* rdram, uint32_t origin) {
    uint64_t n = n_swap++;
    last_origin = origin;
    if (n == 0) trace_event("first VI screen update, origin 0x%08X", origin);
    if (origin == 0 || (n % fb_dump_every) != 0) return;
    // Framebuffer statistics: 320x240 16-bit assumed (VI width register checked).
    auto* vi = ultramodern::renderer::get_vi_regs();
    uint32_t width = vi->VI_WIDTH_REG ? vi->VI_WIDTH_REG : 320;
    uint32_t bpp = (vi->VI_STATUS_REG & 3) == 3 ? 4 : 2;
    uint32_t base = 0x80000000u | (origin & 0x00FFFFFFu);
    uint32_t bytes = width * 240 * bpp;
    if ((base & 0x00FFFFFFu) + bytes > 0x800000u) return;
    uint64_t nz = 0, h = 1469598103934665603ull;
    for (uint32_t i = 0; i < bytes; i += 4) {
        uint32_t w = MEM_W(i, (gpr)(int32_t)base);
        nz += w != 0;
        h = (h ^ w) * 1099511628211ull;
    }
    fb_nonzero = nz;
    fb_hash = h;
    {
        std::lock_guard<std::mutex> lock(mu);
        if (fb_hashes.insert(h).second) {
            fb_distinct = fb_hashes.size();
            if (nz && !fb_dump_dir.empty() && bpp == 2) {
                char path[512];
                snprintf(path, sizeof path, "%s/fb_%05llu.ppm", fb_dump_dir.c_str(), (unsigned long long)n);
                if (FILE* f = fopen(path, "wb")) {
                    fprintf(f, "P6\n%u 240\n255\n", width);
                    for (uint32_t i = 0; i < width * 240; i++) {
                        uint16_t p = MEM_HU(i * 2, (gpr)(int32_t)base);
                        unsigned char rgb[3] = {(unsigned char)(((p >> 11) & 31) << 3),
                                                (unsigned char)(((p >> 6) & 31) << 3),
                                                (unsigned char)(((p >> 1) & 31) << 3)};
                        fwrite(rgb, 1, 3, f);
                    }
                    fclose(f);
                }
            }
        }
    }
}

void rush::trace_audio_samples(const int16_t* samples, size_t n) {
    uint64_t nz = 0;
    int peak = 0;
    for (size_t i = 0; i < n; i++) {
        int v = samples[i] < 0 ? -(int)samples[i] : samples[i];
        nz += v != 0;
        if (v > peak) peak = v;
    }
    if (nz && n_audio_nonzero == 0) trace_event("first non-silent audio buffer (%llu of %zu samples non-zero, peak %d)",
                                                (unsigned long long)nz, n, peak);
    n_audio_samples += n;
    n_audio_nonzero += nz;
    if (peak > audio_peak) audio_peak = peak;
}

void rush::trace_overlay(const char* name, uint32_t stream_rom, uint32_t dst) {
    {
        std::lock_guard<std::mutex> lock(mu);
        if (std::string(name) != "game") current_image = name;
    }
    n_overlay++;
    {
        char ev[96];
        snprintf(ev, sizeof ev, "load %s", name);
        rush::oracle_event(ev);
    }
    trace_event("load %s: stream ROM 0x%06X -> 0x%08X (load #%llu)", name, stream_rom, dst,
                (unsigned long long)n_overlay.load());
}

void rush::trace_shim(const char* name, uint32_t ra) {
    std::lock_guard<std::mutex> lock(mu);
    if (seen_shims.insert(name).second) {
        (void)ra;  // N64Recomp does not materialise $ra for direct calls
        fprintf(stderr, "[rush] t=%.2fs shim %s first called\n", now_s(), name);
    }
}

void rush::on_vi() {
    if (n_vi++ == 0) trace_event("first VI");
}

void rush::poll_input() {}

void rush::set_live_input(live_input_fn input, live_connected_fn connected) {
    live_input = input;
    live_connected = connected;
}

void rush::request_quit(const char* why) {
    status_line("final");
    rush::oracle_finish(why);
    rush::capture_finish();
    rush::run_exit_hook();
    fflush(stderr);
    std::_Exit(0);
}

namespace { std::atomic<void (*)()> exit_hook{nullptr}; }
void rush::set_exit_hook(void (*fn)()) { exit_hook = fn; }
void rush::run_exit_hook() {
    if (auto fn = exit_hook.exchange(nullptr)) fn();
}

bool rush::get_input(int controller, uint16_t* buttons, float* x, float* y) {
    if (live_input && presses.empty()) return live_input(controller, buttons, x, y);
    if (controller < 0 || controller > max_port) return false;
    double t = input_time();
    uint16_t m = 0;
    float sx = 0, sy = 0;
    for (const Press& p : presses) {
        if (p.port == controller && t >= p.t && t < p.t + p.dur) {
            m |= p.mask;
            if (p.x) sx = p.x;
            if (p.y) sy = p.y;
        }
    }
    *buttons = m;
    *x = sx;
    *y = sy;
    return true;
}

void rush::set_rumble(int, bool) {}

void rush::set_frame_clock(bool on) { frame_clock = on; }
void rush::trace_final() { status_line("final"); }
void rush::set_fb_dump_every(uint32_t n) { fb_dump_every = n ? n : 1; }

int rush::scripted_controllers() { return max_port + 1; }

ultramodern::input::connected_device_info_t rush::get_connected_device_info(int controller) {
    // A controller pak (pak.cpp) is reported as a pak that is not a rumble pak:
    // osContGetQuery status bit CONT_CARD_ON, osMotorInit -> PFS_ERR_DEVICE (as a real
    // controller pak answers). ultramodern's Pak enum has no ControllerPak entry yet.
    const auto pak = rush::pak_present(controller) ? static_cast<ultramodern::input::Pak>(2)
                                                   : ultramodern::input::Pak::None;
    if (live_connected && presses.empty()) {
        if (live_connected(controller))
            return {ultramodern::input::Device::Controller, pak};
        return {ultramodern::input::Device::None, ultramodern::input::Pak::None};
    }
    if (controller >= 0 && controller <= max_port) {
        return {ultramodern::input::Device::Controller, pak};
    }
    return {ultramodern::input::Device::None, ultramodern::input::Pak::None};
}
