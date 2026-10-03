#!/usr/bin/env python3
"""Narrow C974 ABI prerequisite proof, using manifest-authenticated targets only."""
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[3]

def signed16(word):
    value = word & 65535
    return value - 65536 if value & 32768 else value

def check_addiu(word, dest, base, offset):
    return (word >> 26, (word >> 16) & 31, (word >> 21) & 31, signed16(word)) == (9, dest, base, offset)

def jal_target(word, pc):
    assert word >> 26 == 3, 'expected direct call'
    return ((pc + 4) & 0xf0000000) | ((word & 0x3ffffff) << 2)

def f12_access(word):
    # Reject unsupported COP1 forms rather than silently hiding a dependency.
    if word >> 26 != 17:
        return False, False
    form, ft = (word >> 21) & 31, (word >> 16) & 31
    if form == 0:  # mfc1
        return ((word >> 11) & 31) == 12, False
    if form == 4:  # mtc1
        return False, ((word >> 11) & 31) == 12
    if form == 8:  # condition-code branch
        return False, False
    assert form in (16, 20), 'unreviewed COP1 format'
    op = word & 63
    assert form != 20 or op == 32, 'unreviewed word conversion'
    fs, ft, fd = (word >> 11) & 31, (word >> 16) & 31, (word >> 6) & 31
    assert op in (0, 1, 2, 3, 4, 5, 6, 7, 13, 32, 36, 50, 60, 62), 'unreviewed COP1 operation'
    reads = fs == 12 or (op in (0, 1, 2, 3, 50, 60, 62) and ft == 12)
    return reads, fd == 12 and op < 48

def prove_no_incoming_f12(words, base=0x800C4200, end=0x800C4300):
    """Explore both outcomes of prefix branches, honoring annulled likely slots.

    This is a bounded first-use proof, not a whole-function liveness analysis.
    No calls or indirect control flow occur in this prefix.
    """
    pending, seen, first_reads = [(base, False)], set(), set()
    def execute(pc, defined):
        word = words[(pc-base)//4]
        read, write = f12_access(word)
        if read:
            assert defined, 'incoming f12 reaches a read'
            first_reads.add(pc)
            return None
        return defined or write
    while pending:
        pc, defined = pending.pop()
        if (pc, defined) in seen:
            continue
        seen.add((pc, defined))
        assert base <= pc < end, 'path escaped before first f12 use'
        word = words[(pc-base)//4]
        op = word >> 26
        after = execute(pc, defined)
        if after is None:
            continue
        cop_branch = op == 17 and (word >> 21) & 31 == 8
        unconditional = op == 4 and (word >> 16) & 1023 == 0
        if cop_branch or unconditional:
            target = pc + 4 + 4 * signed16(word)
            slot_after = execute(pc+4, after)
            if slot_after is not None:
                pending.append((target, slot_after))
            if not unconditional:
                likely = bool((word >> 16) & 2)
                if likely:
                    pending.append((pc+8, after))
                elif slot_after is not None:
                    pending.append((pc+8, slot_after))
        else:
            assert op not in (1,2,3,4,5,6,7,20,21,22,23), 'unreviewed control transfer'
            assert not (op == 0 and word & 63 in (8,9)), 'indirect control transfer'
            pending.append((pc+4, after))
    return sorted(first_reads)

def audit(root=ROOT):
    spec = importlib.util.spec_from_file_location('c974_score', root/'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(score)
    targets = score.targets()
    caller, callee = targets['func_8010C974'], targets['camera_trigger_check']
    at = lambda address: caller[(address-0x8010C974)//4]
    assert check_addiu(at(0x8010CB40), 4, 19, 68)
    assert check_addiu(at(0x8010CB44), 5, 29, 352)
    assert jal_target(at(0x8010CB48), 0x8010CB48) == 0x800C4200
    assert check_addiu(at(0x8010CB4C), 6, 29, 316)
    assert jal_target(at(0x8010D2C4), 0x8010D2C4) == 0x8038D798
    symbols = json.loads(score.verified_bytes(root/'asm/us/blob/symbols.json', score.target_manifest()))['symbols']
    names = sorted(n for n,v in symbols.items() if int(v,16)==0x8038D798)
    first_reads = prove_no_incoming_f12(callee)
    identity = lambda w: dict(native_bytes=4*len(w), native_words=len(w), sha256=hashlib.sha256(struct.pack('>'+str(len(w))+'I',*w)).hexdigest())
    return dict(classification='ABI_PREREQUISITE_ONLY_NO_MATCH', claims=[], caller=identity(caller), callee=identity(callee), call_address='0x8010CB48', arguments=['a0=state+68','a1=caller_sp+352','a2=caller_sp+316 (delay slot)'], f12_first_reads=[f'0x{x:08X}' for x in first_reads], incoming_f12_required=False, unknown_external=dict(address='0x8038D798', symbol_names=names, contract_verified=False), compiled_candidate=False, differing_words=None, unverified_relocations=None, accepted_code_bytes=0)

if __name__ == '__main__':
    print(json.dumps(audit(), indent=2))
