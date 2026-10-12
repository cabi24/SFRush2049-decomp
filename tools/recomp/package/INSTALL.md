# INSTALL: building the Rush 2049 recomp test build (@@VERSION@@)

Audience: an operator (a Claude instance in Claude Desktop/Cowork) on the tester's laptop, with the owner
watching. If you cannot run commands on the Windows host yourself, give each command block to the owner to
paste into the stated shell, and ask them to paste back the last 20 lines of output when a step fails.
Do every step in order; each one says what success looks like. Record each step's result (OK / FAILED +
the error lines) for `REPORT_TEMPLATE.md` section 2.

What gets built (same on every OS):

1. `src/N64Recomp` -> the recompiler programs `N64Recomp` (game code) and `RSPRecomp` (the audio microcode).
2. `make_rom.py` + your ROM -> `config/rom/rush2049.recomp.z64` (the "synthetic ROM": your ROM plus the
   game's three compressed code images unpacked; refused unless byte-identical to the coordinator's,
   sha256 `@@SYNTH_SHA256@@`).
3. `N64Recomp config/rush2049.recomp.toml` -> `config/RecompiledFuncs/*.c` (the game as C, 2,135 functions);
   `RSPRecomp config/rush2049.rsp_audio.toml` -> `config/rsp/rush_audio_ucode.cpp` (the game's audio
   microcode as C++: this is what makes sound).
4. RT64 (cloned from GitHub) + `src/N64ModernRuntime` + `rush/tools/recomp/runtime` + the C from step 3 ->
   the game program `rush2049_recomp(.exe)`.

Known limits of this build (not bugs to report, but do note anything surprising): no save/controller-pak
support; the renderer is RT64 in its default settings. **New in v2** (see `CHANGES.md`): the default
(wall-clock) mode no longer hangs at boot, and there is **sound** (the game's own audio microcode,
recompiled). Both were checked by the coordinator only on a machine without a screen or speakers (RT64 on a
software GPU, audio written to a file), so hearing it and seeing it run at full speed is what this test
finds out.

Privacy: the program copies the ROM into its `--home` folder when it starts (that is how the runtime
library checks it) and, since v2, deletes that copy as soon as the game has loaded it (log line
`removed the ROM copy ...`) and again at exit. Still never zip or upload `*.z64` files (T9 says how).

The commands below assume the package was extracted to `C:\rush2049\@@PACKAGE@@` (Windows) or
`~/rush2049/@@PACKAGE@@` (Linux). Adjust the paths if you chose another place; avoid spaces in the path.

---------------------------------------------------------------------------------------------------------

## A. Windows 11 (primary path)

Time: 30-60 min the first time (mostly downloads: ~8 GB for Visual Studio Build Tools, ~1 GB for RT64).

### A1. Install the tools (once; ~20-40 min)

Open **PowerShell** (normal user is fine; winget asks for elevation when it needs it) and run:

```powershell
winget install --id Microsoft.VisualStudio.2022.BuildTools -e --override "--passive --wait --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621 --add Microsoft.VisualStudio.Component.VC.CMake.Project --add Microsoft.VisualStudio.Component.VC.Llvm.Clang --add Microsoft.VisualStudio.Component.VC.Llvm.ClangToolset"
winget install --id Kitware.CMake -e
winget install --id Ninja-build.Ninja -e
winget install --id Git.Git -e
winget install --id Python.Python.3.12 -e
```

That is: Visual Studio 2022 Build Tools with the C++ workload, the Windows 11 SDK, CMake tools and the
**LLVM/clang-cl** toolset (the same toolchain Zelda64Recomp uses on Windows), plus CMake, Ninja, Git and
Python 3. If the owner prefers the Visual Studio Installer GUI: install "Desktop development with C++" and tick
"C++ Clang Compiler for Windows" and "C++ CMake tools for Windows". (Alternative for clang-cl:
`winget install --id LLVM.LLVM -e`, then use `C:\Program Files\LLVM\bin\clang-cl.exe` below.)

Close PowerShell and open a new one so the new PATH applies. Check:

```powershell
git --version; cmake --version; ninja --version; python --version
```

Success: four version lines (CMake >= 3.20, Python >= 3.9).

### A2. Open the build shell (every time you build)

Every build command below runs in a **Visual Studio x64 developer PowerShell**. In a normal PowerShell:

```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -HostArch amd64
cd C:\rush2049\@@PACKAGE@@
$CLANGCL = "$($env:VCINSTALLDIR)Tools\Llvm\x64\bin\clang-cl.exe"
& $CLANGCL --version
```

Success: `clang version 1x.x ... Target: x86_64-pc-windows-msvc`. (If `Launch-VsDevShell.ps1` is not there,
use the Start menu entry "Developer PowerShell for VS 2022" and run the `cd` and `$CLANGCL` lines.)

### A3. Put the ROM in place and check it (~1 min)

Copy the US ROM to `C:\rush2049\@@PACKAGE@@\rom\baserom.us.z64` (skip if the package already contains
`rom\baserom.us.z64`), then:

```powershell
(Get-FileHash -Algorithm SHA1 rom\baserom.us.z64).Hash
```

Success: `3F99351D7BB61656614BDB2AA1A90CFE55D1922C`. Any other value: the file is not the US `.z64`
cartridge image (a `.n64`/`.v64` byte-swapped dump must be converted to `.z64` first); stop and report.

### A4. Verify the package (~10 s)

```powershell
python -c "import hashlib,sys;bad=[l.split('  ',1)[1] for l in open('MANIFEST.sha256') if hashlib.sha256(open(l.split('  ',1)[1].strip(),'rb').read()).hexdigest()!=l.split('  ')[0]];print('MANIFEST OK' if not bad else bad[:5])"
```

Success: `MANIFEST OK`.

### A5. Build N64Recomp and RSPRecomp (~3-6 min)

```powershell
cmake -S src\N64Recomp -B build\N64Recomp -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER="$CLANGCL" -DCMAKE_CXX_COMPILER="$CLANGCL"
cmake --build build\N64Recomp --target N64RecompCLI RSPRecomp
```

Success: `build\N64Recomp\N64Recomp.exe` and `build\N64Recomp\RSPRecomp.exe` exist. (The CMake target of the
recompiler program is `N64RecompCLI`; its output is named `N64Recomp.exe`. The target `N64Recomp` is only a
library: the v1 report found this.)

### A6. Make the synthetic ROM and recompile (~1 min)

```powershell
python rush\tools\recomp\make_rom.py --rom rom\baserom.us.z64 --manifest config\manifest.json --out config\rom\rush2049.recomp.z64
cd config
..\build\N64Recomp\N64Recomp.exe rush2049.recomp.toml
..\build\N64Recomp\RSPRecomp.exe rush2049.rsp_audio.toml
cd ..
```

Success: `make_rom: wrote ... = manifest synthetic ROM`, then N64Recomp prints `Function count: 2135` and exits
without an error; `config\RecompiledFuncs\` holds ~45 `funcs_*.c` files plus `recomp_overlays.inl` and
`lookup.cpp`; RSPRecomp prints nothing and writes `config\rsp\rush_audio_ucode.cpp` (~2,600 lines).

### A7. Get RT64 at the pinned commit (~3-10 min, ~1 GB)

```powershell
git clone @@RT64_URL@@ src\rt64
git -C src\rt64 checkout @@RT64_COMMIT@@
git -C src\rt64 submodule update --init --recursive
git -C src\rt64 rev-parse HEAD
```

Success: the last line is `@@RT64_COMMIT@@`.

### A8. Configure and build the game program (~5-20 min)

```powershell
cmake -S rush\tools\recomp\runtime -B build\host -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER="$CLANGCL" -DCMAKE_CXX_COMPILER="$CLANGCL" -DN64MODERNRUNTIME_DIR="$PWD\src\N64ModernRuntime" -DRECOMP_OUT="$PWD\config" -DRUSH_RENDERER=rt64 -DRT64_DIR="$PWD\src\rt64"
cmake --build build\host
```

The configure step prints `Rush 2049 host: renderer rt64, capture OFF` (per-call capture is POSIX-only and is
switched off on Windows; nothing in the gameplay test needs it) and `Rush 2049 host: recompiled audio ucode`
(if it says `no audio ucode`, the RSPRecomp step of A6 did not run: run it, delete `build\host`, configure again). It also runs a Python check of the runtime
contract (a failure there names the missing function: report it). The build compiles RT64, its shaders,
N64ModernRuntime, ~45 large generated C files and the host (~500 steps).

Success: `build\host\rush2049_recomp.exe`, with `SDL2.dll`, `dxil.dll` and `dxcompiler.dll` copied next to it.

If the build fails: copy the first `error:` line and the 10 lines around it into the report. Known places
where a Windows build could differ from the coordinator's Linux build (this package's Windows path is
written from Zelda64Recomp's Windows procedure and a header-level check of our sources; it has not been
built on Windows by the coordinator): linker errors naming `__wrap_os...` or `osGetCount_recomp` (the
call-renaming in `runtime/CMakeLists.txt` RUSH_WRAPPED), anything in `rt64_renderer.cpp` /
`sdl_frontend.cpp` (the Windows window handle), RT64 `-Werror` warnings (we pass `-Wno-error` to RT64).
A null-renderer build (`-DRUSH_RENDERER=null`, no RT64) is the fallback that tells us whether the
recompiled game itself builds on Windows: try it in `build\host-null` and report both results.

### A9. First run: headless smoke test (~40 s)

```powershell
mkdir run\logs -Force | Out-Null
cmd /c "build\host\rush2049_recomp.exe --renderer null --rom config\rom\rush2049.recomp.z64 --home run\home --seconds 30 2> run\logs\smoke.txt"
Select-String -Path run\logs\smoke.txt -Pattern "load game|load ovl|audio ucode|non-silent|removed the ROM|pak:|final"
```

(Since v3 the program plugs a controller pak into controller 1, saved as `run\home\rush2049.mpk`, so there is
no boot notice any more: the game shows its title and starts an attract-mode demo race by itself. `--no-pak`
gives the old boot notice "NO CONTROLLER PAK DETECTED", which waits for START.)

Success: lines `pak: controller 1, new run\home\rush2049.mpk (0 notes used, 123 free pages)` (`loaded` on a
second run), `pak: osPfsInitPak(channel 0) -> ok`, `load game: stream ROM 0xB0CB10 -> 0x80086A50`,
`load ovl_a: ...`, `removed the ROM copy ...`, `audio ucode: recompiled`, `first non-silent audio buffer`
(about 3 s in: the title), `load ovl_b: ...` (about 8 s: the demo race) and a `final t=30...` status line with
`gfx_tasks=` around 900, `audio_nonzero=` well above 0 and `image=ovl_b`. This is the same check the coordinator runs on Linux;
continue with `TEST_PROCESS.md`.

### A10. The windowed game

```powershell
cmd /c "build\host\rush2049_recomp.exe --rom config\rom\rush2049.recomp.z64 --home run\home 2> run\logs\play.txt"
```

A window opens; close it (or press Escape) to quit. This is the default **wall-clock** mode (the game runs on
real time, like the console). Options: `--gfx-api d3d12|vulkan` (default: RT64 picks, normally D3D12 on
Windows), `--fullscreen`, `--rt64-dev` (RT64's developer overlay), `--volume PERCENT` (default 100),
`--no-sound`, `--log-input` (prints every button press: the pad check in TEST_PROCESS). The log file collects a
status line per second.

Fallback: if the window stays black or the picture freezes in this mode, add `--clock frames` (the
deterministic "lockstep" mode the v1 test played in; it has sound too since v2) and report the wall-clock
failure (the last 40 log lines).

---------------------------------------------------------------------------------------------------------

## B. Linux (secondary path; how the coordinator builds it)

Tested by the coordinator on Rocky Linux 10 (clang 21, CMake 3.31, Ninja 1.11): headless runs, and since v2
the RT64 window on a virtual display (Xvfb) with Mesa's software Vulkan (lavapipe) and SDL's file audio
driver; never on a real GPU, screen or speakers. Packages (Fedora/Rocky; Ubuntu names in brackets):

```bash
sudo dnf install -y clang lld cmake ninja-build git python3 SDL2-devel gtk3-devel
# Ubuntu/Debian: sudo apt-get install clang lld cmake ninja-build git python3 libsdl2-dev libgtk-3-dev
```

Quick start (no compiler needed; only for x86_64 Linux with glibc >= 2.39, SDL2 and GTK3 libraries
installed): `prebuilt/linux-x86_64/rush2049_recomp` (sha256 `@@BINARY_SHA256@@`) is the coordinator's
RT64-enabled build. It still needs the synthetic ROM:

```bash
cd ~/rush2049/@@PACKAGE@@
python3 rush/tools/recomp/make_rom.py --rom rom/baserom.us.z64 --manifest config/manifest.json \
        --out config/rom/rush2049.recomp.z64
prebuilt/linux-x86_64/rush2049_recomp --renderer null --rom config/rom/rush2049.recomp.z64 --home run/home --seconds 30
```

Full build (~10 min on 8 cores):

```bash
cd ~/rush2049/@@PACKAGE@@
sha1sum rom/baserom.us.z64          # 3f99351d7bb61656614bdb2aa1a90cfe55d1922c
sha256sum -c MANIFEST.sha256 --quiet && echo MANIFEST OK
CC=clang CXX=clang++ cmake -S src/N64Recomp -B build/N64Recomp -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C build/N64Recomp N64RecompCLI RSPRecomp
python3 rush/tools/recomp/make_rom.py --rom rom/baserom.us.z64 --manifest config/manifest.json \
        --out config/rom/rush2049.recomp.z64
(cd config && ../build/N64Recomp/N64Recomp rush2049.recomp.toml && ../build/N64Recomp/RSPRecomp rush2049.rsp_audio.toml)
git clone @@RT64_URL@@ src/rt64 && git -C src/rt64 checkout @@RT64_COMMIT@@ \
    && git -C src/rt64 submodule update --init --recursive
CC=clang CXX=clang++ cmake -S rush/tools/recomp/runtime -B build/host -G Ninja \
    -DN64MODERNRUNTIME_DIR=$PWD/src/N64ModernRuntime -DRECOMP_OUT=$PWD/config \
    -DRUSH_RENDERER=rt64 -DRT64_DIR=$PWD/src/rt64
ninja -C build/host
build/host/rush2049_recomp --renderer null --rom config/rom/rush2049.recomp.z64 --home run/home --seconds 30
build/host/rush2049_recomp --rom config/rom/rush2049.recomp.z64 --home run/home 2> run/play.txt   # window
```

Headless only (no SDL2/GTK/RT64): leave out `-DRUSH_RENDERER=rt64 -DRT64_DIR=...` (default `null`).
Linux keeps per-call capture (`RUSH_CAPTURE=ON`).

---------------------------------------------------------------------------------------------------------

## C. WSL2 (fallback only)

Use only if both native Windows builds fail. Inside WSL2 Ubuntu follow section B. The window appears through
WSLg; RT64 then runs on Vulkan through Mesa's `dozen` (Vulkan on D3D12) or llvmpipe (CPU), which RT64 may
not support or may run at a few frames per second. A WSL2 result therefore says little about GPU rendering
or speed: say "WSL2" clearly in the report. The headless smoke test (A9 equivalent) is still meaningful.

---------------------------------------------------------------------------------------------------------

## Troubleshooting

| Symptom | What to do |
|---|---|
| `make_rom: ROM SHA-1 ... is not the US retail ROM` | wrong dump/region or byte order; stop and report the SHA-1 |
| configure: `set -DN64MODERNRUNTIME_DIR` / `set -DRECOMP_OUT` | paths must be absolute; step A6 must have produced `config\RecompiledFuncs\recomp_overlays.inl` |
| configure: `runtime contract check failed` | report the names it lists |
| `use clang-cl ..., not MSVC cl.exe` | pass `-DCMAKE_C_COMPILER` / `-DCMAKE_CXX_COMPILER` as in A8 (delete `build\host` first) |
| `[rush] RT64 setup: FAILED` in the log | retry with `--gfx-api vulkan`, then `--gfx-api d3d12`; update the GPU driver; report the GPU model |
| window opens but stays black | let it run 60 s (the front end needs ~5 s); try the other `--gfx-api`; then `--clock frames`; report all three |
| black window, log `gfx_tasks=` stops growing | the v1 hang; v2 should not have it. Log lines `busy-wait at 0x...: yielding` are normal (the fix working). Report the last 40 lines; play with `--clock frames` |
| no sound | log line `audio: ...` names the SDL driver/device (or why none opened); `audio_nonzero=` in the status lines says whether the game produced sound at all; check the Windows output device; report both |
| crackling / sound too fast or slow | report it with the `audio:` line and the gfx_tasks rate; `--wav run\logs\sound.wav` writes what the game produced (private: it is the game's audio, do not upload; describe it) |
| crash | the last 40 lines of the log file + Windows Event Viewer > Windows Logs > Application entry for `rush2049_recomp.exe` |
