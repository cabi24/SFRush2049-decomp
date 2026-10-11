#!/usr/bin/env python3
"""Fresh stock-uopt partial listing, reduced to target-only decisions.

The full group listing later aborts in wrapper_ecvt; no diagnostic output is
accepted as matching evidence or run through the assembler. No compiler bytes
or process registers are modified. Raw listings remain temporary.
"""
import json
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import tempfile
import verify

def diagnose():
    source=verify.SOURCE/'group.c';ido=verify.base.score.IDO
    with tempfile.TemporaryDirectory(prefix='cleanup-stock-listing-') as td:
        d=Path(td);shutil.copy(source,d/'group.c');(d/'keep.txt').write_text('func_803A1EAC\n')
        common=['-mips2','-EB','-g0','-O3']
        steps=[['cc','-j',*shlex.split(json.loads((verify.SOURCE/'group.json').read_text())['flags']),'group.c'],
          ['uld','-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt','group.u','-ko','linked'],
          ['usplit','-mips2','-o','split','-t','st','linked'],['umerge','-Olimit','5000',*common,'split','-o','merged','-t','st']]
        for args in steps:
            r=subprocess.run([str(ido/args[0]),*args[1:]],cwd=d,capture_output=True,text=True);verify.require(r.returncode==0,('stock stage',args[0],r.stderr))
        args=[str(ido/'uopt'),'-G','0','-Olimit','5000',*common,'merged','opt','-t','st','optlog','-l','listing','-zdbug:6']
        r=subprocess.run(args,cwd=d,capture_output=True,text=True)
        text=(d/'listing').read_text();start=text.index('LOCAL OPTIMIZATION OF func_8039D05C');end=text.index('LOCAL OPTIMIZATION OF func_8039D300')
        target=text[start:end]
        verify.require('global coloring of func_8039D05C' in target and 'REEMISSION OF func_8039D05C' in target,'incomplete target listing')
        rows=[{'web':int(a),'origin':int(b),'color':int(c)} for a,b,c in re.findall(r'\s+(\d+):\s+(\d+) assigned \(constrained\)\s+(\d+)',target)]
        expected=[(105,18),(113,19),(27,20),(13,21)]
        relevant=[(r['web'],r['color']) for r in rows if r['web'] in (105,113,27,13)]
        verify.require(relevant==expected,('cleanup coloring order changed',relevant))
        verify.require(r.returncode!=0 and 'wrapper_ecvt' in r.stderr,'expected later diagnostic abort changed')
    return {'source_sha256':verify.base.sha(source.read_bytes()),'uopt_sha256':verify.base.sha((ido/'uopt').read_bytes()),
      'diagnostic_flags':['-l','listing','-zdbug:6'],'target_local_optimization_through_reemission_complete':True,
      'complete_group_diagnostic_succeeded':False,'later_failure':'wrapper_ecvt assertion; diagnostic object is not built',
      'allocation_order':rows,'residual_roles':[{'web':105,'role':'slot loop byte bound 2816','compiled_register':'s4','native_register':'s5'},
      {'web':113,'role':'panel loop byte bound 572','compiled_register':'s5','native_register':'s6'},
      {'web':27,'role':'signed-byte player copy','compiled_register':'s6','native_register':'s7'},
      {'web':13,'role':'player loop index','compiled_register':'s7','native_register':'s4'}],
      'scope':'web numbers are this exact source/stock build; no priority-cost or whole-group fidelity claim'}

if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path);a=p.parse_args();text=json.dumps(diagnose(),indent=2)+'\n'
    if a.output:a.output.write_text(text)
    print(text)
