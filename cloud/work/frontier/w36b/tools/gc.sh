#!/bin/bash
# gc.sh BODY.c OUTDIR : compose callers_deep group with 93B3C body replaced
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36b
mkdir -p $2 && cat g/cd_pre.txt $1 g/cd_post.txt > $2/group.c && cp g/cd_base/group.json $2/
