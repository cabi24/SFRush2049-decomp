#!/usr/bin/env python3
"""Run the actual reconstructed C with C89 and sanitizer builds."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile
P=Path(__file__).resolve().parent

def run():
    rows=[]
    with tempfile.TemporaryDirectory(prefix='runtime-host-') as tmp:
        for label,flags in [('c89',['-O2']),('asan_ubsan',['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie'])]:
            exe=Path(tmp)/label
            command=['gcc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror',*flags,str(P/'host_behavior.c'),'-o',str(exe)]
            subprocess.run(command,check=True,capture_output=True,text=True)
            result=subprocess.run([str(exe)],check=True,capture_output=True,text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
            rows.append({'configuration':label,'result':'PASS','stdout':result.stdout.strip()})
    return {'result':'PASS','configurations':rows,'source_hashes':{str(q.relative_to(P)):hashlib.sha256(q.read_bytes()).hexdigest() for q in [P/'nonmatch/func_80018634.c',P/'host_behavior.c',P/'test_host.py']},'limits':'Synthetic aligned valid resources. Host pointers are64-bit; the separate IDO layout assertions and native replay establish32-bit layouts. Helper bodies are contract stubs.'}
if __name__=='__main__':print(json.dumps(run(),indent=2))
