#!/usr/bin/env python3
"""Compile/score only. Requires the pinned PR #163 Git tree in --context-repo."""
import argparse
import json
from pathlib import Path
import subprocess
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline.blob_unit import scan_defs
TREE="277d344e5d9a0edf6eea7732fe7d770cb04aed25"
CONTEXT="cloud/matches/pak_reset_a3724_group"
NAME="track_process_main"
BUILD=ROOT/"build/dot_pak_allocate_20261006"

def report(obj,label):
    r=score.compare(obj,NAME,show=0)
    data,secs=score._elf(obj)
    symbol=next(s for i,sec in enumerate(secs) if sec["type"]==2
                for s in score._symbol_table(data,secs,i) if s["name"]==NAME)
    words=score.text_words(obj)[symbol["value"]//4:]
    frame=next(65536-(w&65535) for w in words[:40] if w>>16==0x27BD)
    print(f"{label}: {r.differing}/{r.total} differing; emitted={symbol['size']//4} words; frame={frame}; extra={r.extra_words}; unresolved={len(r.unresolved)}; unverified={len(r.unverified)}; errors={len(r.errors)}")
    print("existing context func_800A3724:",score.compare(obj,"func_800A3724",show=0).summary())

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--context-repo",type=Path,default=ROOT)
    args=parser.parse_args()
    def read(name):
        return subprocess.check_output(["git","-C",str(args.context_repo),"show",f"{TREE}:{CONTEXT}/{name}"],text=True)
    original=read("group.c")
    spec=json.loads(read("group.json"))
    spec.update(members=[NAME],claims=[])
    spec["context"]=[n for n in spec["context"] if n!=NAME]+["func_800A3724"]
    group=BUILD/"group";group.mkdir(parents=True,exist_ok=True)
    (group/"group.json").write_text(json.dumps(spec,indent=2)+"\n")
    (group/"group.c").write_text(original)
    score.compile_group(group,BUILD/"baseline.o")
    report(BUILD/"baseline.o","PR #163 baseline")
    definitions=[r for r in scan_defs(original) if r["name"]==NAME]
    if len(definitions)!=1: raise SystemExit("Expected one real target definition")
    r=definitions[0]
    (group/"group.c").write_text(original[:r["head"]]+HERE.joinpath("candidate.c").read_text()+original[r["close"]+1:])
    score.compile_group(group,BUILD/"candidate.o")
    report(BUILD/"candidate.o","research NONMATCH")

if __name__=="__main__":main()
