#!/usr/bin/env python3
"""Optional coexistence replay against the independently published PR #82 tree.

The argument must be a materialized checkout with the verified source tree.
It is read only. No PR #82 files are copied into this packet or repository.
"""
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
SPEC = importlib.util.spec_from_file_location('macro_stream_verify', HERE/'verify.py')
v = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(v)
PR82_TREE = '13eb95cbe593611c823dfa3e05a9226cc9c0d1bd'
PR82 = {
    'lib_22300': tuple('func_'+s for s in ('80023844','80023870','8002389C','800238C8',
                                        '800238F4','80023920','8002394C','80023978')),
    'lib_25bb0': ('func_800250F0','func_80025120','func_80025150'),
}


def run(checkout):
    checkout = Path(checkout).resolve()
    tree = subprocess.check_output(['git','rev-parse','HEAD^{tree}'],cwd=checkout,text=True).strip()
    v.require(tree == PR82_TREE, 'PR #82 source tree differs from reviewed snapshot')
    previous = v.score.ASM_DIR, v.score._targets, v.score._target_fingerprint
    try:
        v.score.ASM_DIR=v.ROOT/'asm/us/boot_tail'
        context = v.context_module()
        hashes, results = {}, []
        with tempfile.TemporaryDirectory(prefix='macro-stream-pr82-') as directory:
            tmp=Path(directory)
            for group, names in v.GROUPS.items():
                text=(v.ROOT/'src/rom'/(group+'.c')).read_text()
                baseline=v.build(v.frozen('src/rom/'+group+'.c'),tmp,group+'-baseline',context)
                for scope, functions in (('current',names),('pr82',PR82[group])):
                    paths={}
                    for fn in functions:
                        if scope=='current':
                            source=v.HERE/'sources'/(fn+'.c')
                        else:
                            relative=(Path('cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources')
                                      if group=='lib_22300' else Path('cloud/matches/boot_tail'))/(fn+'.c')
                            source=checkout/relative
                            committed=subprocess.check_output(['git','show','HEAD:'+str(relative)],cwd=checkout)
                            v.require(source.read_bytes()==committed,'modified PR #82 input: '+fn)
                            hashes[str(relative)]=v.sha(committed)
                        if text.count(v.pragma(group,fn))==1:
                            paths[fn]=source
                        else:
                            v.require(fn in v.bodies(text),'missing current candidate: '+fn)
                    rows={fn:context.check(fn,tmp,str(path)) for fn,path in paths.items()}
                    v.require(all(row['status']=='ok' for row in rows.values()),'candidate header refusal')
                    text=v.splice(group,text,paths,rows)
                obj=v.build(text,tmp,group+'-coexistence',context)
                checks={fn:v.exact_bytes(obj,fn) for fn in (*v.EXISTING[group],*names,*PR82[group])}
                raw_equal=v.untouched(baseline,obj,set(checks))
                results.append({'tu':group,'verified_functions':len(checks),'results':checks,
                                'all_offsets_unchanged':True,'unverified_assembly_and_padding_unchanged':True,
                                'all_raw_text_equal':raw_equal,'allocated_data_unchanged':True})
        return {'result':'PASS','pr82_tree':tree,'pr82_source_sha256':hashes,
                'verified_functions':sum(row['verified_functions'] for row in results),
                'own_candidate_functions':8,'pr82_candidate_functions':11,'existing_locks':20,
                'groups':results,'dependency':'No PR #82 file required by the eight repairs. This optional check verifies coexistence only.'}
    finally:
        v.score.ASM_DIR, v.score._targets, v.score._target_fingerprint=previous


if __name__=='__main__':
    try:
        print(json.dumps(run(sys.argv[1]),indent=2))
    except (ValueError,OSError,subprocess.CalledProcessError) as error:
        print(json.dumps({'result':'FAIL','error':str(error)}))
        sys.exit(1)
