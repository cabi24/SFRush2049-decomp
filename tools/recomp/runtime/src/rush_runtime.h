// Shared declarations of the Rush 2049 recomp host (tools/recomp/runtime).
#pragma once
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ultramodern/ultramodern.hpp"

#if defined(__GNUC__) || defined(__clang__)
#define RUSH_PRINTF(a, b) __attribute__((format(printf, a, b)))
#else
#define RUSH_PRINTF(a, b)
#endif

namespace rush {
// register_overlays.cpp
void register_overlays();

// null_renderer.cpp
std::unique_ptr<ultramodern::renderer::RendererContext> create_null_renderer(
    uint8_t* rdram, ultramodern::renderer::WindowHandle window, bool developer_mode);
void set_null_dl_delay_ms(int ms);  // --null-dl-delay: late asynchronous DP completion (test)

// rt64_renderer.cpp (RUSH_RENDERER=rt64)
std::unique_ptr<ultramodern::renderer::RendererContext> create_rt64_renderer(
    uint8_t* rdram, ultramodern::renderer::WindowHandle window, bool developer_mode);

// hooks.cpp: rush_hook_spin yields so far (wall clock only)
uint64_t spin_yield_count();
void set_spin_yield(bool on);
double max_burst_ms();   // longest game-thread run between message calls (wall clock)  // --no-spin-yield (diagnostic): the loops spin as in v1

// sdl_frontend.cpp (RUSH_RENDERER=rt64): window, events, controller/keyboard
ultramodern::gfx_callbacks_t::gfx_data_t sdl_create_gfx();
ultramodern::renderer::WindowHandle sdl_create_window(ultramodern::gfx_callbacks_t::gfx_data_t);
void sdl_update_gfx(ultramodern::gfx_callbacks_t::gfx_data_t);
bool sdl_get_input(int port, uint16_t* buttons, float* x, float* y);
bool sdl_port_connected(int port);
void sdl_set_fullscreen(bool on);
void sdl_set_keyboard_alone(bool on);  // controllers start at player 2
void sdl_set_log_input(bool on);       // --log-input: print button/stick changes
// SDL audio output (rt64 build): AI buffers -> default output device
void sdl_audio_enable(bool on, float volume);
void sdl_audio_set_frequency(uint32_t freq);
void sdl_audio_queue(const int16_t* samples, size_t count);
size_t sdl_audio_frames_remaining();
bool sdl_audio_active();

// trace.cpp: live input (a window's controllers) when no --press script is given
using live_input_fn = bool (*)(int port, uint16_t* buttons, float* x, float* y);
using live_connected_fn = bool (*)(int port);
void set_live_input(live_input_fn input, live_connected_fn connected);
// write the final status line, close the logs and exit (window closed, Escape)
[[noreturn]] void request_quit(const char* why);
void set_exit_hook(void (*fn)());   // run once before the host exits (--wav header)
void run_exit_hook();

// trace.cpp: counters, the status line, the run timer, scripted input
void trace_start(double seconds, const std::string& press, const std::string& fb_dump);
void trace_event(const char* fmt, ...) RUSH_PRINTF(1, 2);
void trace_rsp_task(uint32_t type);
void trace_gfx_task(const OSTask* task);
void trace_screen_update(uint8_t* rdram, uint32_t vi_origin);
void trace_audio_samples(const int16_t* samples, size_t n);  // counts, non-zero samples, peak
void trace_overlay(const char* name, uint32_t stream_rom, uint32_t dst);
void trace_shim(const char* name, uint32_t ra);
void on_vi();
void poll_input();
bool get_input(int controller, uint16_t* buttons, float* x, float* y);
int scripted_controllers();  // highest port the --press script uses (1-4), default 1
void set_rumble(int controller, bool on);
ultramodern::input::connected_device_info_t get_connected_device_info(int controller);
// --clock frames: scripted input reads game frames / 30 instead of wall time
void set_frame_clock(bool on);
void set_fb_dump_every(uint32_t n);
void trace_final();   // the final status line (lockstep --frames stop)   // --fb-every N (default 30 screen updates)

// pak.cpp: controller pak on controller 1 (Phase 6). on=false: no pak (the runtime's
// behaviour); persistent=false: the image is never written (lockstep: fixed image).
void pak_setup(bool on, const std::filesystem::path& path, bool persistent);
bool pak_present(int channel);
uint64_t pak_writes();

// oracle.cpp: per-frame rdram/display-list hashes (tools/recomp/oracle.py)
void oracle_start(const std::string& path, uint64_t stop_frames);
uint64_t oracle_frames();
void oracle_event(const char* text);
void oracle_finish(const char* why);
void oracle_note_stack(uint32_t top);
void oracle_note_thread(uint32_t thread);  // OSThread struct (osCreateThread a0)
void oracle_dump_at(uint64_t frame, const std::string& path);  // rdram (big-endian) at a frame
void oracle_virtual_clock(bool on);
void oracle_mask_images(bool on);  // --mask-rdp-images: RDP color/depth images out of the page hashes  // osGetCount/osGetTime from frames
// thread stacks as [lo, hi) offsets into rdram (masked out of page hashes)
const std::vector<std::pair<uint32_t, uint32_t>>& oracle_stack_ranges();
// rdram offset of a word the runtime owns (OSThread host pointer): never judged
bool oracle_word_masked(uint32_t off);

// capture.cpp: per-call capture of replaced functions (--call-log, --call-detail)
void capture_start(const std::string& path, uint64_t detail_seq);
void capture_finish();

// lockstep.cpp: deterministic VI/AI/SP/DP delivery (--clock frames)
void set_lockstep(bool on);
void set_lockstep_mirror(bool on);  // also send gfx tasks to the real renderer, 60 VI/s pacing
bool lockstep_enabled();
uint64_t lockstep_vis();
void lockstep_idle(uint8_t* rdram);
void lockstep_mirror_done();
// AI pacing (lockstep.cpp): AI messages at frequency / frames-per-buffer
void ai_note_frequency(uint32_t freq);
void ai_note_buffer(size_t samples);
void set_ai_pacing(bool on);   // --ai-per-vi: off (the runtime's one message per VI)
using ai_backlog_fn = int64_t (*)();   // frames queued for the audio device, -1 = no device
void set_ai_backlog(ai_backlog_fn fn);  // renderer: a mirrored gfx task's display list has been interpreted
} // namespace rush
