#!/bin/sh
# Fidelity check of the w5d uopt on the builder, on a snapshot of the whole-program unit's merged ucode.
# usage: run_fidelity.sh [LABEL]   (LABEL = an existing snapshot st_LABEL; default: snapshot the current w5d unit stage)
# Compares (1) stock toolkit uopt, (2) w5d uopt with no tracing, (3) w5d uopt with listing + every trace on
# (-l, W5D_LEVEL=3, W5D_OUT, CDX_LOG) -- all three `opt` outputs must be byte-identical.
lab=${1:-fid}
ssh watchman2 "set -e; cd ~/rush2049/scratch/frontier/w7d; T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
[ -d st_$lab ] || { mkdir st_$lab; cp ../unit/w5d/stage/merged ../unit/w5d/stage/st st_$lab/; }
cd st_$lab
U='-G 0 -Olimit 5000 -mips2 -EB -g0 -O3'
cp st st.run; \$T/uopt \$U merged opt.stock -t st.run optlog >/dev/null 2>&1
cp st st.run; ../uopt/uopt \$U merged opt.off -t st.run optlog >/dev/null 2>&1
cp st st.run; W5D_LEVEL=3 W5D_OUT=\$PWD/fid.w5d CDX_LOG=1 CDX_OUT=\$PWD/fid.cdx ../uopt/uopt \$U merged opt.on -t st.run -l fid.list >/dev/null 2>&1
sha1sum opt.stock opt.off opt.on; ls -la opt.stock
cmp opt.stock opt.off && echo 'IDENTICAL stock vs w5d tracing off'
cmp opt.stock opt.on && echo 'IDENTICAL stock vs w5d tracing on (all procs, level 3)'
wc -l fid.w5d fid.cdx | head -2; ls -la fid.list"
