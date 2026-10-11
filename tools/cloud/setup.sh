#!/bin/bash
# Set up a cloud/CI box to compile and score Rush 2049 game functions without
# the ROM or the LAN pipeline. Needs: a Git checkout, x86-64 Linux, curl,
# sha256sum, python3. Optional:
# MIPS binutils for disassembled diffs (Debian/Ubuntu:
# apt-get install -y binutils-mips-linux-gnu).
#
# Installs IDO 5.3 (static recompilation, decompals/ido-static-recomp v1.2) into
# tools/cloud/ido/ and IDO 7.1 into tools/cloud/ido71/ (both git-ignored). The
# same compilers the pipeline uses (rom_compilers.json pins the 7.1 segments).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
if ! command -v mips-linux-gnu-objdump >/dev/null; then
  echo "note: no mips-linux-gnu-objdump; diffs will show hex only" >&2
  echo "      (Debian/Ubuntu: sudo apt-get install -y binutils-mips-linux-gnu)" >&2
fi
if [ ! -x "$HERE/ido/cc" ]; then
  mkdir -p "$HERE/ido"
  curl -sSL -o "$HERE/ido.tgz" \
    https://github.com/decompals/ido-static-recomp/releases/download/v1.2/ido-5.3-recomp-linux.tar.gz
  # Verify the pinned v1.2 archive before extracting any compiler files.
  (
    cd "$HERE"
    echo "ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506  ido.tgz" | sha256sum -c -
  )
  # Cloud containers may run as root without permission to restore the
  # archive's builder UID/GID. Compiler files should belong to this user.
  tar --no-same-owner -xzf "$HERE/ido.tgz" -C "$HERE/ido"
  rm -f "$HERE/ido.tgz"
fi
# IDO 7.1 (same release) for the Rush boot-tail ROM TUs pinned in
# rom_compilers.json; the installer verifies the pinned archive sha256.
# (The installer lives in tools/conveyor; a checkout without it, such as the
# setup-script test fixture, gets 5.3 only.)
git -C "$HERE/../.." config --local core.hooksPath .githooks
echo "IDO 5.3 ready: $HERE/ido/cc"
if [ -f "$HERE/../conveyor/compilers.py" ]; then
  if [ ! -x "$HERE/ido71/cc" ]; then
    (cd "$HERE/../.." && python3 -m tools.conveyor.compilers install ido7.1 --dest "$HERE/ido71")
  fi
  echo "IDO 7.1 ready: $HERE/ido71/cc"
fi
