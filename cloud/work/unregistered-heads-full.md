# Remaining unregistered function heads

Refreshed against master `05935a7e` with `python3 cloud/work/tools/ipakit/heads.py --json --md ""`.

The previous audit found 52 heads; 45 now have registered symbols and proved extents. These seven remain unregistered. The word counts below are exploratory audit estimates, **not registration proofs**. Six still need bounded jump-table destination analysis; `func_80104704` crosses the registered `highscore_entry_anim` boundary. See [registration findings](../../specs/012-register-heads/findings.md).

The manifest retains obsolete regions with repeated identical sections. The audit loader reads each identical body once and rejects conflicting copies; the canonical cloud scorer still concatenates repeats.

Opaque runs: 12. Remaining heads: 7. Alternate entries: 0.

| address | words | frame | jr ra | evidence | jal callers | pointer refs | contains registered |
|---|---:|---:|---:|---|---|---|---|
| `0x8010221C` | 137 | 24 | 1 | jal, sweep | 0x80102F90 |  |  |
| `0x80102F30` | 890 | 640 | 1 | pointer, sweep |  | 0x8011471C |  |
| `0x80104704` | 260 | 136 | 1 | jal | 0x80105730 |  | highscore_entry_anim |
| `0x80104B14` | 601 | 576 | 1 | jal, sweep | 0x80105760 |  |  |
| `0x80105480` | 445 | 584 | 1 | pointer, sweep |  | 0x801148C4 |  |
| `0x8010D3C0` | 176 | 32 | 1 | pointer, sweep |  | 0x80118858, 0x80118888, 0x801188B8, 0x801188E8, 0x80118918, 0x80118948, 0x80118978, 0x801189A8, 0x801189D8, 0x80118A08, 0x80118A38, 0x80118A68 |  |
| `0x8010D680` | 119 | 80 | 1 | pointer, sweep |  | 0x8011885C, 0x8011888C, 0x801188BC, 0x801188EC, 0x8011891C, 0x8011894C, 0x8011897C, 0x801189AC, 0x801189DC, 0x80118A0C, 0x80118A3C, 0x80118A6C |  |
