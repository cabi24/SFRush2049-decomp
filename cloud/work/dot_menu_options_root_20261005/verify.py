#!/usr/bin/env python3
"""Replay bounded native contracts and host semantics; never score the fixture."""
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata

sha = lambda data: hashlib.sha256(data).hexdigest()
pack = lambda words: struct.pack('>%dI' % len(words), *words)


def direct_calls(words):
    return Counter(0x80000000 | ((word & 0x3ffffff) << 2)
                   for word in words if word >> 26 == 3)


def native_contracts():
    targets, symbols = score.targets(), score.image_symbols()
    image = owndata.ImageData.from_artifact(ROOT/'asm/us/blob_data')
    root = targets['func_8010AEAC']
    slot = targets['slot_state_setup']
    assert len(root)*4 == 1652 and len(slot)*4 == 232
    assert symbols['func_8010AEAC'] == 0x8010AEAC
    calls = direct_calls(root)
    for name, count in [('func_8010A8D0',1), ('func_8010A7A4',14),
                        ('slot_state_setup',2), ('osRecvMesg',2), ('osJamMesg',2),
                        ('fcvt_wrapper',1), ('object_manager_update',2),
                        ('object_bytes_sum_global',2), ('state_utility',2)]:
        assert calls[symbols[name]] == count
    incoming_calls = sum(direct_calls(words)[symbols['func_8010AEAC']] for words in targets.values())
    assert incoming_calls == 0
    # Check source semantics against table destinations without publishing
    # the extracted table or native instruction words.
    raw_table = image.read(0x8012491C, 56)
    branches = struct.unpack('>14I', raw_table)
    labels = [25,13,15,16,17,18,19,20,21,22,23,24,26,14]
    selectors = [None,10,0,1,2,3,4,5,6,7,8,9,18,11]
    for case, start in enumerate(branches):
        assert 0x8010AEAC <= start < 0x8010B520
        end = min([a for a in branches if a > start] + [0x8010B46C])
        arm = root[(start-0x8010AEAC)//4:(end-0x8010AEAC)//4]
        assert direct_calls(arm)[symbols['func_8010A7A4']] == 1
        label_loads = [w & 0xffff for w in arm if w >> 26 == 0x23 and (w >> 16) & 31 == 19]
        assert label_loads == [labels[case]*4]
        selector_loads = [w & 0xffff for w in arm if w >> 26 == 0x20 and (w >> 21) & 31 == 23]
        assert selector_loads == ([] if case == 0 else [selectors[case]])
    format_bytes = image.read(0x80121018, 17)
    assert format_bytes == b'CONTROLLER %d %s\0'
    callback_slots = [0x80116D68, 0x80116D8C]
    for address in callback_slots:
        assert struct.unpack('>I', image.read(address,4))[0] == symbols['func_8010AEAC']
    # Store a0 at the caller home and save/restores are ABI evidence; none
    # of these observations fixes the char array's source extent.
    stores = [(w >> 16 & 31, w & 0xffff) for w in root if w >> 26 == 0x2b and w >> 21 & 31 == 29]
    assert (4,240) in stores
    return {
        'root': {'address':'0x8010AEAC', 'end_exclusive':'0x8010B520',
                 'bytes':1652, 'sha256':sha(pack(root)), 'direct_incoming_jal_count':incoming_calls},
        'slot_state_setup': {'address':'0x800B4200', 'bytes':232, 'sha256':sha(pack(slot)),
                            'source_semantics_audited':True, 'private_register_abi_unresolved':True},
        'menu_switch': {'cases':14, 'verified':True, 'table_sha256':sha(raw_table),
                        'labels':labels, 'signed_selector_offsets':selectors},
        'format': {'address':'0x80121018','sha256':sha(format_bytes),'verified':True,
                   'text':'CONTROLLER %d %s'},
        'callback': {'type':'s32 (void *state)', 'state_unused':True,
                     'static_record_addresses':['0x80116D4C','0x80116D70'],
                     'member_offset':28, 'target_slots_verified':True,
                     'caller':'replay_save_prompt -> sound_control'},
        'storage': {'capacity':None, 'source_declaration_verified':False,
                    'native_frame_bytes':240, 'buffer_stack_offset':176,
                    'later_short_stack_offset':218,
                    'required_bytes_formula':'13 + decimal_text_length + suffix_length',
                    'suffix':'D_8017A4E0.labels[233]; runtime length unknown'},
    }


def verify():
    build = ROOT/'build/menu_options_root'
    build.mkdir(parents=True, exist_ok=True)
    contracts = native_contracts()
    outputs = {}
    for name in ['host_root_test','host_slot_test']:
        binary = build/name
        subprocess.run(['cc','-std=c89','-Wall','-Wextra','-Werror','-O1',
                        '-fsanitize=address,undefined','-fno-sanitize-recover=all',
                        str(HERE/(name+'.c')),'-o',str(binary)], check=True)
        outputs[name] = subprocess.check_output([str(binary)], text=True,
                            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')).strip()
    assert outputs == {'host_root_test':'root host cases: 410370', 'host_slot_test':'slot host cases: 24576'}
    blocked = subprocess.run(['cc','-std=c89','-fsyntax-only',str(HERE/'root.c')],capture_output=True,text=True)
    assert blocked.returncode != 0 and 'Native compilation is blocked on header capacity' in blocked.stderr
    score.compile_single(HERE/'layout_probe.c', score.DEFAULT_FLAGS, build/'layout.o')
    # Compile only the independently complete semantic slot source. No score:
    # standard ABI compilation cannot prove its private incoming s2 contract.
    score.compile_single(HERE/'slot_state_setup.c', score.DEFAULT_FLAGS, build/'slot_syntax.o')
    prior = ROOT/'cloud/work/ipa-groups/dot_menu_row_caller_20261005'
    prior_proof = json.loads((prior/'verification.json').read_text())
    assert sha((prior/'group.c').read_bytes()) == prior_proof['source_sha256']
    return {
        'base_commit':'7487788a0ed4aae747aeb9bb0309e45fa78e1d9b',
        'claims':[], 'new_verified_function_bytes':0,
        'status':'complete control-flow reconstruction; native declaration/ABI blocked',
        'native_root_compiled':False, 'native_root_scored':False,
        'native_group_match_claimed':False, 'native_literal_ownership_proved':False,
        'native_elf_extent_proved':False, 'production_changes':False,
        'root_compilation_gate':'passed fail-closed test',
        'source_sha256':{p.name:sha(p.read_bytes()) for p in sorted(HERE.iterdir())
                         if p.suffix in ['.c','.h','.py']},
        'previous_menu_context_sha256':prior_proof['source_sha256'],
        'native_contracts':contracts,
        'host_root_cases':410370, 'host_slot_cases':24576,
        'sanitizers':'ASan and UBSan passed', 'native_layout_checks':6,
        'slot_ido_syntax_check':True,
        'fixture_header_capacity':128,
        'fixture_capacity_is_native_evidence':False,
    }


if __name__ == '__main__':
    text = json.dumps(verify(), indent=2, sort_keys=True)+'\n'
    if len(sys.argv)>1:
        Path(sys.argv[1]).write_text(text)
    print(text, end="")
