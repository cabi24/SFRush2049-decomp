#!/bin/bash
# usage: tr.sh stage GROUPDIR LABEL | cdx LABEL NAME | force LABEL NAME SPEC | ord LABEL NAME
# Runs the trace toolkit on the builder against a staged group (st_LABEL under the w35f scratch).
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35f
SCR=rush2049/scratch/frontier/w35f
IDO=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
case $1 in
stage) D=$2; L=$3
  rsync -aq $W/tools/stage.py watchman2:$SCR/tools/stage.py
  rsync -aq --delete $D/ watchman2:$SCR/cand/$(basename $D)/
  ssh watchman2 "cd ~/$SCR && IDO_DIR=$IDO python3 tools/stage.py cand/$(basename $D) $L" ;;
cdx) L=$2; N=$3; shift 3
  scp -q /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/remote/tk.sh watchman2:$SCR/tk/tk.sh
  ssh watchman2 "sh ~/$SCR/tk/tk.sh ~/$SCR $L cdx $N $*" ;;
ord) ssh watchman2 "sh ~/$SCR/tk/tk.sh ~/$SCR $2 ord $3" ;;
force) L=$2; N=$3; SPEC=$4
  scp -q /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/remote/tk.sh watchman2:$SCR/tk/tk.sh
  ssh watchman2 "sh ~/$SCR/tk/tk.sh ~/$SCR $L force $N '$SPEC'" ;;
fdiff) L=$2; shift 2   # compare st_L/f.o: fdiff LABEL NAMES [--diff NAME ...]
  rsync -aq $W/tools/gscore.py watchman2:$SCR/tools/gscore.py
  ssh watchman2 "cd ~/$SCR && IDO_DIR=$IDO python3 tools/gscore.py obj st_$L/f.o --names $*" ;;
esac
