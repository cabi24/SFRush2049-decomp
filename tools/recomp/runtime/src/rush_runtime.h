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
} // namespace rush
