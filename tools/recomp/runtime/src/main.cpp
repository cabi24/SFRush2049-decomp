// Rush 2049 recomp host. Phase 1: headless (null renderer, no audio output,
// scripted controller input; tools/recomp/BOOT_LOG.md). Phase 4: built with
// CMake RUSH_RENDERER=rt64 it also has a window (SDL2), RT64 and live
// controller/keyboard input (--renderer rt64, the default in that build).
//
//   rush2049_recomp --rom build/recomp/rom/rush2049.recomp.z64 [--home DIR]
//                   [--seconds N] [--press T:BUTTON[:DUR],...] [--fb-dump DIR]
//                   [--clock wall|frames] [--frames N] [--oracle-log PATH]
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "recomp.h"
#include "librecomp/game.hpp"
#include "librecomp/overlays.hpp"
#include "ultramodern/ultramodern.hpp"
#include "xxHash/xxh3.h"

#include "rush_runtime.h"

extern "C" void recomp_entrypoint(uint8_t* rdram, recomp_context* ctx);
gpr get_entrypoint_address();

namespace {

std::vector<uint8_t> read_all(const std::filesystem::path& p) {
    std::ifstream f(p, std::ios::binary);
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}

void usage() {
    fprintf(stderr,
        "usage: rush2049_recomp --rom SYNTHETIC_ROM [--home DIR] [--seconds N]\n"
        "                       [--press T:BUTTON[:DUR],...] [--fb-dump DIR]\n"
        "                       [--clock wall|frames] [--frames N] [--oracle-log PATH]\n"
        "  --clock frames: --press times are game frames / 30 (deterministic replay)\n"
        "  --frames N: exit after N graphics frames; --oracle-log: per-frame hashes\n"
        "  --time virtual|wall: what osGetCount/osGetTime return (default: virtual with --clock frames)\n"
        "  --clock frames also delivers VI/AI/SP/DP events in lockstep (src/lockstep.cpp);\n"
        "  --no-lockstep keeps the runtime's wall-clock events\n"
        "  --dump-at FRAME:PATH writes the 8 MiB of rdram (big-endian) at that frame\n"
        "  --call-log PATH: per-call capture of replaced/captured functions (src/capture.cpp)\n"
        "  --call-detail SEQ: also list every word call SEQ changed (W lines)\n"
        "  --press items T:BUTTON[@PORT][:DUR]; ports used = controllers connected\n"
        "  SYNTHETIC_ROM = build/recomp/rom/rush2049.recomp.z64 (gen_config.py output)\n"
        "  BUTTON = A B Z START UP DOWN LEFT RIGHT L R CU CD CL CR STICKUP STICKDOWN ...\n"
        "  --press-file PATH: a tools/recomp/scripts/*.press file (same as oracle.py script NAME)\n"
        "  --renderer null|rt64: rt64 = window + RT64 + live input (only in a RUSH_RENDERER=rt64 build;\n"
        "      its default); --seconds 0 = run until the window is closed (rt64 default)\n"
        "  --gfx-api auto|d3d12|vulkan, --fullscreen, --rt64-dev (RT64 developer overlay)\n"
        "  --keyboard-alone: the keyboard is player 1 and game controllers start at player 2\n"
        "  --null-dl-delay MS: (test) the null renderer takes MS ms per display list, like RT64\n"
        "  --mask-rdp-images: zero RDP color/depth images in the oracle's page hashes (RT64 self-check)\n"
        "  --no-spin-yield: (diagnostic) busy-wait hooks do not yield (the v1 behaviour)\n");
}

// tools/recomp/scripts/*.press -> the --press string (oracle.py press_string):
// `TIME BUTTON[@PORT] [DURATION]` per line, `frames N`, `#` comments.
std::string press_file(const std::string& path, uint64_t& frames) {
    std::ifstream f(path);
    if (!f) { fprintf(stderr, "[rush] cannot read %s\n", path.c_str()); std::exit(2); }
    std::string line, out;
    while (std::getline(f, line)) {
        auto hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);
        std::istringstream is(line);
        std::string t, b, d;
        if (!(is >> t)) continue;
        if (t == "frames") { is >> frames; continue; }
        if (!(is >> b)) { fprintf(stderr, "[rush] %s: bad line '%s'\n", path.c_str(), line.c_str()); std::exit(2); }
        if (!out.empty()) out += ",";
        out += t + ":" + b;
        if (is >> d) out += ":" + d;
    }
    return out;
}

// Audio: accept and drop samples, report nothing queued.
void queue_samples(int16_t*, size_t n) { rush::trace_audio_samples(n); }
size_t get_frames_remaining() { return 0; }
void set_frequency(uint32_t f) { rush::trace_event("audio frequency %u", f); }

// RSP: there is no recompiled audio microcode; audio tasks complete at once.
RspExitReason null_ucode(uint8_t*, uint32_t) { return RspExitReason::Broke; }
RspUcodeFunc* get_rsp_microcode(const OSTask* task) {
    rush::trace_rsp_task(task->t.type);
    return null_ucode;
}

ultramodern::gfx_callbacks_t::gfx_data_t create_gfx() { return nullptr; }
ultramodern::renderer::WindowHandle create_window(ultramodern::gfx_callbacks_t::gfx_data_t) {
    // Headless: ultramodern only hands the handle to the renderer, which ignores it.
#if defined(_WIN32) || defined(__APPLE__)
    return ultramodern::renderer::WindowHandle{};
#else
    return reinterpret_cast<ultramodern::renderer::WindowHandle>(uintptr_t{1});
#endif
}
void update_gfx(ultramodern::gfx_callbacks_t::gfx_data_t) {}

std::string thread_name(const OSThread* t) { return "game" + std::to_string(t->id); }
void message_box(const char* msg) { fprintf(stderr, "[rush] message: %s\n", msg); }

} // namespace

int main(int argc, char** argv) {
    std::filesystem::path rom_path, home = "rush2049_recomp_home";
    double seconds = -1.0;   // default: 30 (null renderer), 0 = unlimited (rt64)
#ifdef RUSH_HAVE_RT64
    std::string renderer = "rt64";
#else
    std::string renderer = "null";
#endif
    std::string gfx_api = "auto";
    bool fullscreen = false, rt64_dev = false, keyboard_alone = false;
    uint64_t file_frames = 0;
    std::string press, fb_dump, oracle_log, clock = "wall";
    uint64_t stop_frames = 0;
    std::string time_mode = "";
    bool no_lockstep = false;
    std::string call_log;
    uint64_t call_detail = 0;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        auto next = [&]() -> std::string {
            if (i + 1 >= argc) { usage(); std::exit(2); }
            return argv[++i];
        };
        if (a == "--rom") rom_path = next();
        else if (a == "--home") home = next();
        else if (a == "--seconds") seconds = std::stod(next());
        else if (a == "--press") press = next();
        else if (a == "--press-file") press = press_file(next(), file_frames);
        else if (a == "--renderer") renderer = next();
        else if (a == "--gfx-api") gfx_api = next();
        else if (a == "--fullscreen") fullscreen = true;
        else if (a == "--rt64-dev") rt64_dev = true;
        else if (a == "--keyboard-alone") keyboard_alone = true;
        else if (a == "--fb-dump") fb_dump = next();
        else if (a == "--clock") clock = next();
        else if (a == "--frames") stop_frames = std::stoull(next());
        else if (a == "--oracle-log") oracle_log = next();
        else if (a == "--time") time_mode = next();
        else if (a == "--no-lockstep") no_lockstep = true;
        else if (a == "--call-log") call_log = next();
        else if (a == "--call-detail") call_detail = std::stoull(next());
        else if (a == "--mask-rdp-images") rush::oracle_mask_images(true);
        else if (a == "--no-spin-yield") rush::set_spin_yield(false);
        else if (a == "--null-dl-delay") rush::set_null_dl_delay_ms(std::stoi(next()));
        else if (a == "--dump-at") {
            std::string v = next();
            auto colon = v.find(':');
            if (colon == std::string::npos) { usage(); return 2; }
            rush::oracle_dump_at(std::stoull(v.substr(0, colon)), v.substr(colon + 1));
        }
        else { usage(); return 2; }
    }
    if (rom_path.empty() || (clock != "wall" && clock != "frames")) { usage(); return 2; }
    if (renderer != "null" && renderer != "rt64") { usage(); return 2; }
#ifndef RUSH_HAVE_RT64
    if (renderer == "rt64") {
        fprintf(stderr, "[rush] --renderer rt64: this build has no RT64 (configure with -DRUSH_RENDERER=rt64)\n");
        return 2;
    }
#endif
    if (stop_frames == 0 && file_frames != 0 && clock == "frames") stop_frames = file_frames;
    if (seconds < 0) seconds = renderer == "rt64" ? 0.0 : 30.0;
    fprintf(stderr, "[rush] renderer %s, clock %s%s\n", renderer.c_str(), clock.c_str(),
            press.empty() ? (renderer == "rt64" ? ", live input" : ", no input") : ", scripted input");

    std::vector<uint8_t> rom = read_all(rom_path);
    if (rom.size() < 0x1000) {
        fprintf(stderr, "[rush] cannot read ROM %s\n", rom_path.string().c_str());
        return 2;
    }
    rom.resize((rom.size() + 3) & ~size_t{3});
    // The synthetic ROM is private and derived; register whatever hash it has
    // (gen_config.py already refused anything but the verified retail ROM).
    uint64_t hash = XXH3_64bits(rom.data(), rom.size());

    std::filesystem::create_directories(home);
    recomp::register_config_path(home);

    recomp::GameEntry game{};
    game.rom_hash = hash;
    game.internal_name = std::string(reinterpret_cast<const char*>(rom.data()) + 0x20, 20);
    game.display_name = "San Francisco Rush 2049 (US)";
    game.game_id = u8"rush2049.n64.us";
    game.mod_game_id = "rush2049";
    game.save_type = recomp::SaveType::AllowAll;
    game.is_enabled = true;
    game.entrypoint_address = get_entrypoint_address();
    game.entrypoint = recomp_entrypoint;
    recomp::register_game(game);
    rush::register_overlays();

    auto rc = recomp::select_rom(rom_path, game.game_id);
    if (rc != recomp::RomValidationError::Good) {
        fprintf(stderr, "[rush] select_rom failed (%d)\n", static_cast<int>(rc));
        return 2;
    }
    recomp::check_all_stored_roms();

    rush::set_frame_clock(clock == "frames");
    // osGetCount/osGetTime: virtual (frame-derived) or wall; default follows --clock
    if (time_mode.empty()) time_mode = clock == "frames" ? "virtual" : "wall";
    if (time_mode != "virtual" && time_mode != "wall") { usage(); return 2; }
    rush::oracle_virtual_clock(time_mode == "virtual");
    rush::set_lockstep(clock == "frames" && !no_lockstep);
    rush::set_lockstep_mirror(renderer == "rt64");
    rush::oracle_start(oracle_log, stop_frames);
    rush::capture_start(call_log, call_detail);
    rush::trace_start(seconds, press, fb_dump);

    static char arg0[] = "rush2049_recomp", arg1[] = "--game", arg2[] = "rush2049";
    static char* cli[] = {arg0, arg1, arg2, nullptr};

    recomp::Configuration cfg{};
    cfg.argc = 3;
    cfg.argv = cli;
    cfg.project_version = recomp::Version{0, 4, 0, "-phase4"};
    cfg.rsp_callbacks = recomp::rsp::callbacks_t{.get_rsp_microcode = get_rsp_microcode};
    cfg.renderer_callbacks = ultramodern::renderer::callbacks_t{.create_render_context = rush::create_null_renderer};
#ifdef RUSH_HAVE_RT64
    if (renderer == "rt64") {
        cfg.renderer_callbacks = ultramodern::renderer::callbacks_t{.create_render_context = rush::create_rt64_renderer};
        ultramodern::renderer::GraphicsConfig gc = ultramodern::renderer::get_graphics_config();
        gc.developer_mode = rt64_dev;
        gc.api_option = gfx_api == "d3d12" ? ultramodern::renderer::GraphicsApi::D3D12
                      : gfx_api == "vulkan" ? ultramodern::renderer::GraphicsApi::Vulkan
                      : ultramodern::renderer::GraphicsApi::Auto;
        gc.wm_option = fullscreen ? ultramodern::renderer::WindowMode::Fullscreen
                                  : ultramodern::renderer::WindowMode::Windowed;
        ultramodern::renderer::set_graphics_config(gc);
        rush::sdl_set_fullscreen(fullscreen);
        rush::sdl_set_keyboard_alone(keyboard_alone);
        rush::set_live_input(rush::sdl_get_input, rush::sdl_port_connected);
    }
#endif
    cfg.audio_callbacks = ultramodern::audio_callbacks_t{
        .queue_samples = queue_samples, .get_frames_remaining = get_frames_remaining,
        .set_frequency = set_frequency};
    cfg.input_callbacks = ultramodern::input::callbacks_t{
        .poll_input = rush::poll_input, .get_input = rush::get_input,
        .set_rumble = rush::set_rumble, .get_connected_device_info = rush::get_connected_device_info};
    cfg.gfx_callbacks = ultramodern::gfx_callbacks_t{
        .create_gfx = create_gfx, .create_window = create_window, .update_gfx = update_gfx};
#ifdef RUSH_HAVE_RT64
    if (renderer == "rt64")
        cfg.gfx_callbacks = ultramodern::gfx_callbacks_t{.create_gfx = rush::sdl_create_gfx,
            .create_window = rush::sdl_create_window, .update_gfx = rush::sdl_update_gfx};
#endif
    cfg.events_callbacks = ultramodern::events::callbacks_t{.vi_callback = rush::on_vi, .gfx_init_callback = nullptr};
    cfg.error_handling_callbacks = ultramodern::error_handling::callbacks_t{.message_box = message_box};
    cfg.threads_callbacks = ultramodern::threads::callbacks_t{.get_game_thread_name = thread_name};
    recomp::start(cfg);
    return 0;
}
