"""Optional allocation observation. Instrumented output must equal stock exactly."""
from pathlib import Path
import argparse,json,os,re,shutil

import replay

def observe(work,uopt):
    work=Path(work).resolve();work.mkdir(parents=True,exist_ok=True)
    results={}
    for label,remove_cast in [('candidate',False),('without_count_conversion',True)]:
        d=work/label;shutil.copytree(replay.GROUP,d,dirs_exist_ok=True)
        names=json.loads((d/'group.json').read_text())['members']
        src=d/'hud_setup.c'
        if remove_cast:src.write_text(src.read_text().replace('func_800C8738((u32)D_80110264)','func_800C8738(D_80110264)'))
        stock=d/'stock.o';traced=d/'traced.o';replay.score.compile_group(d,stock)
        original=replay.score._run
        def stage(command,**kwargs):
            command=list(command)
            if Path(command[0]).name=='uopt':
                command[0]=str(uopt)
                command+=['-l',str(d/'uopt.list')]
                env=dict(os.environ,CDX_LOG='1',CDX_DETAIL_WEB='all',CDX_OUT=str(d/'color.log'),W5D_PROC='hud_setup',W5D_OUT=str(d/'pre.log'),W5D_LEVEL='3')
                for name in ['CDX_FORCE','W5D_NOPRE','CDX_PROC']:env.pop(name,None)
                kwargs['env']=env
            return original(command,**kwargs)
        try:
            replay.score._run=stage;replay.score.compile_group(d,traced)
        finally:replay.score._run=original
        replay.check_trace_identity(stock.read_bytes(),traced.read_bytes())
        proc=re.search(r'ordinal proc=hud_setup cdx_proc=(\d+)',(d/'pre.log').read_text())[1]
        decisions=[]
        for line in (d/'color.log').read_text().splitlines():
            if 'p1dec ' in line and f'proc={proc} ' in line:
                r=dict(re.findall(r'(\w+)=([^ ]+)',line))
                if r.get('bestreg') in ['v0','v1','a0']:
                    decisions.append({k:r[k] for k in ['web','save','nocs','totalsave','bestcost','bestreg','forbidden0','numintf','decision','forced']})
        replay.require(all(replay.complete(replay.inspect(stock,n)) for n in names if n!='hud_setup'),'trace changed preserved members')
        results[label]={'scope':'complete deliverable group, all 24 prior members exact','complete_stock_object_identity':True,'inputs_sha256':replay.source_bindings(),'stock_object_sha256':replay.sha(stock.read_bytes()),'traced_object_sha256':replay.sha(traced.read_bytes()),'source_sha256':replay.sha(src.read_bytes()),'target':replay.inspect(stock,replay.FN),'allocation_decisions':decisions}
    return results
if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('--uopt',required=True,type=Path);ap.add_argument('--work',required=True,type=Path);ap.add_argument('--record',action='store_true');args=ap.parse_args()
    result=observe(args.work,args.uopt.resolve())
    if args.record:(replay.HERE/'compiler-decisions.json').write_text(json.dumps(result,indent=2)+'\n')
    else:replay.require(result==json.loads((replay.HERE/'compiler-decisions.json').read_text()),'fresh compiler observation differs')
    print('Both complete instrumented objects equal stock; allocation observations recorded.' if args.record else 'Both complete instrumented objects equal stock.')
