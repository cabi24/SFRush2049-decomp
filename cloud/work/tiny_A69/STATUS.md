# A69 — func_800FC9F8: frozen nonmatch

No claims. Canonical extent 0x800FC9F8–0x800FCDF0, 254 words / 1,016 bytes; ordinary ABI with no calls. Current shared lock and accepted interval checks were clear at reservation. Literal flags: `-g0 -O2 -mips2 -G 0 -non_shared` (canonical compiler also applies its r4300_mul handling).

Final source SHA256: `84998d6414dee6cb4ed6ebfdaaf57db6938359dbb7d1547ae57f9674aa4c8728`.
Canonical verification exits 1: strict differing words 235/254, compiled 254 words, extra 0; no errors, unresolved symbols or unverified evidence. Complete original target disassembly, compiled object/disassembly, raw comparison and canonical stdout are ignored under `build/codex-A69/`, with corresponding private Rocky copies. No original instruction streams are included in this packet.

The complete source preserves four nullable point-list slots; signed halfword count and coordinates at byte 20/22 + 20*i; dimensions at unsigned halfword offsets 16/18; signed frame halfword; delta fields at frame*224 + slot*56 -208/-204; float outputs at -200/-184 + 4*i; dirty flag at frame-1. It preserves conversion through signed 32-bit truncation then signed halfword stores. It deliberately preserves the unusual second Y test `y <= +height*32`, not a conventional negative lower bound. Shift sums become dimension corrections only at exactly -4 or +4. Lists with zero/negative counts still mark the current frame dirty.

The motion symbol's original containing aggregate and valid frame/count domains are not proved. The source uses observed byte offsets without inventing a header/prefix, allocation capacity or bound check. Thus this is an assembly-grounded behavioral reconstruction, not a certified runtime-domain or original declaration recovery. The two-float correction array is supported by adjacent retail stack slots; no additional padding or unrelated locals were introduced.

Bounded controls:
- Initial explicit point-record projection, separate scalar correction locals: 250/254 differing, 236 compiled words.
- Halfword-list accesses remove the unproved single-element payload capacity: same 250/254, 236 words.
- Actual two-component correction array: 235/254, exact 254 words; final retained source.
- A raw 224-byte row projection and factorized stride expression each produce the same 235/254 and 254 words; neither retained because they add no evidence/gain.

Remaining differences include a 48-byte source frame versus 40-byte retail frame, an additional saved register, source multiply-by-224 versus the retail shifted factorization, and float operation register/operand scheduling. The original aggregate/type algebra and operand provenance remain unknown. No allocation, register/formal permutation, arbitrary pressure, reflow or padding sweep was attempted. Stop this hypothesis without a new declaration or semantic lead.

Reproduce in private Rocky checkout:
`python3 tools/cloud/score.py fn cloud/work/tiny_A69/func_800FC9F8.c func_800FC9F8 --flags '-g0 -O2 -mips2 -G 0 -non_shared'`
