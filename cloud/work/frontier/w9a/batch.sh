#!/bin/bash
# usage: batch.sh body1.c body2.c ... : score each body as group member; print diff line
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w9a
T=$(mktemp -d)
for b in "$@"; do n=$(basename $b .c); mkdir -p $T/$n; cat $W/pre.c $b $W/post.c > $T/$n/group.c; cp $W/groups/steering_sensitivity/group.json $T/$n/; done
ssh watchman2 'rm -rf ~/rush2049/scratch/frontier/w9a/cand/b; mkdir -p ~/rush2049/scratch/frontier/w9a/cand/b'
tar -C $T -cf - . | ssh watchman2 'tar -C ~/rush2049/scratch/frontier/w9a/cand/b -xf -'
ssh watchman2 'cd ~/rush2049/scratch/frontier/w9a && export IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls cand/b | xargs -P2 -I{} sh -c "echo {} \$(taskset -c 0,1 python3 tools/cloud/score.py group cand/b/{} 2>&1 | grep -E \"words differ|^  MATCH|error\" | head -1)"'
rm -rf $T
