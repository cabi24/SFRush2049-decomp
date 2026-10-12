// SDL2 window, events and controller/keyboard input for the playable host
// (Phase 4; built with CMake RUSH_RENDERER=rt64). The null renderer build has
// no window and takes input only from --press scripts (trace.cpp).
//
// Controls (TEST_PROCESS.md repeats them):
//   game controller (XInput / SDL mapping, up to 4): left stick = analog stick,
//     A (south) = A, X (west) = B, B (east) = C-down, Y (north) = C-up,
//     right stick = C buttons, LB = L, RB or RT = R, LT = Z, START = START,
//     D-pad = D-pad
//   keyboard (player 1): arrows = analog stick, X = A, C = B, Z = Z,
//     Enter = START, Q = L, E = R, W/A/S/D = D-pad, I/J/K/L = C-up/left/down/right,
//     Escape = quit (writes the final status line)
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define SDL_MAIN_HANDLED
#ifdef _WIN32
#include "SDL.h"
#include "SDL_syswm.h"
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#undef None
#undef Status
#undef LockMask
#undef ControlMask
#undef Success
#undef Always
#endif

#include "rush_runtime.h"

namespace {

SDL_Window* window = nullptr;
SDL_GameController* pads[4] = {};
std::atomic<uint32_t> pad_buttons[4];
std::atomic<int32_t> pad_x[4], pad_y[4];   // stick * 32767
bool fullscreen = false;
int pad_first_port = 0;   // --keyboard-alone: controllers start at player 2

enum : uint16_t {
    N_A = 0x8000, N_B = 0x4000, N_Z = 0x2000, N_START = 0x1000, N_DU = 0x0800, N_DD = 0x0400, N_DL = 0x0200,
    N_DR = 0x0100, N_L = 0x0020, N_R = 0x0010, N_CU = 0x0008, N_CD = 0x0004, N_CL = 0x0002, N_CR = 0x0001
};

void open_pads() {
    int slot = pad_first_port;
    for (int i = 0; i < SDL_NumJoysticks() && slot < 4; i++) {
        if (!SDL_IsGameController(i)) continue;
        if (SDL_GameController* c = SDL_GameControllerOpen(i)) {
            fprintf(stderr, "[rush] controller -> player %d: %s\n", slot + 1, SDL_GameControllerName(c));
            pads[slot++] = c;
        }
    }
}

void close_pads() {
    for (auto& p : pads) {
        if (p) SDL_GameControllerClose(p);
        p = nullptr;
    }
}

float axis(SDL_GameController* c, SDL_GameControllerAxis a) {
    float v = SDL_GameControllerGetAxis(c, a) / 32767.0f;
    return std::fabs(v) < 0.15f ? 0.0f : std::fmax(-1.0f, std::fmin(1.0f, v));
}

void sample() {
    const Uint8* k = SDL_GetKeyboardState(nullptr);
    for (int port = 0; port < 4; port++) {
        uint16_t m = 0;
        float x = 0, y = 0;
        if (SDL_GameController* c = pads[port]) {
            auto b = [&](SDL_GameControllerButton btn) { return SDL_GameControllerGetButton(c, btn) != 0; };
            if (b(SDL_CONTROLLER_BUTTON_A)) m |= N_A;
            if (b(SDL_CONTROLLER_BUTTON_X)) m |= N_B;
            if (b(SDL_CONTROLLER_BUTTON_START)) m |= N_START;
            if (b(SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) m |= N_L;
            if (b(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) m |= N_R;
            if (b(SDL_CONTROLLER_BUTTON_DPAD_UP)) m |= N_DU;
            if (b(SDL_CONTROLLER_BUTTON_DPAD_DOWN)) m |= N_DD;
            if (b(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) m |= N_DL;
            if (b(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) m |= N_DR;
            if (b(SDL_CONTROLLER_BUTTON_B)) m |= N_CD;
            if (b(SDL_CONTROLLER_BUTTON_Y)) m |= N_CU;
            if (axis(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 0.5f) m |= N_Z;
            if (axis(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 0.5f) m |= N_R;
            float rx = axis(c, SDL_CONTROLLER_AXIS_RIGHTX), ry = axis(c, SDL_CONTROLLER_AXIS_RIGHTY);
            if (ry < -0.5f) m |= N_CU;
            if (ry > 0.5f) m |= N_CD;
            if (rx < -0.5f) m |= N_CL;
            if (rx > 0.5f) m |= N_CR;
            x = axis(c, SDL_CONTROLLER_AXIS_LEFTX);
            y = -axis(c, SDL_CONTROLLER_AXIS_LEFTY);
        }
        if (port == 0) {   // the keyboard is player 1
            if (k[SDL_SCANCODE_X]) m |= N_A;
            if (k[SDL_SCANCODE_C]) m |= N_B;
            if (k[SDL_SCANCODE_Z]) m |= N_Z;
            if (k[SDL_SCANCODE_RETURN]) m |= N_START;
            if (k[SDL_SCANCODE_Q]) m |= N_L;
            if (k[SDL_SCANCODE_E]) m |= N_R;
            if (k[SDL_SCANCODE_I]) m |= N_CU;
            if (k[SDL_SCANCODE_K]) m |= N_CD;
            if (k[SDL_SCANCODE_J]) m |= N_CL;
            if (k[SDL_SCANCODE_L]) m |= N_CR;
            if (k[SDL_SCANCODE_W]) m |= N_DU;
            if (k[SDL_SCANCODE_S]) m |= N_DD;
            if (k[SDL_SCANCODE_A]) m |= N_DL;
            if (k[SDL_SCANCODE_D]) m |= N_DR;
            if (k[SDL_SCANCODE_LEFT]) x = -1.0f;
            if (k[SDL_SCANCODE_RIGHT]) x = 1.0f;
            if (k[SDL_SCANCODE_UP]) y = 1.0f;
            if (k[SDL_SCANCODE_DOWN]) y = -1.0f;
        }
        pad_buttons[port] = m;
        pad_x[port] = (int32_t)(x * 32767.0f);
        pad_y[port] = (int32_t)(y * 32767.0f);
    }
}

} // namespace

void rush::sdl_set_fullscreen(bool on) { fullscreen = on; }
void rush::sdl_set_keyboard_alone(bool on) { pad_first_port = on ? 1 : 0; }

ultramodern::gfx_callbacks_t::gfx_data_t rush::sdl_create_gfx() {
    SDL_SetMainReady();
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
    SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        fprintf(stderr, "[rush] SDL_Init failed: %s\n", SDL_GetError());
        std::exit(2);
    }
    fprintf(stderr, "[rush] SDL video driver: %s\n", SDL_GetCurrentVideoDriver());
    open_pads();
    return nullptr;
}

ultramodern::renderer::WindowHandle rush::sdl_create_window(ultramodern::gfx_callbacks_t::gfx_data_t) {
    uint32_t flags = SDL_WINDOW_RESIZABLE;
#if defined(__APPLE__)
    flags |= SDL_WINDOW_METAL;
#elif defined(RT64_SDL_WINDOW_VULKAN)
    flags |= SDL_WINDOW_VULKAN;
#endif
    window = SDL_CreateWindow("San Francisco Rush 2049 (recomp test)", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 1280, 960, flags);
    if (window == nullptr) {
        fprintf(stderr, "[rush] SDL_CreateWindow failed: %s\n", SDL_GetError());
        std::exit(2);
    }
#if defined(__linux__)
    if (fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
    SDL_SysWMinfo wm;
    SDL_VERSION(&wm.version);
    SDL_GetWindowWMInfo(window, &wm);
#if defined(_WIN32)
    return ultramodern::renderer::WindowHandle{wm.info.win.window, GetCurrentThreadId()};
#elif defined(__linux__) || defined(__ANDROID__)
    return ultramodern::renderer::WindowHandle{window};
#else
    static_assert(false, "platform not supported by the Rush 2049 recomp host");
#endif
}

void rush::sdl_update_gfx(ultramodern::gfx_callbacks_t::gfx_data_t) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_QUIT:
                rush::request_quit("window closed");
                break;
            case SDL_KEYDOWN:
                if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE) rush::request_quit("escape");
                break;
            case SDL_CONTROLLERDEVICEADDED:
            case SDL_CONTROLLERDEVICEREMOVED:
                close_pads();
                open_pads();
                break;
            default:
                break;
        }
    }
    sample();
}

bool rush::sdl_get_input(int port, uint16_t* buttons, float* x, float* y) {
    if (port < 0 || port > 3) return false;
    if (port > 0 && pads[port] == nullptr) return false;   // player 1 always present (keyboard)
    *buttons = (uint16_t)pad_buttons[port].load();
    *x = pad_x[port].load() / 32767.0f;
    *y = pad_y[port].load() / 32767.0f;
    return true;
}

bool rush::sdl_port_connected(int port) { return port == 0 || (port > 0 && port < 4 && pads[port] != nullptr); }
