# Image A menu callback: 428-byte strict match

Deliverable: `cloud/matches/ovl_a/func_803A4340.c`, complete native extent
`0x803A4340..0x803A44EC`. Stock IDO 5.3 O2 matches all 107 words, with
zero unresolved/unverified relocations, errors, or extra nonzero words.
**New candidate code: 428 bytes. Preserved code: 0. Accepted image/ROM: 0.**
This standalone callback defines no owned data and migrates no locked group.

## Source lever

Current-master baseline `262e9f0f3` retains the earlier complete reconstruction
at `../dot_runtime_a_menu_loop_20261006/candidate.c`: four differing accesses
to the compiler-cached flag pointer (native sp+76, candidate sp+80).

The callback never reads its incoming word. Reusing that already-dead parameter
as the outer index removes its separate local home. It is assigned zero before
its first read. This preserves the one-argument O32 signature and every live
operation, helper prototype, type and buffer capacity. The reused version has
the correct pointer home but a 104-byte rather than 112-byte frame. Two unused
s32 slots represent exactly this measured eight-byte residual. One precedes
and one follows the live locals; that placement also restores the real signed
halfword output-index home at sp+102. The source header discloses both slots.
These declarations are shaping evidence, not a claim to original source names.

Fresh controls: baseline 4/107 (112-byte frame); parameter reuse without slots
4/107 (104-byte frame); removing the second slot 4/107 (112-byte aligned frame).
Two slots both before or both after leave only the real index-home word wrong;
the balanced placement closes it. No patch to IDO or object bytes is used.

## Verification and limits

`verify.py` checks input-bound source/target/metadata receipts, the exact source
transformation, stock strict scoring and all three diagnostic controls. Its
independent ELF reader verifies the complete 428-byte STT_FUNC, four zero
alignment bytes, all 21 relocations and zero owned data. `--deep` independently
GNU-links the whole object at its native entry and verifies all 107 words.
The routine replay also passes under Python -O. This is an overlay single;
there is no static-ROM TU whose per-TU compiler selection is bypassed.

`host_verify.py` compiles the actual new C source and reuses the earlier
independent bounded oracle/harness. All 85,476 cases pass under C89,
strict aliasing, UBSan, bounds sanitization and stack protection. All six
32-bit layout checks pass. This is inherited-harness replay, not a newly
written independent semantic oracle. As documented by that original packet,
font-helper cache writes must remain disjoint from menu/texture storage; valid
storage, a successful texture lookup and the stated bounded player domains
are required. Actual external helpers, N64 hardware and gameplay are not run.
The complete strict native body reproduces the native O32 saves/restores.

A separate worker independently reviewed the source/ABI transformation and
reran stock scoring, GNU/extent controls and receipt replay. No compiler/tool
source hash, lock, Makefile or assembly-text hash is pinned. No binary image,
raw assembly dump, object, credential or unrelated data is published.

## Reproduce

With the documented IDO 5.3 and GNU MIPS binutils environment:

```
python3 tools/cloud/score.py fn cloud/matches/ovl_a/func_803A4340.c func_803A4340 --targets asm/us/ovl_a
# func_803A4340:
#   MATCH
python3 cloud/work/frontier/dot_runtime_a_menu_home_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_menu_home_20261012/verify.py --deep
python3 cloud/work/frontier/dot_runtime_a_menu_home_20261012/host_verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_menu_home.py -q
# 1 passed in 0.11s
```

No broader repository suite, image splice, acceptance, full-ROM hash or merge
is claimed. Publication remains a draft for the independent checker.
