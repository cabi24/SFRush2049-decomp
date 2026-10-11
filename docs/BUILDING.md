# Build environment and remote builder

Read [CLAUDE.md](../CLAUDE.md) for matching rules and the
[promotion skill](../.claude/skills/promote-match/SKILL.md) for static versus
compressed-game build paths. This guide owns the documented host setup and sync
rules, extracted from the local `CODEX.md` primer on 2026-09-28. Check service
status before depending on the builder; the recorded setup is not a liveness check.

## watchman2 builder

`watchman2` replaced the original builder in July 2026; the recovery is recorded
in [project history](history/project-milestones.md#july-28-replacement-builder).

`watchman2` is an x86-64 machine on the LAN reachable over **Tailscale**
(MagicDNS name `watchman2`, `100.100.198.109`, LAN `192.168.50.9`; no entry
needed in `~/.ssh/config`). 16 cores, RTX 3090, 4 KB pages — so IDO runs there
but not on this Pi (16 KB pages). It is the `--capabilities x86_64,builder`
node for the conveyor pipeline.

**This box is shared with GPU/LLM workloads.** Everything of ours lives under
one root and the agent is capped and niced so inference work wins:

```
~/rush2049/
├── repo/     git working tree (the builder's repo; keep it CLEAN)
├── agent/    node_agent.py
├── cache/    toolkit cache (content-addressed)
├── tmp/      job scratch — TMPDIR override, keeps /tmp free of conveyor-* dirs
├── logs/
└── .env      CONVEYOR_TOKEN (mode 600)
```

### Services (both survive reboot — install once, never hand-start)

| Unit | Host | Command |
|---|---|---|
| `conveyor-coordinator.service` | Pi | `python3 -m tools.conveyor.cli serve` (0.0.0.0:8323) |
| `conveyor-node.service` | watchman2 | agent, `--cores 12`, `Nice=10`, `CPUWeight=50` |

```bash
systemctl status conveyor-coordinator          # on the Pi
ssh watchman2 'systemctl status conveyor-node' # on the builder
ssh watchman2 'journalctl -u conveyor-node -f'
```

Hand-started daemons are how the farm's ingest loop silently died four days
before watchman did, unnoticed. Use the units.

### Test connectivity

```bash
tailscale status | grep watchman2
tailscale ping -c 2 watchman2
ssh -o ConnectTimeout=5 -o BatchMode=yes watchman2 'echo OK; hostname; uptime'
```

`tailscale status` can show a node as `active` even when it is unreachable —
that field is the last successful handshake, not current liveness. Confirm
with ping or a port check.

### Building on watchman2

For the complete image → compressed blob → cartridge workflow, run
`python3 -m tools.conveyor.pipeline.blob_rom rom` on the Pi. It produces and
syncs the required `build/blob/game_code.deflate` before the builder runs Make.
Every ROM build also composes the runtime-image streams `build/ovl/ovl_{a,b}.deflate`
(`ovl_layout build` then `ovl_rom produce`; `ovl_rom rom` does both sync and build);
`blob_rom rom` and `ovl_rom rom` refuse when any of the three is absent.
The direct command below assumes those files are already present and current:

```bash
ssh watchman2
cd ~/rush2049/repo
make VERSION=us COMPILER=ido -j16     # full matching ROM; ends in "ROM matches!"
```

Sync the repo before building (the builder tree must be clean — `promote`
refuses a dirty tree):

```bash
rsync -a --exclude='venv/' --exclude='build/' --exclude='reference/repos/' \
  --exclude='tools/ido-static-recomp/' --exclude='__pycache__/' --exclude='*.pyc' \
  --exclude='.pytest_cache/' --exclude='backup/' \
  /home/cburnes/projects/rush2049-decomp/ watchman2:~/rush2049/repo/
rsync -a --delete /home/cburnes/projects/rush2049-decomp/asm/ watchman2:~/rush2049/repo/asm/
```

The second command mirrors `asm/` exactly. The first never deletes, so when a
segment is converted to a C TU its old `asm/us/<SEG>.s` survives on the builder.
The Makefile links `asm/us/*.s` by wildcard, so that leftover fails the link
with `multiple definition` errors (seen 2026-10-04 for 21F0/2CF0/3140/3330/3390/34A0).

`tools/ido-static-recomp/` is excluded deliberately: the Pi's copy is an
**aarch64** build that cannot run. The builder's x86-64 IDO was installed from
the pinned toolkit blob and must not be overwritten by a sync:

```bash
tar xzf <blob> -C ~/rush2049/repo/tools/ido-static-recomp/build/out \
    --strip-components=1 ido
```

### IDO 7.1 (second compiler, boot-tail ROM TUs)

The 18 Rush boot-tail ROM TUs (`lib_11050` ... `lib_26ab0`) build with IDO 7.1;
everything else (libultra, libc, all other TUs) stays on IDO 5.3. The pins live in
`rom_compilers.json`; `pipeline.layout` turns them into `CC := $(IDO71_CC)` lines
in `src/rom/opt_overrides.mk`. Evidence and segment list:
[compiler settings](COMPILER_SETTINGS.md#ido-71-boot-tail-segments).

Vendoring follows the 5.3 pattern: the binaries are **not committed**
(`tools/ido-static-recomp/` and `tools/cloud/ido*/` are ignored). The
decompals ido-static-recomp v1.2 release archive is pinned by sha256 in
`tools/conveyor/compilers.py` (`ido-7.1-recomp-linux.tar.gz`,
`0d411696e178fcca34c31c3bf02011b928d7fd9c1fa7f8bf45070e0781b58e15`), and the
installer verifies the archive before extracting and checks the unpacked
binaries' fingerprints:

```bash
# builder, once (into tools/ido-static-recomp/build71/out = the Makefile's IDO71_ROOT)
ssh watchman2 'cd ~/rush2049/repo && python3 -m tools.conveyor.compilers install ido7.1'
python3 -m tools.conveyor.compilers identify tools/ido-static-recomp/build71/out   # -> ido7.1
```

The repo sync above excludes `tools/ido-static-recomp/`, so the builder's 7.1
survives syncs like its 5.3. The binaries are x86-64 recompilations that need
4 KB pages: they run on watchman2 and CI, not on the Pi (the Pi may download and
fingerprint them, which is how the 7.1 toolkit is built, but never runs them). A
build whose selected compiler is missing stops with
`error: compiler ... is not installed`; it never falls back to 5.3.

CI builds no ROM (no baserom) and its tests need no IDO 7.1. A cloud box that
wants to score boot-tail functions installs it beside the 5.3 copy:
`python3 -m tools.conveyor.compilers install ido7.1 --dest tools/cloud/ido71`.

**Toolkit.** The pool scores with toolkit bundles. The 7.1 toolkit is the pinned
5.3 toolkit with `ido/` swapped, built deterministically from the same blob
(reproduced on the Pi and watchman2: `4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557`
from `d4c39cbc…`). It is uploaded, **not pinned**: jobs keep the 5.3 default, and
`lock add --toolkit <sha>` selects 7.1 for one proof and records
`"compiler": "ido7.1"` in the lock entry.

```bash
python3 -m tools.conveyor.compilers install ido7.1 --dest /tmp/ido71
python3 -m tools.conveyor.bundles.build_toolkit --swap-ido-from ~/.conveyor/blobs/<pinned-5.3-sha> \
    --ido /tmp/ido71 --out /tmp/tk71.tar.gz        # prints sha256 + "compiler: ido7.1"
python3 -m tools.conveyor.cli publish-toolkit /tmp/tk71.tar.gz --no-pin
```

### Replacing the builder again

For `pipeline/promote.py` and `pipeline/romtruth.py`, set `CONVEYOR_BUILDER`
and `CONVEYOR_BUILDER_REPO` (defaults `watchman2`, `~/rush2049/repo`).
The blob modules still have their own builder/toolkit constants; inspect those
when replacing the host rather than assuming the environment variables cover them.

## CI-equivalent test hosts

CI (`.github/workflows/verify.yml`) runs `tests/conveyor` and `tests/cloud` with
`REQUIRE_TOOLCHAIN=1 -m 'not node_required'` on an x86-64 box with IDO 5.3 and
7.1 and MIPS binutils. The Pi cannot run IDO, so before pushing, the same two
suites run on an x86-64 host from a bundle of HEAD **plus all remote branches**
(some research packets `git show` commits that exist only on old unmerged
branches). `tools/ci_equiv.sh` does the whole sequence from the Pi:

```bash
git bundle create /tmp/X.bundle HEAD --remotes=origin
tools/ci_equiv.sh watchman2 /tmp/X.bundle     # or: tools/ci_equiv.sh rocky /tmp/X.bundle
CI_FAST=8 tools/ci_equiv.sh rocky /tmp/X.bundle   # parallel (make test-fast), see below
```

It copies the bundle to `~/rush2049/scratch/ci-bundles/`, resets the host's
disposable clone (`git checkout -q . && git clean -qfd cloud tests`), fetches the
bundle's HEAD and remote branches, checks HEAD out, confirms
`tools/cloud/ido/cc` and `tools/cloud/ido71/cc`, runs both suites, and prints
the tested HEAD and one pytest summary line per suite (full logs stay on the
host in `~/rush2049/scratch/ci-logs/`). Exit status is non-zero if either suite
failed. Per-host settings live in `~/rush2049/scratch/ci.env` on the host
(`CI_CLONE`, `CI_VENV`, `CI_PATH`, `CI_LOGS`, plus any exports); without one,
the clone is `~/rush2049/scratch/ciclone6` and the venv `~/rush2049/scratch/civenv`.
`CI_CLONE`, `CI_VENV`, `CI_LOGS` and `CI_BUNDLES` (bundle directory, relative
to the host's home) set in the caller's environment override them for one run.

**watchman2** uses the defaults (`ciclone6`, a copy of `~/rush2049/scratch/ci`
with its submodule checkouts; `civenv`). It is the coordinator's clone; a lane
that wants its own copies it (`cp -a ciclone6 ciclone-<lane>`) and passes
`CI_CLONE` (and `CI_LOGS`/`CI_BUNDLES` under its own scratch directory).
`civenv` has no pytest-xdist; `CI_FAST` there needs
`~/rush2049/scratch/civenv/bin/pip install pytest-xdist==3.8.0` (or another
venv via `CI_VENV`).

### Second test host: rocky

`rocky` (x86-64, 20 cores, 38 GB, Rocky Linux 10, 4 KB pages; `ssh rocky`) is a
second CI-equivalent host, set up 2026-10-11 without sudo. Everything lives
under `~/rush2049/` (the `agent/`, `cache/`, `tmp/` there predate it):

```
~/rush2049/scratch/ciclone      clone of a bundle, submodules from GitHub; IDO in tools/cloud/
~/rush2049/scratch/civenv       python3.12 venv: pytest 9.1.1, pytest-xdist, pycparser 3.0, toml, graphviz
~/rush2049/scratch/ci.env       CI_CLONE/CI_PATH/... for tools/ci_equiv.sh
~/rush2049/opt/mips-binutils    GNU binutils 2.45 --target=mips-linux-gnu (built from source)
~/rush2049/opt/sanitizers       Rocky-signed libasan/libubsan RPMs, unpacked
```

Setup (the Pi side makes the bundle and copies it as `~/rush2049/scratch/X.bundle`):

```bash
mkdir -p ~/rush2049/scratch ~/rush2049/opt/src && cd ~/rush2049/scratch
git clone -q X.bundle ciclone && cd ciclone
git fetch -q ../X.bundle HEAD "refs/remotes/origin/*:refs/remotes/origin/*" && git checkout -q FETCH_HEAD
git submodule update --init                      # tools/mips_to_c, tools/decomp-permuter
python3 -m venv ../civenv && ../civenv/bin/pip install -q pytest 'graphviz>=0.20.1,<0.21' pycparser==3.0 toml pytest-xdist
PATH=$HOME/rush2049/scratch/civenv/bin:$PATH bash tools/cloud/setup.sh   # IDO 5.3 + 7.1, both sha256-pinned

# MIPS binutils (no packaged cross binutils on EL10; no texinfo, so MAKEINFO=true)
cd ~/rush2049/opt/src && curl -sSLO https://ftp.gnu.org/gnu/binutils/binutils-2.45.tar.xz
# sha256 c50c0e7f9cb188980e2cc97e4537626b1672441815587f1eab69d2a1bfbef5d2
tar xf binutils-2.45.tar.xz && mkdir build-mips && cd build-mips
../binutils-2.45/configure --target=mips-linux-gnu --prefix=$HOME/rush2049/opt/mips-binutils \
    --disable-nls --disable-werror --disable-gdb --disable-gprofng --disable-sim MAKEINFO=true
make -j16 MAKEINFO=true && make install MAKEINFO=true

# gcc is installed but not its sanitizer runtimes (host tests use -fsanitize=address,undefined)
mkdir -p ~/rush2049/opt/rpms ~/rush2049/opt/sanitizers && cd ~/rush2049/opt/rpms
dnf download --disablerepo=tailscale-stable libasan libubsan
mkdir keydb && rpmkeys --dbpath $PWD/keydb --import /etc/pki/rpm-gpg/RPM-GPG-KEY-Rocky-10
rpmkeys --dbpath $PWD/keydb -K *.rpm            # "digests signatures OK"
cd ../sanitizers && for r in ../rpms/*.rpm; do rpm2cpio $r | cpio -idm --quiet; done
cd usr/lib64 && ln -s libubsan.so.1.0.0 libubsan.so && ln -s libasan.so.8.0.0 libasan.so
```

`~/rush2049/scratch/ci.env` on rocky:

```bash
CI_CLONE=$HOME/rush2049/scratch/ciclone
CI_VENV=$HOME/rush2049/scratch/civenv
CI_PATH=$HOME/rush2049/opt/mips-binutils/bin
CI_LOGS=$HOME/rush2049/scratch/ci-logs
export LIBRARY_PATH=$HOME/rush2049/opt/sanitizers/usr/lib64${LIBRARY_PATH:+:$LIBRARY_PATH}
export LD_LIBRARY_PATH=$HOME/rush2049/opt/sanitizers/usr/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
```

Like watchman2, rocky has no `mips-linux-gnu-gcc`, so `test_ido_syntax.py`
skips there for the same reason. With `sudo dnf install libasan libubsan` the
sanitizer exports become unnecessary.

### Fast test runs (pytest-xdist)

`make test-fast` runs each suite in two parts: every test not marked `serial`
with `pytest -n $(JOBS) --dist loadgroup`, then the `serial` tests alone. The
selection (`-m 'not node_required'`) and `REQUIRE_TOOLCHAIN=1` behave exactly as
in the plain run; it needs `pytest-xdist` in the venv.

```bash
REQUIRE_TOOLCHAIN=1 make test-fast JOBS=4 PYTEST=~/rush2049/scratch/civenv/bin/python
make test-fast SUITES=tests/cloud JOBS=16     # one suite
```

Each test file is one xdist group, so its tests run in order on one worker
with their module fixtures, as in a serial run. Files that write a shared,
non-unique path are grouped together in `tests/conftest.py`
(`CONCURRENCY_GROUPS`). Today every such group comes from IDO `-O3`: its
`ujoin` step writes `<source basename>.u` into the current directory (the
repository root). `tools/test_write_audit.py tests/<suite>` (strace) finds such
files again and exits 1 when `CONCURRENCY_GROUPS` misses one; re-run it after
adding a test that compiles at `-O3` from the repository root. `-n` without
`--dist loadgroup` is refused, and a `serial` test selected under xdist fails.
