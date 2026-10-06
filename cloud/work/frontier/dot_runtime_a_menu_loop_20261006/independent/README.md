# Independent menu-loop review

This replays the complete original 428-byte native caller against both the
unchanged hosted C89 candidate and its GNU-linked IDO body. It is an independent
behavioral review of a four-word NONMATCH, not match or coverage credit.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 cloud/work/frontier/dot_runtime_a_menu_loop_20261006/independent/verify.py --check
```

A source-only checkout can use `--repo /path/to/protected/checkout`, or set
`RUSH_PROTECTED_REPO`. IDO_DIR and MIPS GNU binutils must be configured. All
objects and native material are generated beneath ignored `build/`; no native
bytes or assembly dumps are included in this packet. Run without `--check`
only to intentionally regenerate the frozen hash/count receipt. The verifier
rejects Python optimization and requires the frozen source and native hashes.

The independent interpreter executes delay slots, annulled branch-likely
slots, integer instructions, floating argument transfers and all original
caller instructions. External helpers are bounded contract hooks. Caller-save
registers/FPRs are randomized across calls; stack extent, original argument
home, saved registers and return value are checked. This does not execute the
renderer or prove full game integration.

- 4,049 complete native / GNU-linked IDO / UBSan+bounds host trace comparisons
- All 65,536 unsigned texture heights in an additional hosted-source oracle pass
- All 256 table-count values, all 256 skip bytes, all 766 possible font returns,
  index boundaries through 65,535, player-count boundaries through 32,767
- 105 reachable instruction words and all 15 reachable branch outcomes; the
  unsigned texture height makes two signed-division correction words unreachable
- Seven compiled semantic mutants rejected, including cached dynamic count,
  three rows, wrong skip/mode/index, signed texture height and wrong spacing
- Ten corrupted complete-ELF views rejected; exact image/entry/extent, 21
  relocations, 12 bindings, four zero alignment bytes and no owned data/storage

The domain requires a successful texture lookup, adequate accessible flag and
text arrays, valid nonoverlapping pointer chains and disjoint renderer-cache
storage. The font-height helper is not pure and performs real synchronization
writes; this harness permits no arbitrary menu/texture mutation at its call.
Signed-byte and signed-halfword narrowing follows the observed IDO/GCC
low-bit interpretation. No unrestricted pointer, concurrency, gameplay,
image/compression/ROM, hardware, publication or CI claim is made.

The independent helper-source check separately rebuilt accepted
`func_800B24EC.c` SHA-256
`405f7b1f5c31e4e4c19b82d745a778e85c2046903cea6d439d5d0564e925712b`
at ordinary O2, obtaining strict 0/91 with no unresolved/unverified relocation,
errors or extra words. Its native body and the font helper's transitive
synchronization/no-op bodies are additionally bound in this receipt.
