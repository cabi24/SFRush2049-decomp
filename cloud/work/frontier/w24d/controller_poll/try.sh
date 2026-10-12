#!/bin/sh
# try.sh BODY.c... : build each body with mk.sh and unit-score controller_poll (serial, TAG=w24d)
D=$(cd "$(dirname "$0")" && pwd); R=$(cd "$D/../../../../.." && pwd)
export TAG=${TAG:-w24d}
for b in "$@"; do
  o=$D/v/full_$(basename $b); sh $D/mk.sh $b $o
  (cd $R && cloud/work/frontier/tools/trace/us.sh controller_poll $o)
done
