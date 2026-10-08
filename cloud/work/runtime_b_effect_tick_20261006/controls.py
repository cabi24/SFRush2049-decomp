"""Host-only falsification controls; none invokes the target compiler."""
import ctypes,subprocess,tempfile
from pathlib import Path
from semantic import HERE,native
from native import verify_host,Machine,ENTRY

MUTANTS={
 'alpha_wrong_clamp':('player->alpha <= 16','player->alpha < 16',[dict(players=[dict(status=1,phase=1,alpha=24,timer=1.)])]),
 'countdown_wrong_signedness':('s8 countdown;','u8 countdown;',[dict(players=[dict(countdown=-1)])]),
 'phase3_wrong_lookup':('alpha_steps[player->countdown]','alpha_steps[0]',[dict(players=[dict(status=1,phase=3,countdown=4)])]),
 'wrong_countdown_decrement':('if (player->countdown > 0)','if (player->countdown >= 0)',[{}]),
 'wrong_status_bit_clear':('player->status &= ~1','player->status = 0',[dict(players=[dict(status=0x80000001,phase=2,timer=0.)])]),
 'fade_in_wrong_saturation':('player->alpha = 255','player->alpha = 254',[dict(players=[dict(status=1,phase=2,alpha=255,timer=1.)])]),
 'reset_wrong_range':('func_8008B2E4(10.0f)','func_8008B2E4(60.0f)',[dict(timers=[-2.,20.,20.])]),
 'wrong_sentinel':('timer_fast == -2.0f','timer_fast == -1.0f',[dict(timers=[-2.,20.,20.])]),
 'wrong_expiry_boundary':('timer_fast < 0.0f','timer_fast <= 0.0f',[dict(timers=[0.25,20.,20.])]),
 'wrong_search_kind':('D_80399AE0.entries[index].state == 4','D_80399AE0.entries[index].state == 3',[dict(timers=[0.,20.,20.],fractions=[0.],entries=[dict(state=4),dict(state=1)])]),
 'wrong_search_eligibility':('D_80399AE0.entries[index].eligibility != 1','D_80399AE0.entries[index].eligibility != 2',[dict(timers=[0.,20.,20.],fractions=[0.],entries=[dict(eligibility=1),dict(eligibility=2)])]),
 'wrong_setter_resource':('object->scene, D_80142A82','object->scene, D_80142A84',[dict(timers=[0.,20.,20.])]),
 'wrong_animation':('object->animation = 353','object->animation = 354',[dict(timers=[0.,20.,20.])]),
 'selected_index_reused_after_call':('func_8008B0D8(D_80399AE0.entries[index].object->scene, 0, 15);\n            D_80399AE0.timer_fast', 'func_8008B0D8(D_80399AE0.entries[D_80399AE0.selected_fast].object->scene, 0, 15);\n            D_80399AE0.timer_fast',[dict(timers=[0.,20.,20.],mutation=True)]),
 'child_order_reversed':('func_80390D38();\n    func_80390B10();','func_80390B10();\n    func_80390D38();',[dict(timers=[0.,20.,0.])]),
}
def controls(reference,work):
    code,tables=native(reference);source=(HERE/'effect_tick.c').read_text();host=(HERE/'host.c').read_text();rejected=[]
    for name,(old,new,cases) in MUTANTS.items():
        assert old in source,name
        where=work/name;where.mkdir();(where/'effect_tick.c').write_text(source.replace(old,new));(where/'host.c').write_text(host)
        so=where/'host.so';subprocess.run(['cc','-std=c89','-O2','-fPIC','-shared','-ffp-contract=off',str(where/'host.c'),'-o',str(so)],check=True)
        try:verify_host(code,tables,ctypes.CDLL(str(so)),cases)
        except AssertionError as exc:
            assert 'semantic mismatch' in str(exc),name
            rejected.append(name)
        else:raise AssertionError(('mutant survived',name))
    try:Machine({ENTRY:0xFFFFFFFF},ENTRY,tables,{}).run()
    except AssertionError as exc:assert 'opcode' in str(exc)
    else:raise AssertionError('unknown opcode accepted')
    return rejected+['unknown_opcode_refused']
if __name__=='__main__':
    import argparse,json
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);args=p.parse_args()
    with tempfile.TemporaryDirectory(prefix='effect-controls-') as td:result=controls(args.reference_root,Path(td))
    args.output.write_text(json.dumps({'rejected':result},indent=2)+'\n');print(json.dumps({'rejected':result},indent=2))
