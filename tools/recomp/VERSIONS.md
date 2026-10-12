# Recompiler versions (Phase 0, recorded 2026-10-11 on rocky)

Host: rocky, Rocky Linux 10.2, kernel 6.12.0-211.16.1.el10_2, x86-64, 20 cores.
Layout: `~/rush2049/recomp/{src,build,rom,repo}` (repo = rsync of the w20r worktree's tracked files).

## N64Recomp (the generator we run)

- `https://github.com/N64Recomp/N64Recomp` at `ffb39cdad1da5de07eaaa48bd1db4a89a7986771`
  (2026-05-27, "dump symbols in deterministic order (sorted by addr, name) (#179)")
- Submodules: ELFIO `ad8b641f` (Release_3.7-236), fmt `407c905e` (12.1.0), rabbitizer `e0d80030`
  (1.7.10), sljit `f6326087`, tomlplusplus `1f7884e5` (v3.4.0-7)
- Build: `~/rush2049/recomp/build/N64Recomp/`, CMake 3.31.8 + Ninja 1.11.1, Release, clang/clang++
  21.1.8; binary `N64Recomp` sha1 `318355606528931e1d23459a17e61df2a4c8378e` (RecompModTool and
  OfflineModRecomp built alongside, unused)

## N64ModernRuntime (built in Phase 1)

- `https://github.com/N64Recomp/N64ModernRuntime` at `cdf5abbd5026fef5c364c676e4667c45e42b6863`
  (2026-08-30, "Add CLI options to select games and game modes. (#153)"), cloned recursively at
  `~/rush2049/recomp/src/N64ModernRuntime`; built in Phase 1 (lane w21r) as a subproject of
  `tools/recomp/runtime` (librecomp + ultramodern static libraries, its own N64Recomp
  submodule for LiveRecomp), CMake 3.31.8 + Ninja, clang 21.1.8, RelWithDebInfo
- Submodules: N64Recomp `81213c18` (mod-tool-release-20; an ancestor of ffb39cda, 2 commits
  behind), miniz `8573fd7c` (3.0.2-41), o1heap `a124b850`, xxHash `ac3a25da` (v0.7.4-748)
- Phase 0 compile check uses `N64ModernRuntime/N64Recomp/include/recomp.h` (librecomp and
  ultramodern include dirs for `recomp_overlays.inl` / `lookup.cpp`)

## Toolchain on rocky

clang 21.1.8 (RESF 21.1.8-1.el10), gcc 14.3.1 20251022, CMake 3.31.8, Ninja 1.11.1, Python 3.12.13.

## Inputs

ROM `~/rush2049/recomp/rom/baserom.us.z64`, SHA-1 `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`
(`us.sha1`), copied by hand. Generated config hashes (identical on the Pi and rocky):
`rush2049.syms.toml` sha256 `1df64b46...`, `rush2049.recomp.toml` `f432fe6d...`,
`manifest.json` `75cac2ea...` (final run of branch w20r).

## Pin decision (Phase 1, 2026-10-11)

The runtime's submodule pins N64Recomp `81213c18`; we generate with `ffb39cda`. `git log
81213c18..ffb39cda` is two commits: `70a894a` (adds CONTRIBUTING.md) and `ffb39cd` ("dump
symbols in deterministic order"), whose only code change is `src/main.cpp` `dump_context`
(sorting relocs/functions/data symbols in the `--dump-context` output). Neither touches the
code generator, `recomp.h`, the section table or `funcs.h`, so the emitted C contract is the
same: we keep generating with `ffb39cda` and link against the runtime at `cdf5abbd` with its
pinned `81213c18` headers (the build compiles the emitted C against
`N64ModernRuntime/N64Recomp/include/recomp.h`).

## RT64 (Phase 4, lane w24r, 2026-10-12)

- `https://github.com/rt64/rt64` at `23cab603c4f9f4a8b369b38e036f1aa484603878` (2025-08-25, "Re-enable sleep
  on display framerate setting to fix pacing issues on some multi monitor setups"): the commit
  Zelda64Recomp (dev `b65c482e`, 2026-09-25) pins as `lib/rt64`. Cloned recursively at
  `~/rush2049/recomp/src/rt64` (955 MB with submodules: dxc-bin, mupen64plus-win32-deps (SDL2 2.26.3 for
  Windows), Vulkan-Headers, volk, VMA, D3D12MA, imgui, nativefiledialog-extended, zstd, ...).
- Interface check: N64ModernRuntime cdf5abbd's `RendererContext` adds `send_dummy_workload` over the
  df7e820d Zelda pins; our `runtime/src/rt64_renderer.cpp` implements it (no-op), everything else is the
  RT64 HLE API Zelda64Recomp uses.
- Built on rocky (compile + link only; rocky has no display, so the RT64 window is never run there):
  `-DRUSH_RENDERER=rt64 -DRT64_DIR=...`, clang 21.1.8, 496 ninja steps, 45 s with 20 jobs, binary 63 MB
  (RelWithDebInfo). Needed on Rocky 10: `SDL2-devel` (2.28.5) and `gtk3-devel` (3.24.43, for RT64's
  nativefiledialog-extended), installed with dnf for this lane; RT64's `-Werror` is relaxed for its
  target (clang 21's `-Wuninitialized-const-pointer` fires in its bundled imgui).
- Windows: the host sources were syntax-checked against Windows headers (fedora:41 container,
  clang 19 `--target=x86_64-w64-mingw32`, mingw-w64 12 headers, `-DRUSH_HAVE_RT64 -DRUSH_CAPTURE=0`): all
  11 files parse. Not built or run on Windows by us (no Windows host).

## Gameplay-test package

`tools/recomp/package.py` reads this block. Every recomp phase that changes what a tester runs
regenerates the package and bumps `package_version` (and adds a section to `package/CHANGES.md`).

<!-- package-pins
package_version = 1
n64recomp = ffb39cdad1da5de07eaaa48bd1db4a89a7986771
n64modernruntime = cdf5abbd5026fef5c364c676e4667c45e42b6863
rt64_url = https://github.com/rt64/rt64
rt64 = 23cab603c4f9f4a8b369b38e036f1aa484603878
-->

| Version | Date | Built from | Notes |
|---|---|---|---|
| v1 | 2026-10-12 | branch w24r (lane w24r) | first package: stock recompiled game, RT64 window, live input |

## Zelda64Recomp (studied, not built)

`https://github.com/Zelda64Recomp/Zelda64Recomp` dev branch, shallow clone at
`~/rush2049/recomp/src/Zelda64Recomp` (reference for main.cpp wiring, register_overlays.cpp,
the RecompiledFuncs CMake target and us.rev1.toml). Phase 1-3 used only the null renderer; Phase 4
builds RT64 (above) as `RUSH_RENDERER=rt64`.
