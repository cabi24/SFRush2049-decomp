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
The direct command below assumes that blob is already present and current:

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

### Replacing the builder again

For `pipeline/promote.py` and `pipeline/romtruth.py`, set `CONVEYOR_BUILDER`
and `CONVEYOR_BUILDER_REPO` (defaults `watchman2`, `~/rush2049/repo`).
The blob modules still have their own builder/toolkit constants; inspect those
when replacing the host rather than assuming the environment variables cover them.
