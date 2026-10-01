# A22: six ordinary tiny leaves, six strict matches

Frozen full translation units; flags for every target: `-g0 -O2 -mips2 -G 0 -non_shared`. Canonical score.py fn exits0 for all six exact files. Each strict0, zero extra words, unverified/unresolved relocations, and errors. Total40 retail words/160 bytes. Current authoritative shared lock has no duplicate claim. No whole-program context, invented caller, guard, pressure input, or source quirk is required.

| Target | Retail/emitted words | Full TU SHA256 |
|---|---:|---|
| func_800B669C | 5 | `50307061de5efe55e1ff022d234f62884c4efc8d39f4a3374827bb4f599ae66c` |
| func_8009D444 | 6 | `e27847dc937732c8e1335ef379b1236007f702a4fc18efc254a5008bd221a7c2` |
| func_800F75EC | 6 | `62833827a70744b7091917c4c8fcd99afaefc3b37fdc0f1835cfa2171874d08e` |
| func_800F75D0 | 7 | `ddac9dc1234b595f61227c58521395bf044080ec16c306285cc813c2728a6f76` |
| func_800F7604 | 7 | `fff4c7eed63e62f22f34725a6d1615cff34031d05008b0f62d4e4cc14e11c5a2` |
| func_800A79B8 | 9 | `ad3fc832b44fa8ffd7b524e449e1d081face19747d67af2a11416b4e903da828` |

Ten-candidate inventory and exclusion reasons are inventory.json. All selected targets are actual ordinary ABI leaves (no entry hidden register or unsaved preserved-register dependency in the graph/retail instructions), so separate O2 full TUs are appropriate. Selection avoids the observed parameter-home quirks; static C lane and B alias/data ownership targets are disjoint.

Source semantics/proofs: B669C stores full32-bit input words into the actual pair at0x80118E20/+4. Its void return is source reconstruction; retail's final v0 contains the formed address but no explicit return transfer/use contract is claimed. 9D444 returns sqrtf(x*x+y*y), with two real f32 inputs, correct f32 sqrt prototype and intrinsic lowering; retained retail floating operand order and multiply erratum nop match strictly under the standard flags. F75EC returns signed byte at0x80150EB8+row*8+column; F75D0 at0x80150F00+row*5+column; F7604 at0x80150ED8+row*9+column. Unsized row dimensions avoid inventing array capacities, and signed-byte returns follow actual lb sign extension. The real inputs are full32-bit indices with no source narrowing or new bounds guards.

A79B8 computes actual record at0x8012E700+index*68, stores low16 bits of its second full-word input at+26 and the full third word at+28. The local record layout is26 padding bytes, signed16 field, unsigned32 field,36 trailing bytes: size68, field offsets26/28. It reconstructs raw word/halfword bit operations; third-word logical pointer/handle meaning remains unknown and is not asserted. Padding describes observed record stride, not a stack/frame control. Full-width second formal avoids inventing an incoming narrowing home: narrowing occurs only at the actual16-bit store.

Six faithful baseline compiles all matched immediately. Six canonical exact-source fn verifications reconfirmed them. One additional B669C explicit pointer-return control changes the address-forming registers (2/5) and was rejected; final source and hash are restored exactly. No direct or address-taken caller reference was found in the current corpus for this tiny setter, so its logical public return contract remains unknown; exact matched instructions preserve the retail final v0 bits regardless. No mutation sweep/tests or nonstandard assembler flag was needed. scores.json records source hashes, byte-verdict counts and canonical exits only. For each name reproduce: `python3 tools/cloud/score.py fn cloud/work/tiny_A22/NAME.c NAME --flags '-g0 -O2 -mips2 -G 0 -non_shared'`. Raw disassembly/canonical logs stay ignored build/codex-A22 or private Rocky A scratch. No shared source/layout/locks/state or prior packet was changed. Root must independently score then image/full-ROM gate before accepting.
