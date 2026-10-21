#!/bin/bash
# gbuild.sh GROUPDIR: compile a group on watchman2 keeping intermediates in ~/rush2049/scratch/frontier/w36o/gb/<name>/
d=${1%/}; n=$(basename $d)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36o/gb/$n; mkdir -p ~/rush2049/scratch/frontier/w36o/gb"
scp -q -r $d watchman2:rush2049/scratch/frontier/w36o/gb/ || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36o/gb/$n && export T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && python3 - <<'PY'
import json,subprocess,os,re
g=json.load(open('group.json'))
open('keep.txt','w').write(''.join(k+'\n' for k in g['keep']))
T=os.environ['T']
fl=g['flags'].split()
units=[re.sub(r'\.c$','.u',f) for f in g['files']]
common=['-mips2','-EB','-g0','-O3']
steps=[[T+'/cc','-j',*fl,*g['files']],
 [T+'/uld','-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt',*units,'-ko','linked'],
 [T+'/usplit','-mips2','-o','split','-t','st','linked'],
 [T+'/umerge','-Olimit','5000',*common,'split','-o','merged','-t','st'],
 [T+'/uopt','-G','0','-Olimit','5000',*common,'merged','opt','-t','st','optlog'],
 [T+'/ugen','-G','0',*common,'opt','-o','gen','-t','st','-temp','ugtmp'],
 [T+'/as1','-elf','-G','0','-p0',*common,'-r4300_mul','-Olimit','5000','gen','-o','g.o','-t','st']]
for s in steps:
    r=subprocess.run(s,capture_output=True,text=True)
    if r.returncode: print('FAIL',s[0],r.stderr[:2000]); break
else: print('built g.o')
PY"
