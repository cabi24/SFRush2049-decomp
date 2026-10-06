#!/bin/bash
# iub.sh body... : ub.sh for input_deadzone_apply with the ipcorder header/part
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11b
HDR=input_deadzone_apply/hdr_ipcorder.h PART=input_deadzone_apply/w1g_part_ipcorder.c $W/tools/ub.sh input_deadzone_apply "$@"
