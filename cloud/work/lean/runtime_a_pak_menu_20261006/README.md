# Image A Controller Pak menu: lean research

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

The genuine four-body group covers 3,116 native bytes:

- `A:func_803A6F2C`, 596 bytes: **2/149 words differ**.
- `A:func_803A7180`, 1,184 bytes: **2/296 words differ**.
- `A:func_803A7620`, 760 bytes: **4/190 words differ**.
- `A:func_803A7918`, 576 bytes: **7/144 words differ**.

All four comparisons have zero extra words, unresolved symbols, unverified
relocations or errors. Every remaining differing instruction concerns a stack
frame size or local-buffer address. Standalone O3 baselines for the three
children were 137/149 (+10 extra words), 296/296 (+17), and 186/190 (+14).
Their real caller restores the native interprocedural register-save context.
The first group attempt also exposed a mistaken final formatter call in the
file list; correcting it to the actual libc `sprintf` removed one difference.

Follow-on to [draft #232](https://github.com/cabi24/SFRush2049-decomp/pull/232).
Reordering only existing local declarations improves A6F2C from 7/149 to 2/149
and A7180 from 6/296 to 2/296. All their real text-buffer offsets now agree.
Only allocation/deallocation frame sizes remain: A6F2C allocates 312 rather
than 320 bytes; A7180 allocates 184 rather than 168. No array capacity,
statement, interface or other function changed. The two sibling scores retain.

## Reproduce

```
python3 cloud/work/lean/runtime_a_pak_menu_20261006/reproduce.py
```

Actual recipe: IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`, canonical
`uld`/`usplit`/`umerge`/`uopt`/`ugen`/`as1`, and `as1 -r4300_mul`.
Only the real A7918 callback root is kept. All three real children are complete;
there are no keeper stubs. Original TU/private visibility remains a hypothesis
supported by the direct calls and native callee clobbers. A:80398BF0 remains
an external lookup with its existing `void **(s32)` interface.

The helper authenticates the frozen compressed asset and full image A in
memory, then uses the unchanged canonical scorer. The root's complete five-way
switch table at A:0x803B9834 is verified by content. No native bytes, assembly
dumps or objects are included. Only compilation/scoring was performed.

## Source and assumptions

The root draws the screen title, dispatches the controller-status, file-list
and confirmation children, and displays two explanatory text states. The
controller view follows both real handle chains before offering available
space. The file view performs wrapping row selection, filename/extension
composition, selected-file headings and free-space descriptions. The
confirmation view preserves the observed title-buffer zero store and language
lookup/choice behavior.

The 72-byte file-prefix view preserves the observed name (+18), extension
(+53), controller (+16), ID (+17) and size (+68) offsets. Unknown spans and the
16-byte controller stride describe real record layout, not stack filler. Both
linked-list layouts retain their distinct file-handle offsets. The language
and node payloads are reloaded across external calls as in the target.

Original local-array capacities are not established. Candidates use 128/128/16
bytes for confirmation, 96/16 for the list, 40/24 for controller text, and two
256-byte root text arrays. Native stack spacing is recorded by the residual
score rather than forced with unused padding or volatile. Inputs must provide
valid acyclic handle lists, valid language indices and terminated strings that
fit these buffers; visible row counts and selection globals must remain
consistent. The callback context is unused, and its full original type is
unknown. Original names and arcade ancestry are unconfirmed.

This is research, with empty claims. Observed improvement is not accepted
coverage; the independent checker owns validation, acceptance and integration.
