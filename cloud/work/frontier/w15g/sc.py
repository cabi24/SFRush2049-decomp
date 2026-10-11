#!/usr/bin/env python3
"""sc.py SRC.c TARGET [--flags F] [--ver L] : compile with IDO (toolkit) and score vs tgt/TARGET.o
Prints: true score and reloc-blind score, using conveyor scoring.py."""
import os,sys,subprocess,shlex,argparse,tempfile
HERE=os.path.dirname(os.path.abspath(__file__))
TK=os.path.expanduser('~/rush2049/cache/toolkits/d4c39cbc85750cd02d3494f318c65e770bbf7d4b10aae137bcf5b5f149137c5b')
os.environ['CONVEYOR_TOOLKIT']=TK
sys.path.insert(0,HERE+'/jobs')
import scoring
ap=argparse.ArgumentParser();ap.add_argument('src');ap.add_argument('target')
ap.add_argument('--flags',default='-g0 -O2 -mips2 -G 0 -non_shared');ap.add_argument('--ver',default='L')
ap.add_argument('--keep');ap.add_argument('-D',action='append',default=[]);a=ap.parse_args()
ul=HERE+'/ul'
o=a.keep or tempfile.mktemp(suffix='.o')
cmd=[TK+'/ido/cc','-c','-Xcpluscomm']+shlex.split(a.flags)+['-DBUILD_VERSION=VERSION_'+a.ver,'-DBUILD_VERSION_STRING="2.0%s"'%a.ver,'-D_FINALROM','-DNDEBUG','-D_LANGUAGE_ASSEMBLER' if a.src.endswith('.s') else '-D_LANGUAGE_C','-DF3DEX_GBI',*['-D'+d for d in a.D],'-I',ul+'/include','-I',ul+'/include/PR','-I',ul+'/src','-I',os.path.dirname(os.path.abspath(a.src)),'-I',ul+'/include/compiler/ido','-I',TK+'/shim','-o',o,a.src]
p=subprocess.run(cmd,capture_output=True,text=True)
if p.returncode: print('COMPILE FAIL',' | '.join([l for l in (p.stderr or p.stdout).splitlines() if 'Error' in l][:2]));sys.exit(1)
t=HERE+'/tgt/%s.o'%a.target
print('true=',scoring.score(t,o),'reloc_blind=',scoring.reloc_blind_score(t,o), 'flags=',a.flags,'ver',a.ver)
