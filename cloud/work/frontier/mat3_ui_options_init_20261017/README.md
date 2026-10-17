# Options initScreen: authentic boundaries, two frame words open

Research only, baseline `58425072b49e95f0fbe5c4c239988fb4885f7c5c`.
`replay_save_prompt` is the historical label for optionscr `initScreen` at
0x800DABDC, 1,540 bytes. It is not a replay-save dialog. Claims are empty.
No canonical edits, integration, full-ROM gate, behavior harness, or new
accepted-image credit are included.

## Result

The complete private target is **2/385 differing words**, with its native
1,540-byte extent, no unresolved/unverified references, no errors or extra
words. Only frame allocation/release differ: 248 bytes rather than 256.
Color at sp+156 and texture index at sp+150 are native-exact. The scorer
verifies the four-byte color initializer at 0x80116DAC and float literals
at 0x80124238..0x80124248. The current source is not a match.

The genuine DB1E0 caller remains MATCH. DA174 retains both real callers,
DB1E0 and this initializer, and remains 2/83 (its separate frame-only lead).
No synthetic caller or boundary blocker was added. All known emitted native
bodies are members; context and claims are empty. Full receipt:

- time_result_display / DA174: 2/83
- func_800DB1E0: MATCH, inherited
- func_800DA2C0: code exact, inherited mixed-rodata layout refusal
- slot_state_setup: 2/58 and two extra nonzero words
- object_create: 28/28, internalized/deleted body
- func_800B669C: code exact, one extra nonzero word
- replay_save_prompt / DABDC: 2/385
- func_800B8810 / SaveGlobals: 4/4, internalized/deleted body
- sign_extend_call / MBOX_NewObject: 10/10, internalized/deleted body

Non-native helper names are emitted deleted-procedure stubs, not additional
native functions or matching claims. Making them non-static gives them
explicit symbols so their artifacts do not silently extend the initializer
or its button callee. Existing inherited artifacts remain visible above.
Keeping the two native wrappers yields exact wrapper bodies but changes the
initializer to 6/385. That bounded control does not justify a whole-group claim.

## Evidence and progression

MAT3 optionscr DWARF records initScreen's signed-short y; calcOptionMenuMaxSize's
int height then int i; and initButtons' color union, texture pointer,
unsigned-short texture index, and int button index. The authentic boundaries
and local order reduce the flattened inherited partial. N64's menu bound14,
height210, button count32, y55.2, object stride68, controller stride16, and
queue operations remain native. The donor's GC height214/y59.2 are not copied.

The locked options-button source already declares volatile gNumModels and a
Pad16 controller record. Restoring the volatile model-count reads recovers
four native address/load instructions (242/385 ->24/385). Restoring the
controller record rather than a two-dimensional byte array removes15
register-only differences (24 ->9). A genuine SaveGlobals source unit removes
the initial four scheduling differences (9 ->5). Its native four-word
function identity at B8810 was verified against the protected target.

The original MAT3 MBOX_NewObject DWARF and current matching donor agree on
int parent, signed-short dlidx, and signed-short return. ZOID_AddObject takes
short parent and returns short. N64's current canonical allocator body at
8008E26C ends with a signed-short return; both returned-short wrapper forms
independently reproduce the native 40-byte sign_extend_call extent. Including
the genuine dlidx-local wrapper contributes24 bytes of inline storage and
puts both color/texture homes on their native offsets. A direct return loses
that evidence and fails the replay negative control.

The donor color initializer is an actual four-byte union initializer, so its
owned data is verified rather than treated as an external constant. Native
color helper argument narrowing is safe because the genuine allocator return
is already signed-short. The current matching GC ZOID_SetColor pointer API is
used; no extra nested color wrapper is invented.

Trace confirms ugen already places the object-lookup return copy ahead of
argument setup. as1 reordered the three words according to source locations
when the MBOX wrapper definition appeared before its callsite. Forward
declaration with the real definition after the callsite preserves native
ordering. This source-layout scheduling lever is disclosed. The complete
instrumented object equals stock output; no tracing modification is accepted.

## Donor provenance and limits

Read-only supplied MAT3 reconstructions and original DWARF are evidence for
boundaries, locals, types and semantics, not automatic N64 matching proof:

- optionscr.c SHA256: 054f390e4d4580fd5150b3530e792968e8d5dd255448887c7f468f86d469bf17
- mbox.c SHA256: 8cbe7ecbc8b1ff1a373c73aeef46b3f6e80a8e6b01ad520858970084d238690e
- zoid.c SHA256: 9d95dd5f92f5aaae64e5f29d9c9779a317db2b44fe8b4477eaac8ae79167699b
- save.c SHA256: b3dbeb247cd8db6746b1d5bf9ce73176c0277cc5debe6e42affb763d74f4e153
- zoidtexture.c SHA256: 3c6c46b38ab700bf405e32bbbce3c5fabcb5ea7ed3e47799b2c293b883cebe43
- HAL_text.c SHA256: 01edf586c784fbb0ed8da894559957b058e4da80de3af1c6f564731891a3a4fb

The remaining8 bytes have no authenticated explanation. The current MAT3
options reconstruction includes an addPane arithmetic helper whose historical
identity is unproven. It is not adopted here, and no padding array, unused local,
synthetic helper, forced register, arbitrary prototype, or target edit closes
the frame. Parenthesization controls preserve the same residual. Same-TU
SaveGlobals-after-use also leaves frame248 and worsens scheduling; it is not
preferred over this source-unit form. Inherited locked button-source shaping
is retained verbatim and not represented as newly authenticated source.

## Replay

With stock IDO_DIR configured, from the repository root:

    python3 cloud/work/frontier/mat3_ui_options_init_20261017/verify.py

The replay pins every source and target digest; freshly compiles the complete
group; checks every declared member against the full refusal-preserving receipt;
requires exactly the two frame words; and rejects wrong font, missing dlidx,
flattened SaveGlobals, and wrong owned literal. It uses explicit exceptions,
not removable assertions. Normal and python -O outputs are identical. The
wrong-literal result proves that a low instruction difference alone cannot
hide bad data. Compared retail byte values are omitted from the receipt.
