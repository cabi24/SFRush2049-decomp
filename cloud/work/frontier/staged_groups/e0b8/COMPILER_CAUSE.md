# E0B8: the missing source boundary was the real length helper

## Result and identity gate

Independent validation of the final `cloud/work/ipa-groups/dot_e0b8_vector_normalize/group.c` and its group spec used only the pinned, unmodified IDO 5.3 compiler. Both real public functions were kept:

- `func_8008E098`: strict 0/8 differing words, 32 bytes.
- `func_8008E0B8`: strict 0/35 differing words, 140 bytes.
- No unresolved symbols, unverified references, extra words, or data errors.
- A retained-stage build and a separate `tools.cloud.score.compile_group` build produced identical complete ELF objects, SHA-256 `659f9d233b7c59a7b56bf6249d8dcabcf9a74fcbe60399ac2c176a9529980267`.
- The final comment-only source update preserved the earlier successful object's `.text` exactly.

Final source SHA-256: `dae0d6c43b1d3cf25e41f0d7d625214aff3c8a3bd7303513b515441d30f7d43f`.
Final group spec SHA-256: `138e49b581ef400b5601d240e8346a131b5a44671df7ef4d0f1b3e53618095a5`.

`stock_stage_receipt.json` records the compiler, scorer, intermediate-stage and object hashes. No instrumented compiler, forced color, patched intermediate, altered scoring rule, or unverified acceptance mode was used. Full-ROM/image integration is a separate gate, not claimed here.

## What the stock passes actually do

The caller contains only two consumed scalar locals. It computes length through the genuine three-float `func_8008E098` call, checks the threshold, computes the reciprocal, and normalizes the three original components in place. The helper body is independently matched and remains a public emitted function.

1. **Before inlining:** the retained `usplit` stream contains the actual three-argument helper call. The caller's own locals are `length` at relative home -4 and `inverse_length` at -8. No caller-declared vector snapshot, address-taking, padding, or dummy local exists.
2. **Inlining:** stock `umerge` replaces the call with the real helper body. It creates argument homes x=-24, y=-20, z=-16 and emits a 24-byte local extent for this caller. These are compiler-created homes for genuine consumed arguments.
3. **Allocation and CSE:** the retained stock `uopt` stream distinguishes values needed by the inlined formal arguments from values retained for later in-place output. It emits the following allocation:
   - original x for later output: f12; helper-formal x copy: f2
   - original y for later output: f16; helper-formal y copy: f14
   - helper-formal z copy: f18; original z for later output: a new compiler-generated memory home at -36
   - after the helper expression, length reuses f2
   - on the normalization path, inverse length reuses f14
4. **Native stack shape:** `uopt` itself introduces the z memory home, marks it with `Uvreg`, and emits a 36-byte local extent. It is absent from the pre-optimization `umerge` stream. `ugen` rounds the frame to 40 bytes, so the -36 home becomes stack offset +4. The native z store/reloads arise without source-level address exposure or artificial register pressure.

The pass evidence therefore explains both the three-register rotation and the otherwise surprising 40-byte leaf frame. Inlined formal copies have shorter lifetimes than the original vector components reused after the threshold check. The compiler can reuse their colors for length and reciprocal. This is observed allocation/lifetime evidence; no claim about the allocator's hidden cost tie-break is needed.

Selected record references in the final retained streams (zero-based):

- `merged`: argument stores 46/51/56, corresponding home offsets -24/-20/-16; helper body follows; local extent at record 135 is 24 bytes.
- `opt`: x original/copy stores 50/52; y original/copy stores 55/57; generated z home at records 60-63 and later reload 113-114; length assignment at 82; reciprocal assignment at 99; local extent at 122 is 36 bytes.
- `gen`: frame description at record 32 is 40 bytes.

The workbench's current `ucode-window` display swaps the `Ulod`/`Ustr` length and offset labels. The interpretation above uses the actual record layout (block, length, offset) and checks it against the resulting object. No workbench source was changed. Raw streams and disassembly are deliberately not included in this report.

## Why the tempting controls failed

The clean standalone scalar implementation and the same clean source in a one-root O3 group were frameless, 35/35-different controls. Direct pointer expressions changed the CSE allocation but still produced a frameless 34/35-different body. Capturing x/y/z before the real length-helper call also did not reproduce the required shape.

The historical 13/35 source forced an address-taken z to front-end home -36 by inserting six unused integer locals. Its frame was retained from source layout. A later, genuinely consumed arithmetic-local control could reproduce that same 13/35 body, but still had the wrong y/length/reciprocal colors. Neither control explained the successful implementation's origin of the spill: in the matched group the extra home is created during stock optimization after genuine inlining.

## Reproduction

Use the compiler installed by the repository's pinned `tools/cloud/setup.sh`; set `IDO_DIR` to that installation. From the repository root, first run the normal strict check:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_e0b8_vector_normalize
```

To retain pass output locally under the ignored build directory, copy the final group's `group.c` there and run the same stock stages as `compile_group`:

```sh
mkdir -p build/e0b8-stock-stages
cp cloud/work/ipa-groups/dot_e0b8_vector_normalize/group.c build/e0b8-stock-stages/
cd build/e0b8-stock-stages
printf 'func_8008E098\nfunc_8008E0B8\n' > keep.txt
"$IDO_DIR/cc" -j -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul group.c
"$IDO_DIR/uld" -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt group.u -ko linked
"$IDO_DIR/usplit" -mips2 -o split -t st linked
"$IDO_DIR/umerge" -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st
"$IDO_DIR/uopt" -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st optlog
"$IDO_DIR/ugen" -G 0 -mips2 -EB -g0 -O3 opt -o gen -t st -temp ugtmp
"$IDO_DIR/as1" -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen -o candidate.o -t st
```

Compare both named function extents through the normal protected scorer; inspect `split`, `merged`, `opt`, and `gen` locally if needed. Do not publish retained binary streams or target disassembly.

## Generalizable lesson and limitation

A leaf function's unexpected frame and register allocation can preserve the consequences of a genuine source call that was inlined away. For small math helpers, recover and test the real helper boundary before fitting local declarations to stack offsets. Keeping both genuine public roots protects the source contract and separately checks the helper.

This is a byte-exact, causally explained source reconstruction. It is strong evidence for the helper boundary, not proof of the original historical source spelling or variable names.
