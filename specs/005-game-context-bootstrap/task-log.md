# Task Log: Game-Code Context Bootstrap

## T001 preflight (2026-07-16)

- Branch: `005-game-context-bootstrap`
- Toolchain: GNU binutils 2.40 (`mips-linux-gnu-objdump`,
  `mips-linux-gnu-as`); GCC 12.2.0 (`mips-linux-gnu-gcc`); cpp 12.2.0
- `build/game_code.bin`: 647,072 bytes; SHA-256
  `bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d`
- Extracted targets: 885; extracted targets with `target_o_sha`: 885
- Cluster resolution: all 10 target IDs resolve uniquely in the extracted
  population and all 10 have `target_o_sha` set.

## T005 extent-repair acceptance (2026-07-16)

- First extract: `extents: 243 agree, 423 repaired, 219 conflict`;
  `superseded: 627 targets, 108110 evidence rows purged`.
- Oracle: all 10 cluster exclusive end addresses matched exactly;
  `countdown` ended at `0x800FC9F8` with 668 instructions.
- Immediate second extract: `extents: 666 agree, 0 repaired, 219 conflict`;
  `superseded: 0 targets, 0 evidence rows purged`.
