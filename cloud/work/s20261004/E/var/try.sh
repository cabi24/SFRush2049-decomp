#!/bin/sh
# try.sh VARIANT : splice var/VARIANT.c into group3 as slot_state_setup
cd /home/cburnes/projects/rush2049-decomp/cloud/work/s20261004/E
V=$1; { sed -n 1,138p group3.c; cat var/$V.c; sed -n '170,$p' group3.c; } > g_$V.c
./run.sh g_$V keep3.txt slot_state_setup,func_80100B8C | head -3
./dis.sh g_$V slot_state_setup | head -12 | tail -6
