# Independent review: authentic graphics compilation context

Reviewed commit `6763dd63f660728138440d276c921f1760d0cb26` separately from the
SDK-scope experiment. No blocking findings. Accepted as bounded negative
research only; the rectangle remains unaccepted.

## Checks performed

- Read the predeclared plan, implementation, receipt, complete relevant group
  sources and portable tests. The experimental recipe adds only the unchanged
  rectangle file, member and external keep root; it neither invents a caller
  nor changes the six authentic source files or existing keep roots.
- Independently reran `python cloud/work/texture_rect_compile_context/verify.py`
  with the pinned stock IDO environment. Every field in the newly generated
  receipt agrees with the committed receipt, including source/tool hashes,
  object hashes, caller inventory, whole-function comparisons and exact sizes.
- Ran the context, existing rectangle-verifier and compiler-boundary tests:
  **22 passed**. The context worktree remained clean.
- Independently linked both standalone and augmented-group objects with GNU
  MIPS ld. Resolved their actual undefined-symbol sets from the protected
  symbol map, extracted the complete rectangle symbol, and compared its bytes
  with the protected target and the receipt. Both leaf extents are **1,780
  bytes**; both GNU-linked hashes are
  `8ced2a588337f504ff6bd865a609a63a66f0c4f89a9c6c0d9cb8abf1c2a2a7a4`.
  The only differences remain `0x4c8`, `0x4cc`, `0x4d0`, and `0x4d4`.
- Confirmed the original seven bodies retain strict zero and exact extents,
  totaling 3,381 words. The mode function's own-data table is verified by the
  existing scorer; the report correctly omits a purported fully relocated
  hash from the lower-level relocator that has not finished that table.
- Reviewed changed paths: research prose, Python, tests and scalar/hash receipt
  only. No target/lock/scorer/compiler edits, binary payloads, raw disassembly,
  synthetic helper functions, or promotion claims.

## Scope and interpretation

The report correctly distinguishes an effective accepted reconstruction group
from recovered original translation-unit boundaries. It also limits the caller
source inventory to the concrete typed-definition syntax searched and does
not treat the two empty stubs as authentic whole-program context. The existing
leaf has no call instructions; the added keep root is appropriate to this
bounded external-entry control and is not proof of the original export list.

The GNU comparison here verifies the rectangle leaf only. Whole-group link
placement is not asserted to reproduce the retail image; the unchanged strict
scorer comparisons provide the existing-neighbor regression check. No ROM,
image splice, compression gate or N64 hardware rendering check was performed.
The result rejects the specific authentic-group hypothesis, not every possible
original source/compiler context.

## Source bindings at review

- `cloud/work/texture_rect_compile_context/PLAN.md`: `0c88e7a3a93726d840a2093430092dd757f307a46fd47f1661141999b5eb1d79`
- `cloud/work/texture_rect_compile_context/README.md`: `de0a0ee0ca89bae11b7195dd696ac198b4039f9c5504c992bbfed5d7c61dd0cb`
- `cloud/work/texture_rect_compile_context/verify.py`: `c13519edfdf4bcd29e8a732fc5252964b5dfd6b8ab7ce805f1666dd26ea5fcd3`
- `cloud/work/texture_rect_compile_context/verification.json`: `665ab74652e87abbe524c058cc685a1e889ecd945936dd3a750c7cd106e42664`
- `tests/cloud/test_texture_rect_compile_context.py`: `da881e68648507d868acb0117b8c49a9bbcc263440bf1f52c627c79b254a4e47`
