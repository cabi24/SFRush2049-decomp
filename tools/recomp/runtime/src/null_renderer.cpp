// Headless renderer: accepts display lists and screen updates, executes
// nothing, and reports them to trace.cpp. Proves the boot reaches the gfx
// path (gfx tasks submitted, VI swaps with a framebuffer origin) without RT64.
#include <chrono>
#include <thread>

#include "rush_runtime.h"

// --null-dl-delay MS (test only): each display list "takes" MS ms on the
// renderer thread, as RT64's does, so the DP completion reaches the game
// asynchronously and late. Reproduces the v1 wall-clock boot hang headless
// (game_loop's busy-wait at 0x800FD500, patches.json rush_hook_spin).
static int null_dl_delay_ms = 0;
void rush::set_null_dl_delay_ms(int ms) { null_dl_delay_ms = ms; }

namespace {

class NullRenderer : public ultramodern::renderer::RendererContext {
public:
    explicit NullRenderer(uint8_t* rdram) : rdram_(rdram) {
        setup_result = ultramodern::renderer::SetupResult::Success;
        chosen_api = ultramodern::renderer::GraphicsApi::Auto;
    }
    bool valid() override { return true; }
    bool update_config(const ultramodern::renderer::GraphicsConfig&,
                       const ultramodern::renderer::GraphicsConfig&) override { return true; }
    void enable_instant_present() override {}
    void send_dl(const OSTask* task) override {
        rush::trace_gfx_task(task);
        if (null_dl_delay_ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(null_dl_delay_ms));
    }
    void send_dummy_workload(uint32_t) override {}
    void update_screen() override {
        rush::trace_screen_update(rdram_, ultramodern::renderer::get_vi_regs()->VI_ORIGIN_REG);
    }
    void shutdown() override {}
    uint32_t get_display_framerate() const override { return 60; }
    float get_resolution_scale() const override { return 1.0f; }

private:
    uint8_t* rdram_;
};

} // namespace

std::unique_ptr<ultramodern::renderer::RendererContext> rush::create_null_renderer(
    uint8_t* rdram, ultramodern::renderer::WindowHandle, bool) {
    return std::make_unique<NullRenderer>(rdram);
}
