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
#include <mutex>
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
#ifdef RUSH_HAVE_AUDIO_UCODE
RspExitReason rush_audio_ucode(uint8_t* rdram, uint32_t ucode_addr);   // RSPRecomp output
#endif
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
        "  --ai-per-vi: one AI message per VI (the runtime's default; the game then makes half its audio)\n"
        "  --no-sound: do not open an audio device (rt64 build); --volume PERCENT (default 100)\n"
        "  --wav PATH: also write the game's audio output to a WAV file\n"
        "  --log-input: print every button/stick change per player (pad-mapping check)\n"
        "  --no-audio-ucode: audio RSP tasks complete without running the microcode (silence, as v1)\n"
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

// Audio: counted (audio_samples / audio_nonzero / audio_peak in the status
// line); played through SDL in the rt64 build with --renderer rt64 (unless
// --no-sound), dropped otherwise (headless: nothing queued).
bool sdl_audio = false;
// --wav PATH: the AI stream as a 16-bit stereo WAV (L/R corrected), rate =
// the last osAiSetFrequency; the header is finished at exit (trace.cpp).
FILE* wav_file = nullptr;
uint32_t wav_rate = 22050;
uint64_t wav_bytes = 0;
std::mutex wav_mu;
void wav_finish() {
    std::lock_guard<std::mutex> lock(wav_mu);
    if (!wav_file) return;
    auto le32 = [](uint8_t* p, uint32_t v) { for (int i = 0; i < 4; i++) p[i] = (uint8_t)(v >> (8 * i)); };
    uint8_t h[44] = {'R','I','F','F',0,0,0,0,'W','A','V','E','f','m','t',' ',16,0,0,0,1,0,2,0,
                     0,0,0,0,0,0,0,0,4,0,16,0,'d','a','t','a',0,0,0,0};
    le32(h + 4, (uint32_t)(36 + wav_bytes));
    le32(h + 24, wav_rate);
    le32(h + 28, wav_rate * 4);
    le32(h + 40, (uint32_t)wav_bytes);
    fseek(wav_file, 0, SEEK_SET);
    fwrite(h, 1, sizeof h, wav_file);
    fclose(wav_file);
    wav_file = nullptr;
}
void queue_samples(int16_t* s, size_t n) {
    rush::trace_audio_samples(s, n);
    rush::ai_note_buffer(n);
    if (wav_file) {
        std::lock_guard<std::mutex> lock(wav_mu);
        for (size_t i = 0; i + 1 < n; i += 2) {
            int16_t lr[2] = {s[i + 1], s[i]};
            uint8_t b[4] = {(uint8_t)lr[0], (uint8_t)(lr[0] >> 8), (uint8_t)lr[1], (uint8_t)(lr[1] >> 8)};
            fwrite(b, 1, 4, wav_file);
            wav_bytes += 4;
        }
    }
#ifdef RUSH_HAVE_RT64
    if (sdl_audio) rush::sdl_audio_queue(s, n);
#endif
}
size_t get_frames_remaining() {
#ifdef RUSH_HAVE_RT64
    if (sdl_audio) return rush::sdl_audio_frames_remaining();
#endif
    return 0;
}
void set_frequency(uint32_t f) {
    rush::trace_event("audio frequency %u", f);
    if (f) wav_rate = f;
    rush::ai_note_frequency(f);
#ifdef RUSH_HAVE_RT64
    if (sdl_audio) rush::sdl_audio_set_frequency(f);
#endif
}

// RSP: the audio microcode recompiled by RSPRecomp (Phase 5, RUSH_HAVE_AUDIO_UCODE:
// build/recomp/rsp/rush_audio_ucode.cpp from gen_config's rush2049.rsp_audio.toml);
// without it audio tasks complete at once (silence). Graphics tasks never get
// here (the renderer takes them).
RspExitReason null_ucode(uint8_t*, uint32_t) { return RspExitReason::Broke; }
bool audio_ucode_enabled = true;   // --no-audio-ucode
RspUcodeFunc* get_rsp_microcode(const OSTask* task) {
    static bool first_audio = true;
    if (task->t.type == M_AUDTASK && first_audio) {
        first_audio = false;
        rush::trace_event("audio task: ucode_boot 0x%08X/%u ucode 0x%08X/%u ucode_data 0x%08X/%u "
                          "dram_stack 0x%08X/%u output 0x%08X data 0x%08X/%u yield 0x%08X/%u flags %u",
                          (uint32_t)task->t.ucode_boot, task->t.ucode_boot_size, (uint32_t)task->t.ucode,
                          task->t.ucode_size, (uint32_t)task->t.ucode_data, task->t.ucode_data_size,
                          (uint32_t)task->t.dram_stack, task->t.dram_stack_size, (uint32_t)task->t.output_buff,
                          (uint32_t)task->t.data_ptr, task->t.data_size, (uint32_t)task->t.yield_data_ptr,
                          task->t.yield_data_size, task->t.flags);
#ifdef RUSH_HAVE_AUDIO_UCODE
        rush::trace_event(audio_ucode_enabled ? "audio ucode: recompiled (rush_audio_ucode, RSPRecomp)"
                                              : "audio ucode: disabled (--no-audio-ucode), tasks complete silently");
#else
        rush::trace_event("audio ucode: not built (no rsp/rush_audio_ucode.cpp), tasks complete silently");
#endif
    }
    rush::trace_rsp_task(task->t.type);
#ifdef RUSH_HAVE_AUDIO_UCODE
    if (task->t.type == M_AUDTASK && audio_ucode_enabled) return rush_audio_ucode;
#endif
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
    bool fullscreen = false, rt64_dev = false, keyboard_alone = false, no_sound = false, log_input = false;
    double volume = 100.0;
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
        else if (a == "--no-sound") no_sound = true;
        else if (a == "--ai-per-vi") rush::set_ai_pacing(false);
        else if (a == "--wav") {
            std::string w = next();
            wav_file = fopen(w.c_str(), "wb");
            if (!wav_file) { fprintf(stderr, "[rush] cannot write %s\n", w.c_str()); return 2; }
            uint8_t zero[44] = {};
            fwrite(zero, 1, sizeof zero, wav_file);
            rush::set_exit_hook(wav_finish);
        }
        else if (a == "--volume") volume = std::stod(next());
        else if (a == "--log-input") log_input = true;
        else if (a == "--no-audio-ucode") audio_ucode_enabled = false;
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
        rush::sdl_set_log_input(log_input);
        rush::sdl_audio_enable(!no_sound, (float)(volume / 100.0));
        sdl_audio = !no_sound;
        if (sdl_audio) rush::set_ai_backlog([]() -> int64_t {
            return rush::sdl_audio_active() ? (int64_t)rush::sdl_audio_frames_remaining() : -1; });
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
