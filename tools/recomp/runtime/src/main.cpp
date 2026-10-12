// Rush 2049 recomp host (Phase 1, boot). Headless: null renderer, no audio
// output, scripted controller input. See tools/recomp/BOOT_LOG.md.
//
//   rush2049_recomp --rom build/recomp/rom/rush2049.recomp.z64 [--home DIR]
//                   [--seconds N] [--press T:BUTTON[:DUR],...] [--fb-dump DIR]
//                   [--clock wall|frames] [--frames N] [--oracle-log PATH]
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
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
        "  SYNTHETIC_ROM = build/recomp/rom/rush2049.recomp.z64 (gen_config.py output)\n"
        "  BUTTON = A B Z START UP DOWN LEFT RIGHT L R CU CD CL CR STICKUP STICKDOWN ...\n");
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
    return reinterpret_cast<ultramodern::renderer::WindowHandle>(uintptr_t{1});
}
void update_gfx(ultramodern::gfx_callbacks_t::gfx_data_t) {}

std::string thread_name(const OSThread* t) { return "game" + std::to_string(t->id); }
void message_box(const char* msg) { fprintf(stderr, "[rush] message: %s\n", msg); }

} // namespace

int main(int argc, char** argv) {
    std::filesystem::path rom_path, home = "rush2049_recomp_home";
    double seconds = 30.0;
    std::string press, fb_dump, oracle_log, clock = "wall";
    uint64_t stop_frames = 0;
    std::string time_mode = "";
    bool no_lockstep = false;
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
        else if (a == "--fb-dump") fb_dump = next();
        else if (a == "--clock") clock = next();
        else if (a == "--frames") stop_frames = std::stoull(next());
        else if (a == "--oracle-log") oracle_log = next();
        else if (a == "--time") time_mode = next();
        else if (a == "--no-lockstep") no_lockstep = true;
        else if (a == "--dump-at") {
            std::string v = next();
            auto colon = v.find(':');
            if (colon == std::string::npos) { usage(); return 2; }
            rush::oracle_dump_at(std::stoull(v.substr(0, colon)), v.substr(colon + 1));
        }
        else { usage(); return 2; }
    }
    if (rom_path.empty() || (clock != "wall" && clock != "frames")) { usage(); return 2; }

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
    rush::oracle_start(oracle_log, stop_frames);
    rush::trace_start(seconds, press, fb_dump);

    static char arg0[] = "rush2049_recomp", arg1[] = "--game", arg2[] = "rush2049";
    static char* cli[] = {arg0, arg1, arg2, nullptr};

    recomp::Configuration cfg{};
    cfg.argc = 3;
    cfg.argv = cli;
    cfg.project_version = recomp::Version{0, 1, 0, "-phase1"};
    cfg.rsp_callbacks = recomp::rsp::callbacks_t{.get_rsp_microcode = get_rsp_microcode};
    cfg.renderer_callbacks = ultramodern::renderer::callbacks_t{.create_render_context = rush::create_null_renderer};
    cfg.audio_callbacks = ultramodern::audio_callbacks_t{
        .queue_samples = queue_samples, .get_frames_remaining = get_frames_remaining,
        .set_frequency = set_frequency};
    cfg.input_callbacks = ultramodern::input::callbacks_t{
        .poll_input = rush::poll_input, .get_input = rush::get_input,
        .set_rumble = rush::set_rumble, .get_connected_device_info = rush::get_connected_device_info};
    cfg.gfx_callbacks = ultramodern::gfx_callbacks_t{
        .create_gfx = create_gfx, .create_window = create_window, .update_gfx = update_gfx};
    cfg.events_callbacks = ultramodern::events::callbacks_t{.vi_callback = rush::on_vi, .gfx_init_callback = nullptr};
    cfg.error_handling_callbacks = ultramodern::error_handling::callbacks_t{.message_box = message_box};
    cfg.threads_callbacks = ultramodern::threads::callbacks_t{.get_game_thread_name = thread_name};
    recomp::start(cfg);
    return 0;
}
