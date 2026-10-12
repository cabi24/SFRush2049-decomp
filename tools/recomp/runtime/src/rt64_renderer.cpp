// RT64 renderer for the Rush 2049 recomp host (Phase 4; CMake RUSH_RENDERER=rt64).
// Implements ultramodern's RendererContext with RT64's HLE application, the
// way Zelda64Recomp's src/main/rt64_render_context.cpp does (no texture packs,
// no UI hooks). RT64 is fetched from GitHub at the commit in
// tools/recomp/VERSIONS.md; nothing here is ROM-derived.
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <memory>

#define HLSL_CPU
#include "hle/rt64_application.h"

#include "ultramodern/config.hpp"
#include "ultramodern/ultramodern.hpp"
#include "rush_runtime.h"

namespace {

uint8_t DMEM[0x1000];
uint8_t IMEM[0x1000];
unsigned int MI_INTR_REG = 0;
unsigned int DPC_START_REG = 0, DPC_END_REG = 0, DPC_CURRENT_REG = 0, DPC_STATUS_REG = 0, DPC_CLOCK_REG = 0,
             DPC_BUFBUSY_REG = 0, DPC_PIPEBUSY_REG = 0, DPC_TMEM_REG = 0;
void dummy_check_interrupts() {}

using UC = RT64::UserConfiguration;
namespace ur = ultramodern::renderer;

ur::SetupResult map_setup(RT64::Application::SetupResult r) {
    switch (r) {
        case RT64::Application::SetupResult::Success: return ur::SetupResult::Success;
        case RT64::Application::SetupResult::DynamicLibrariesNotFound: return ur::SetupResult::DynamicLibrariesNotFound;
        case RT64::Application::SetupResult::InvalidGraphicsAPI: return ur::SetupResult::InvalidGraphicsAPI;
        case RT64::Application::SetupResult::GraphicsAPINotFound: return ur::SetupResult::GraphicsAPINotFound;
        case RT64::Application::SetupResult::GraphicsDeviceNotFound: return ur::SetupResult::GraphicsDeviceNotFound;
    }
    return ur::SetupResult::GraphicsDeviceNotFound;
}

ur::GraphicsApi map_api(UC::GraphicsAPI api) {
    switch (api) {
        case UC::GraphicsAPI::D3D12: return ur::GraphicsApi::D3D12;
        case UC::GraphicsAPI::Vulkan: return ur::GraphicsApi::Vulkan;
        case UC::GraphicsAPI::Metal: return ur::GraphicsApi::Metal;
        default: return ur::GraphicsApi::Auto;
    }
}

const char* api_name(UC::GraphicsAPI api) {
    switch (api) {
        case UC::GraphicsAPI::D3D12: return "D3D12";
        case UC::GraphicsAPI::Vulkan: return "Vulkan";
        case UC::GraphicsAPI::Metal: return "Metal";
        default: return "Automatic";
    }
}

class Rt64Renderer : public ur::RendererContext {
public:
    Rt64Renderer(uint8_t* rdram, ur::WindowHandle window, bool developer_mode) {
        static unsigned char dummy_rom_header[0x40];
        RT64::Application::Core core{};
#if defined(_WIN32)
        core.window = window.window;
#elif defined(__linux__) || defined(__ANDROID__)
        core.window = window;
#elif defined(__APPLE__)
        core.window.window = window.window;
        core.window.view = window.view;
#endif
        core.checkInterrupts = dummy_check_interrupts;
        core.HEADER = dummy_rom_header;
        core.RDRAM = rdram;
        core.DMEM = DMEM;
        core.IMEM = IMEM;
        core.MI_INTR_REG = &MI_INTR_REG;
        core.DPC_START_REG = &DPC_START_REG;
        core.DPC_END_REG = &DPC_END_REG;
        core.DPC_CURRENT_REG = &DPC_CURRENT_REG;
        core.DPC_STATUS_REG = &DPC_STATUS_REG;
        core.DPC_CLOCK_REG = &DPC_CLOCK_REG;
        core.DPC_BUFBUSY_REG = &DPC_BUFBUSY_REG;
        core.DPC_PIPEBUSY_REG = &DPC_PIPEBUSY_REG;
        core.DPC_TMEM_REG = &DPC_TMEM_REG;
        ur::ViRegs* vi = ur::get_vi_regs();
        core.VI_STATUS_REG = &vi->VI_STATUS_REG;
        core.VI_ORIGIN_REG = &vi->VI_ORIGIN_REG;
        core.VI_WIDTH_REG = &vi->VI_WIDTH_REG;
        core.VI_INTR_REG = &vi->VI_INTR_REG;
        core.VI_V_CURRENT_LINE_REG = &vi->VI_V_CURRENT_LINE_REG;
        core.VI_TIMING_REG = &vi->VI_TIMING_REG;
        core.VI_V_SYNC_REG = &vi->VI_V_SYNC_REG;
        core.VI_H_SYNC_REG = &vi->VI_H_SYNC_REG;
        core.VI_LEAP_REG = &vi->VI_LEAP_REG;
        core.VI_H_START_REG = &vi->VI_H_START_REG;
        core.VI_V_START_REG = &vi->VI_V_START_REG;
        core.VI_V_BURST_REG = &vi->VI_V_BURST_REG;
        core.VI_X_SCALE_REG = &vi->VI_X_SCALE_REG;
        core.VI_Y_SCALE_REG = &vi->VI_Y_SCALE_REG;

        RT64::ApplicationConfiguration app_config;
        app_config.useConfigurationFile = false;
        app_ = std::make_unique<RT64::Application>(core, app_config);

        const ur::GraphicsConfig& cfg = ur::get_graphics_config();
        app_->userConfig.resolution = UC::Resolution::WindowIntegerScale;
        app_->userConfig.downsampleMultiplier = 1;
        app_->userConfig.aspectRatio = UC::AspectRatio::Original;
        app_->userConfig.antialiasing = UC::Antialiasing::None;
        app_->userConfig.refreshRate = UC::RefreshRate::Original;
        app_->userConfig.displayBuffering = UC::DisplayBuffering::Triple;
        app_->userConfig.developerMode = developer_mode;
        switch (cfg.api_option) {
            case ur::GraphicsApi::D3D12: app_->userConfig.graphicsAPI = UC::GraphicsAPI::D3D12; break;
            case ur::GraphicsApi::Vulkan: app_->userConfig.graphicsAPI = UC::GraphicsAPI::Vulkan; break;
            case ur::GraphicsApi::Metal: app_->userConfig.graphicsAPI = UC::GraphicsAPI::Metal; break;
            default: app_->userConfig.graphicsAPI = UC::GraphicsAPI::Automatic; break;
        }
        uint32_t thread_id = 0;
#ifdef _WIN32
        thread_id = window.thread_id;
#endif
        setup_result = map_setup(app_->setup(thread_id));
        chosen_api = map_api(app_->chosenGraphicsAPI);
        fprintf(stderr, "[rush] RT64 setup: %s, graphics API %s\n",
                setup_result == ur::SetupResult::Success ? "ok" : "FAILED", api_name(app_->chosenGraphicsAPI));
        if (setup_result != ur::SetupResult::Success) {
            app_ = nullptr;
            return;
        }
        app_->setFullScreen(cfg.wm_option == ur::WindowMode::Fullscreen);
    }

    bool valid() override { return app_ != nullptr; }

    bool update_config(const ur::GraphicsConfig& old_config, const ur::GraphicsConfig& new_config) override {
        if (old_config == new_config) return false;
        if (new_config.wm_option != old_config.wm_option)
            app_->setFullScreen(new_config.wm_option == ur::WindowMode::Fullscreen);
        return true;
    }

    void enable_instant_present() override {
        app_->enhancementConfig.presentation.mode = RT64::EnhancementConfiguration::Presentation::Mode::PresentEarly;
        app_->updateEnhancementConfig();
    }

    void send_dl(const OSTask* task) override {
        if (!rush::lockstep_enabled()) rush::trace_gfx_task(task);   // lockstep counts it itself
        app_->state->rsp->reset();
        app_->interpreter->loadUCodeGBI(task->t.ucode & 0x3FFFFFF, task->t.ucode_data & 0x3FFFFFF, true);
        app_->processDisplayLists(app_->core.RDRAM, task->t.data_ptr & 0x3FFFFFF, 0, true);
    }

    void send_dummy_workload(uint32_t) override {}

    void update_screen() override {
        rush::trace_screen_update(app_->core.RDRAM, ur::get_vi_regs()->VI_ORIGIN_REG);
        app_->updateScreen();
    }

    void shutdown() override {
        if (app_ != nullptr) app_->end();
    }

    uint32_t get_display_framerate() const override {
        return app_->presentQueue->ext.sharedResources->swapChainRate;
    }

    float get_resolution_scale() const override {
        constexpr int kReferenceHeight = 240;
        if (app_->sharedQueueResources->swapChainHeight > 0)
            return std::max(float((app_->sharedQueueResources->swapChainHeight + kReferenceHeight - 1) /
                                  kReferenceHeight), 1.0f);
        return 1.0f;
    }

private:
    std::unique_ptr<RT64::Application> app_;
};

} // namespace

std::unique_ptr<ultramodern::renderer::RendererContext> rush::create_rt64_renderer(
    uint8_t* rdram, ultramodern::renderer::WindowHandle window, bool developer_mode) {
    return std::make_unique<Rt64Renderer>(rdram, window, developer_mode);
}
