#!/bin/sh
# sv.sh FILE.c LABEL: unit-score func_80100E58 with FILE (E58 uargs), snapshot it, run the w36m interference oracle
# (uopt_spl2) and print the score line, the cols-piece (sym 15) bb45 decision and its counted interferers.
cd /home/cburnes/projects/rush2049-decomp
export TAG=w36z
F=$1; L=$2
UARGS="$(cat cloud/work/frontier/w33a/f0e58/uargs.txt) --internal rs_DrawEllipseText --internal rs_SetJust $XU" cloud/work/frontier/tools/trace/us.sh func_80100E58 $F 2>&1 | tail -1
cloud/work/frontier/tools/trace/snap.sh $L >/dev/null 2>&1
UOPTB=uopt_spl2 cloud/work/frontier/w36z/tools/oracle.sh $L func_80100E58 "SPLI=1" --summary 2>&1 | sed 's/^/  oracle(stock split): /'
awk '/split lr=[0-9]* sym=15 /{on=1; print "  " $0; next} /split lr=/{on=0} on && /SPLI\] bb=45:/{n=split($0,a," "); s=""; for(i=3;i<=n;i++) if (a[i] ~ /\*/) s=s" "a[i]; print "  counted@45:" s} on && /adj bb=45 /{print "  " $0; getline; print "  " $0}' build/trace/w36z/or_$L.spl
