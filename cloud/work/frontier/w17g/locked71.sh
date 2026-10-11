#!/bin/sh
# w17g: re-score every LOCKED static function (matched.lock.json source + lock flagset, against its inventory
# target) with IDO 7.1 (tk71), LOCK step of w16g check.py. Question: can a converted segment switch compilers
# without breaking locked bodies? Builder, scratch root.
while read fn tid src fl; do
  f=$(echo $fl | tr '|' ' ')
  r=$(TK=$PWD/tk71 python3 lane/check.py $src $fn $tid --flags "$f -I src/rom -I $(dirname $src)" 2>&1 | grep '^LOCK:')
  echo "$fn $tid $fl $r"
done < lane/locked_list.txt
