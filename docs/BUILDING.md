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
