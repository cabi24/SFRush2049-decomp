#!/bin/bash
# padcheck.sh GROUPC LABEL: A820 listing from the group -> prepend .data with mapPos at section offset 0x7FF8 ->
# stock as0/as1 -> compare against retail words (relocation fields masked).
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36x2 || exit 1
export TAG=w36x2; T=../tools/trace
$T/snap.sh $2 $1 func_8038E5DC >/dev/null || exit 1
$T/as1t.sh $2 func_8038A820 >/dev/null 2>&1 || exit 1
L=/home/cburnes/projects/rush2049-decomp/build/trace/w36x2; S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/w36x2
for PAD in 0x7FF8 0x0; do
{ printf '\t.data\n\t.space %s\nmapPos:\t.word 0xC2C80000, 0x41F00000, 0x43480000\n' $PAD; cat $L/fn_$2.func_8038A820.s; } > $S/pc_$2.s
scp -q $S/pc_$2.s watchman2:rush2049/scratch/frontier/w36x2/astest/pc.s
ssh watchman2 'cd ~/rush2049/scratch/frontier/w36x2/astest && I=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && $I/as0 -G 0 -mips2 -EB -g0 -O3 pc.s -o pc.b -t pc.st >/dev/null 2>&1 && $I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 pc.b -o pc.o -t pc.st >/dev/null 2>&1 && mips-linux-gnu-objcopy -O binary -j .text pc.o pc.bin'
scp -q watchman2:rush2049/scratch/frontier/w36x2/astest/pc.bin $S/pc.bin
python3 - $S/pc.bin $PAD <<'PY'
import struct,re,sys
b=open(sys.argv[1],'rb').read(); got=struct.unpack('>%dI'%(len(b)//4), b)
want=[]; p=False
for l in open('/home/cburnes/projects/rush2049-decomp/asm/us/ovl_a/ovl_a_8038a400.s'):
    if l.startswith('func_8038A820:'): p=True; continue
    if p and l.startswith('.section'): break
    m=re.match(r'\s*\.word 0x([0-9A-F]{8})',l)
    if p and m: want.append(int(m.group(1),16))
d=0
for a,c in zip(want,got):
    if a==c: continue
    oa,oc=a>>26,c>>26
    if oa==oc==0x0f and ((a>>16)&31)==((c>>16)&31): continue
    if oa==oc and oa in (0x20,0x21,0x23,0x24,0x25,0x28,0x29,0x2b,0x31,0x39,0x09) and (a>>16)==(c>>16): continue
    if oa==oc==3: continue
    d+=1
print('pad %s: want %d words, got %d words, %d differ (relocation fields masked)'%(sys.argv[2],len(want),len(got),d+abs(len(want)-len(got))))
PY
done
