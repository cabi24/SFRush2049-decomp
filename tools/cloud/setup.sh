#!/bin/bash
# Set up a cloud/CI box to compile and score Rush 2049 game functions without
# the ROM or the LAN pipeline. Needs: x86-64 Linux, curl, python3. Optional:
# MIPS binutils for disassembled diffs (Debian/Ubuntu:
# apt-get install -y binutils-mips-linux-gnu).
#
# Installs IDO 5.3 (static recompilation, decompals/ido-static-recomp v1.2) into
# tools/cloud/ido/ (git-ignored). Same compiler the pipeline uses.
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
  tar xzf "$HERE/ido.tgz" -C "$HERE/ido"
  rm -f "$HERE/ido.tgz"
fi
echo "IDO 5.3 ready: $HERE/ido/cc"
