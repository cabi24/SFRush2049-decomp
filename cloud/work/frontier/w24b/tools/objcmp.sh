#!/bin/sh
# w24b (builder, scratch root): compile SRC with 5.3 (or $CC) at FLAGS and compare each function's
# objdump -dr rows (words + reloc type/symbol) with tgt/<fn>.o.  usage: sh objcmp.sh SRC "FLAGS" fn...
CC=${CC:-$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc}
src=$1; fl=$2; shift 2; d=$(mktemp -d)
$CC -c $fl -I include -I include/PR -I src/rom -Xcpluscomm -o $d/m.o $src || exit 1
rows() { mips-linux-gnu-objdump -dr -j .text $1 | awk -v f="<$2>:" '$2==f{on=1;next} /^[0-9a-f]+ <.*>:$/{on=0} on' | sed -n 's/^ *[0-9a-f]*:\s*//p' | awk -F'\t' '/R_MIPS/{print $1; next}{print $1}' | sed 's/ *$//'; }
for fn; do rows tgt/$fn.o $fn > $d/t; rows $d/m.o $fn > $d/m
  for x in t m; do awk '{a[NR]=$0} END{n=NR; while(n>0 && a[n]=="00000000") n--; for(i=1;i<=n;i++) print a[i]}' $d/$x > $d/$x.s; mv $d/$x.s $d/$x; done
  if cmp -s $d/t $d/m; then echo "$fn: IDENTICAL ($(wc -l < $d/t) rows)"; else echo "$fn: DIFF $(diff $d/t $d/m | grep -c '^[<>]') ($(wc -l <$d/t) vs $(wc -l <$d/m))"; [ -n "$V" ] && diff -y -W 120 $d/t $d/m; fi
done
rm -rf $d
