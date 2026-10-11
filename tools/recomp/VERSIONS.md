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

## N64ModernRuntime (headers only in Phase 0)

- `https://github.com/N64Recomp/N64ModernRuntime` at `cdf5abbd5026fef5c364c676e4667c45e42b6863`
  (2026-08-30, "Add CLI options to select games and game modes. (#153)"), cloned recursively at
  `~/rush2049/recomp/src/N64ModernRuntime`; not built yet (Phase 1)
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
`manifest.json` `75cac2ea...` (before the README/doc commits; regenerate to compare).
