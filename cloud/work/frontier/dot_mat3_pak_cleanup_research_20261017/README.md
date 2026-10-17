# Controller-Pak cleanup: zero target-word residual, failed group boundary

Research target `func_800A3640` / native Pak-removal cleanup, 0x800A3640,
204 bytes. **Empty claims; zero new candidate or image bytes.**

The candidate reproduces all 51 target words with the native 24-byte frame.
It still has **two excess nonzero words**, from adjacent anonymous deleted-helper
residue, so the strict scorer rejects it. This is not a matching claim.
No guessed identity is assigned to the anonymous helpers to hide that boundary.

The fresh result combines two genuine source ingredients:
- the entire unchanged accepted `codex_heap_release_a25` context, including its
  real private release-call convention and original external fill boundary;
- the already published branch-local allocation handle in the October 6 cleanup
  reconstruction, rather than the older function-wide cached handle.

With the real heap context but the outer handle local, cleanup differs by five
words. The older typed context with the nested handle differs by 29 words.
The current replay independently tests the handle lifetime and external-only
heap boundary. There is no new unused local, artificial caller, false SDK
prototype, assembly patch, register directive, or arbitrary padding.

## Source evidence and platform distinctions

The complete typed Pak source descends from the independently published PR163
source at tree `277d344e5d9a0edf6eea7732fe7d770cb04aed25` and the October 6
endpoint/cleanup packets already in this repository. Current SDK and game
symbol names are restored by their native call destinations: osSendMesg,
osPfsAllocateFile, osPfsFileState, osMotorInit, osPfsRepairId, PS_CreateFile,
PS_FlushPak and pak_delete_file_retry.

The fresh MAT3 paksysdc donor supports `_RemovePak` handle traversal, successor
preservation, removal callbacks, allocation cleanup and list removal. Its
platform backend separately allocates directory entries and icons; those GC
operations are not transferred. The N64 callback executes before testing the
allocation field, and the removal list uses the post-callback port field.
The native request pool is recycled by insertion at the free-list head.

The donor `_ClearPak(unsigned char)` has one signed damaged local. Its direct
index spelling is used with the independently established native 772-byte Pak
record and signed byte at offset 5; this is object-neutral. The GC 604-byte
record, directory-list reset, initialization backend and six-local server
skeleton do not establish N64 SDK storage or the full native server locals.
MAT3 source SHA256 is
`9454217151d592fd263fa5f969737204b6c6aa7558d212ce5512bd7300527f87`.
Its historical reviewed export records paksysdc 33/33; no fresh MAT3 compile or
N64 source identity is asserted by that historical status.

## Complete context

The replay copies all 11 current accepted heap source files unchanged and
materializes the full typed Pak source. It binds 34 scored bodies and all 42
emitted function symbols; the anonymous residue remains explicitly rejected.
All 26 accepted heap functions remain strict with protected ELF extents.
The status converter is also strict; the prior reset candidate is strict at
88 bytes. Neither receives new credit.

The following complete failures remain: initializer 16/81 (324 bytes), status
poller 51/53 (360 bytes plus 33 excess words), deleted insertion wrapper 14/14
(8 bytes), CreateFile 348/438 (1776 bytes plus 6 excess words), and Pak server
747/800 (3080 bytes). The status poller is an inherited regression of an
existing locked routine. It is not silently dropped or treated as accepted.
Using its separately accepted source creates another excess-word boundary;
that private control is not substituted for this frozen result.

Existing accepted heap shaping, including its documented unused locals,
remains unchanged. The broad server, queue factoring and visibility are
reconstruction context, not newly proven original source boundaries. The
separately compiled fill routine remains external exactly as in the accepted
heap recipe; importing it changes that boundary and regresses accepted bodies.

## Reproduction and bounded semantics

With the stock IDO toolchain configured, run from the repository root:

```
python3 cloud/work/frontier/dot_mat3_pak_cleanup_research_20261017/replay.py
python3 -O cloud/work/frontier/dot_mat3_pak_cleanup_research_20261017/replay.py
pytest -q tests/cloud/test_mat3_pak_cleanup_research.py
```

The receipt binds all packet/context sources, recipe, scorer, own-data verifier,
actual IDO stage executables and protected target hashes. Repeated complete
object identity is required. Native layouts are verified through emitted
constants, including OSPfs 104 bytes, Pak record 772, request 76, buffer offset
72, byte port/file offsets 16/17 and handle/list geometry. A wrong request stride
must fail. Python optimization cannot remove verification guards.

A fatal-UBSan host comparison covers 65,536 finite fixtures: four ports, zero
to four request handles, optional allocation handles, callbacks that change the
current successor/allocation or relocate the current handle, ordered queue and
release traces, and recycling order. An independent index-driven cleanup model
shares controlled service implementations with the compiled candidate body.
Six semantic mutations must fail with the exact differential exit and message.

This does not execute the native image or validate the SDK, real callbacks,
allocator internals, generic linked-list code, malformed pointers, concurrency
or scheduler behavior. Host handles are real host pointers; the address scalar
is widened to uintptr_t so the harness does not truncate them. Native 32-bit
layouts are checked separately. The full unmatched server is not executed by
this harness. No canonical integration, ROM/image build, merge or CI monitoring.

## Missing-tool portability follow-up

Compiler fingerprints now resolve through the scorer's standard IDO availability check. When the toolchain is absent, tests follow the shared local-skip policy; REQUIRE_TOOLCHAIN=1 still makes those skips failures. The new diagnostic test pins that boundary. C, recipe, target data and all comparison/semantic results are unchanged. The refreshed proof and narrow availability review supplement the original source/semantics review. No claims or byte credit are added.
