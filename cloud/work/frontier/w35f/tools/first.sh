#!/bin/bash
# usage: first.sh DIR FUNC START END  -> rows START..END of our FUNC
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35f/tools/g.sh $1 --names $2 --got $2:$3:$4 2>&1 | grep -E '^ *[0-9]+ [a-z]'
