#!/bin/bash
# ge.sh BODY.c OUTDIR : compose base2 (installed w36a) group with E444 body replaced
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36b
mkdir -p $2 && cat g/b2e_pre.txt $1 g/b2e_post.txt > $2/group.c && cp g/base2/group.json $2/
