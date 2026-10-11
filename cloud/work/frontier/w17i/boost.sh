#!/bin/sh
# boost.sh LABEL PROC ORD "ENV=V ..." [udiff args]: run bin/uopt_boost (patch_spl + splitpatch + boostpatch) on snapshot
# st_LABEL with W5D_PROC=PROC, CDX_PROC=ORD, CDX_LOG, plus ENV (e.g. SPL_BOOST=33:0.1 CDX_FORCE=p1:w33=c2), stock ugen+as1,
# then diff PROC against retail. Diagnostic only.
L=$1; P=$2; O=$3; E=$4; shift 4
S=rush2049/scratch/frontier/w17i; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.bo && env W5D_PROC=$P SPL=1 SPLITLOG=$O CDX_PROC=$O CDX_LOG=1 CDX_OUT=\$PWD/bo.cdx $E ../bin/uopt_boost -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.bo -t st.bo optlog 2>bo.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.bo -o opt.bo.gen -l bo.s -t st.bo -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.bo.gen -o bo.o -t st.bo >/dev/null 2>&1 && grep -E 'boost|force_declined|forced=[0-9-]' bo.spl bo.cdx | grep -v 'forced=-2' | head -20; awk '/^\t.ent\t$P /,/^\t.end\t$P\$/' bo.s | grep -nE 'alias|\\\$3' " || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w17i
scp -q watchman2:$S/st_$L/bo.o /home/cburnes/projects/rush2049-decomp/build/trace/w17i/bo_$L.o
cd /home/cburnes/projects/rush2049-decomp && TAG=w17i python3 cloud/work/frontier/tools/trace/udiff.py $P --obj build/trace/w17i/bo_$L.o "$@"
