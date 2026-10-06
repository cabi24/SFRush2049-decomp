#!/usr/bin/env python3
"""Independent edge drills for unchanged complete effect-tick C.

Host compilation only. Target bytes are reconstructed in memory from the
historical B image by the packet loader. No target compiler or production edits.
"""
import argparse
import ctypes as C
import hashlib
import json
import os
from pathlib import Path
import random
import subprocess
import sys
import tempfile


def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()


def independent_cases(n):
    cases=[]
    def add(label,c,expect=None): cases.append((label,c,expect))
    # Independent oracle: all byte values, including the modulo-before-threshold
    # decrease and the widened-before-saturation increase.
    for phase in (1,0):
      for alpha in range(256):
       for timer in (0.25,0.,-1.):
        expected_alpha=alpha;expected_phase=phase;expected_status=0xA5A50001
        expected_timer=timer
        if timer>0:
            if phase==1:
                expected_alpha=(alpha-8)&255
                if expected_alpha<=16:expected_alpha,expected_phase=16,3
            else:expected_alpha=min(alpha+8,255)
            expected_timer=n.real(n.bits(timer-0.25))
        elif phase==1: expected_phase=3
        else: expected_phase=0;expected_status &= ~1
        c=dict(players=[dict(status=0xA5A50001,phase=phase,countdown=127,alpha=alpha,timer=timer)])
        add('alpha-byte-domain',c,(expected_status,0,expected_alpha,expected_phase,n.bits(expected_timer)))
    for count in range(-128,128):
        add('signed-countdown',dict(players=[dict(status=0,phase=-128,countdown=count,alpha=197,timer=7.)]),(0,(count-1 if count>0 else count)&255,197,128,n.bits(7.)))
    for count,v in enumerate([16,32,64,96,112,144,176,176]):
        add('phase-three-table',dict(players=[dict(status=1,phase=3,countdown=count,alpha=0,timer=-0.)]),(1,max(count-1,0),v,3,n.bits(-0.)))
    for phase in range(-128,128):
        add('signed-phase',dict(players=[dict(status=1,phase=phase,countdown=0,alpha=128,timer=1.)]))
    for count in (-32768,-129,-128,-1,0,1,7,8):
        add('signed-player-count',dict(count=count,players=[dict(status=1,phase=0,alpha=248,timer=1.)]*8))
    # Binary32 neighborhoods around the sentinel, zero crossing and a half-ULP.
    floats=[n.real(x) for x in [0,0x80000000,0x3e7fffff,0x3e800000,0x3e800001,0xbf800000,0xbfffffff,0xc0000000,0xc0000001,0x33800000,0x3f800000,0x3f800001,0x4b800000,0x4b800001]]
    for value in floats:
      for delta in (0.,-0.,0.25,1.,n.real(0x33800000),n.real(0x33c00000)):
       add('binary32-boundaries',dict(timers=[value,value,value],entry_count=8,delta=delta,players=[dict(status=1,phase=1,alpha=24,timer=value)]))
    # Cyclic selection crosses the end and skips independently excluded states.
    for which,excluded in ((0,4),(1,2),(2,3)):
      for start in range(8):
       for target in range(8):
        entries=[dict(state=excluded if j%2 else 1,eligibility=0 if not j%2 else 1) for j in range(8)]
        entries[target]=dict(state=1,eligibility=1)
        timers=[20.,20.,20.];timers[which]=0.
        add('cyclic-eligibility',dict(timers=timers,entry_count=8,entries=entries,fractions=[start/8.]))
      for timer in (-2.,0.):
       for mutation in ('mutation','rng_mutation'):
        timers=[20.,20.,20.];timers[which]=timer
        add('external-memory-reload',dict(timers=timers,entry_count=8,alternate_entries=[dict(object=7-j) for j in range(8)],**{mutation:True}))
    for which,kind,selected_slot in ((0,4,0),(1,2,2),(2,3,1)):
      for selected_index in range(8):
       for entry_count in (-128,-1,0,1,4,8):
        timers=[20.,20.,20.];timers[which]=-2.
        selected=[0,1,2];selected[selected_slot]=selected_index
        entries=[dict(state=kind if j%2 else 1,eligibility=kind) for j in range(8)]
        add('sentinel-state-reset',dict(timers=timers,entry_count=entry_count,selected=selected,entries=entries))
    # Simultaneous expiration/rearm must respect D38, B10, player, F60 order.
    add('multi-frame-lifecycle',dict(timers=[0.,0.,0.],entry_count=8,repeat=16))
    add('multi-frame-rearm',dict(timers=[-2.,-2.,-2.],entry_count=8,repeat=16))
    return cases


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--packet',type=Path,required=True)
    p.add_argument('--reference-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args(); a.packet=a.packet.resolve()
    sys.path.insert(0,str(a.packet))
    import native as n
    import semantic as s
    frozen={f:sha(a.packet/f) for f in ('effect_tick.c','host.c','native.py','semantic.py')}
    code,data=s.native(a.reference_root)
    assert list(data[0][1])==[16,32,64,96,112,144,176,176]
    # Check every concrete nonpointer field and all fixed strides in the host
    # model; pointer widening is confined to the explicit address adapter.
    assert C.sizeof(n.Object)==84
    assert (n.Object.flags.offset,n.Object.scene.offset,n.Object.animation.offset)==(4,12,80)
    assert [getattr(n.Control,x).offset for x in ('count','selected_fast','selected_second','selected_first','timer_fast','timer_first','timer_second','entries')]==[0,1,2,3,4,8,12,16]
    # Conservative direct CFG includes both outcomes and delay slots; jal
    # includes real local bodies plus return continuations, never guessed calls.
    reachable=set();pending=[n.ENTRY,0x80390D38,0x80390B10]
    while pending:
        pc=pending.pop()
        if pc in reachable or pc not in code:continue
        reachable.add(pc);w=code[pc];op=w>>26
        rs,rt=(w>>21)&31,(w>>16)&31
        if op in (2,3):
            reachable.add(pc+4)
            pending.append(((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2))
            if op==3:pending.append(pc+8)
        elif op==0 and w&63==8:reachable.add(pc+4)
        elif op in (4,5,6,7,20,21,22,23,1) or (op==17 and rs==8):
            reachable.add(pc+4);pending.append(pc+4+n.signed(w,16)*4)
            if not (op==4 and rs==rt==0):pending.append(pc+8)
        else:pending.append(pc+4)
    assert set(code)-reachable=={0x80390BFC,0x80390E24,0x80391064,0x80391098,0x803910B0,0x803911D4}
    cases=independent_cases(n)
    covered=set();branches=set();runs=0;labels={}
    with tempfile.TemporaryDirectory(prefix='effect-independent-') as td:
        td=Path(td)
        (td/'adapter.c').write_text('#include "'+str(a.packet/'host.c')+'"\nvoid audit_private(int which) { if (which == 0) func_80390D38(); else func_80390B10(); }\n')
        so=td/'audit.so'
        subprocess.run(['cc','-std=c89','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared','-ffp-contract=off',str(td/'adapter.c'),'-o',str(so)],check=True)
        lib=C.CDLL(str(so));lib.audit_private.argtypes=[C.c_int]
        for ix,(label,case,expect) in enumerate(cases):
            machine=n.Machine(code,n.ENTRY,data,case);host=n.Host(lib,case)
            # Independent input poison changes saved/argument registers, FPRs,
            # condition state and HI/LO. There are no semantic incoming inputs.
            rng=random.Random(ix+0xF60)
            for reg in list(range(1,29))+[30]:machine.r[reg]=rng.getrandbits(32)
            machine.f=[n.bits(rng.uniform(-100,100)) for _ in range(32)]
            machine.original_r=machine.r[:];machine.original_f=machine.f[:]
            machine.condition=bool(ix&1);machine.lo=ix;machine.hi=~ix
            for frame in range(case.get('repeat',1)):
                machine.r[31]=n.RETURN
                actual=machine.run();portable=host.run();runs+=1
                assert actual==portable,('differential mismatch',ix,label,frame,case)
                if expect is not None:assert actual[0][3][0]==expect,('oracle mismatch',label,case,actual[0][3][0],expect)
            labels[label]=labels.get(label,0)+1;covered|=machine.coverage;branches|=machine.branches
        private_runs=0
        for which,entry in ((0,0x80390D38),(1,0x80390B10)):
          for timer in (-2.,-1.,0.,0.25,5.):
           for flags in (0,2,128,255):
            for frac in (0.,0.125,0.875):
                timers=[20.,20.,20.];timers[0 if which==0 else 2]=timer
                case=dict(timers=timers,entry_count=8,objects=[dict(flags=flags)]*8,fractions=[frac])
                machine=n.Machine(code,entry,data,case);host=n.Host(lib,case)
                rng=random.Random(private_runs+0xB10)
                for reg in list(range(1,29))+[30]:machine.r[reg]=rng.getrandbits(32)
                machine.f=[n.bits(rng.uniform(-100,100)) for _ in range(32)]
                machine.original_r=machine.r[:];machine.original_f=machine.f[:]
                actual=machine.run();lib.audit_private(which)
                if host.error:raise host.error
                assert actual==(host.state(),host.trace),('private mismatch',hex(entry),case)
                private_runs+=1;covered|=machine.coverage;branches|=machine.branches
        # A no-eligible-entry search is outside the terminating domain. Verify
        # the interpreter fails closed instead of silently manufacturing output.
        failed=False
        try:n.Machine(code,0x80390D38,data,dict(timers=[0.,20.,20.],entry_count=1,entries=[dict(state=4,eligibility=1)])).run()
        except AssertionError as ex: failed=str(ex)=='step limit'
        assert failed
    assert frozen=={f:sha(a.packet/f) for f in frozen},'packet changed during audit'
    result={'status':'PASS: bounded semantic audit only','base':s.BASE,'source_sha256':frozen['effect_tick.c'],'packet_inputs_sha256':frozen,'native_targets':{k:{'address':hex(v[0]),'bytes':v[1],'sha256':v[2]} for k,v in s.TARGETS.items()},'paired_root_fixtures':len(cases),'paired_root_executions':runs,'paired_private_executions':private_runs,'oracle_player_cases':sum(x[2] is not None for x in cases),'fixture_groups':labels,'native_instructions_covered':len(covered),'native_instructions_total':len(code),'static_cfg_reachable_instructions':len(reachable),'static_cfg_reachable_all_executed':reachable<=covered,'native_branch_outcomes':len(branches),'native_unexecuted':[hex(x) for x in sorted(set(code)-covered)],'invalid_search_control':'step-limit rejection as expected','target_compiler_invocations':0,'host_compiler_flags':['-std=c89','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared','-ffp-contract=off'],'limits':'The independent fixture/oracle suite uses the manually reviewed packet interpreter and address adapter; it is not a second MIPS engine. Binary32 nearest-even, finite normal-or-zero, valid aligned initialized nonaliasing storage, bounded player/entry counts, valid selected/object indices, terminating eligibility searches. Semantic agreement is distinct from exact object match, original translation-unit identity and whole-image acceptance.'}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
