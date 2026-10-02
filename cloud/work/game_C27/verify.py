#!/usr/bin/env python3
import argparse,hashlib,json,os,shlex,subprocess,sys,tempfile
from dataclasses import asdict
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,required=True);ap.add_argument('--toolkit',type=Path,required=True);ap.add_argument('--target',type=Path,required=True);ap.add_argument('--source',type=Path,default=Path(__file__).with_name('music_track_control.c'));ap.add_argument('--output',type=Path,required=True);a=ap.parse_args()
os.environ['CONVEYOR_TOOLKIT']=str(a.toolkit);sys.path[:0]=[str(a.repo/'tools/conveyor/jobs'),str(a.repo/'tools/cloud')]
import scoring
import score as retail
expected=json.loads(Path(__file__).with_name('verification.json').read_text());assert hashlib.sha256(a.source.read_bytes()).hexdigest()==expected['source_sha256'];assert hashlib.sha256(a.target.read_bytes()).hexdigest()==expected['target_sha256']
flags=expected['flags'];assert a.source.read_text().splitlines()[0]=='/* flags: '+flags+' */'
with tempfile.TemporaryDirectory(prefix='C27-verify-') as scratch:
 obj=Path(scratch)/'candidate.o';cmd=[str(a.toolkit/'ido/cc'),'-c',*shlex.split(flags),str(a.source.resolve()),'-o',str(obj)];cp=subprocess.run(cmd,capture_output=True,text=True);assert cp.returncode==0,cp.stderr
 result={'function':expected['function'],'flags':flags,'source_sha256':expected['source_sha256'],'target_sha256':expected['target_sha256'],'compile_command':cmd,'compile_exit':cp.returncode,'strict_score':scoring.score(a.target,obj,stack_differences=True),'linked':asdict(retail.compare(obj,expected['function'],show=0))}
 a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result));assert result['strict_score']==0 and result['linked']['differing']==0 and not any(result['linked'][k] for k in ['unresolved','unverified','errors','extra_words'])
