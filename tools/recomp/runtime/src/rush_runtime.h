// Shared declarations of the Rush 2049 recomp host (tools/recomp/runtime).
#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "ultramodern/ultramodern.hpp"

namespace rush {
// register_overlays.cpp
void register_overlays();

// null_renderer.cpp
std::unique_ptr<ultramodern::renderer::RendererContext> create_null_renderer(
    uint8_t* rdram, ultramodern::renderer::WindowHandle window, bool developer_mode);

// trace.cpp: counters, the status line, the run timer, scripted input
void trace_start(double seconds, const std::string& press, const std::string& fb_dump);
void trace_event(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
void trace_rsp_task(uint32_t type);
void trace_gfx_task(const OSTask* task);
void trace_screen_update(uint8_t* rdram, uint32_t vi_origin);
void trace_audio_samples(size_t n);
void trace_overlay(const char* name, uint32_t stream_rom, uint32_t dst);
void trace_shim(const char* name, uint32_t ra);
void on_vi();
void poll_input();
bool get_input(int controller, uint16_t* buttons, float* x, float* y);
void set_rumble(int controller, bool on);
ultramodern::input::connected_device_info_t get_connected_device_info(int controller);
// --clock frames: scripted input reads game frames / 30 instead of wall time
void set_frame_clock(bool on);

// oracle.cpp: per-frame rdram/display-list hashes (tools/recomp/oracle.py)
void oracle_start(const std::string& path, uint64_t stop_frames);
uint64_t oracle_frames();
void oracle_event(const char* text);
void oracle_finish(const char* why);
void oracle_virtual_clock(bool on);  // osGetCount/osGetTime from frames

// lockstep.cpp: deterministic VI/AI/SP/DP delivery (--clock frames)
void set_lockstep(bool on);
bool lockstep_enabled();
uint64_t lockstep_vis();
void lockstep_idle(uint8_t* rdram);
} // namespace rush
