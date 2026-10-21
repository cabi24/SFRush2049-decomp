#!/bin/bash
# usage: rowtest.sh VARDIR FUNC START END   -> prints our rows START..END (brief)
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h/tools/first.sh $1 $2 $3 $4 | tr '\n' ';' | cut -c1-400; echo
