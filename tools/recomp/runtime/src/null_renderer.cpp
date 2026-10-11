// Headless renderer: accepts display lists and screen updates, executes
// nothing, and reports them to trace.cpp. Proves the boot reaches the gfx
// path (gfx tasks submitted, VI swaps with a framebuffer origin) without RT64.
#include "rush_runtime.h"

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
    void send_dl(const OSTask* task) override { rush::trace_gfx_task(task); }
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
