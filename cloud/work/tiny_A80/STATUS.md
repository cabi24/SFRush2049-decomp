# A80 resource publication — frozen non-match

func_800CD798 @ 0x800CD798..0x800CD8EC, 340 bytes / 85 words.
The full native source `func_800CD798.c` is 67/85 with four nonzero extras under
both O2 and O3. Its line-1 literal O2 flags have a fresh proof in verification.json.
The real helper-inclusive module is `cloud/work/ipa-groups/codex_resource_a80`:
caller remains 67/85; format_string_parse is 0/45 with two unverified .rodata
relocations. Claims are empty. This is not an accepted MATCH or coverage credit.

Actual Handle->Object->Resource->data access, u8 mode, six 96-byte primary
profiles and six 96-byte extra profiles, and four 64-byte records were recovered.
Modes 19..24 map to primary 0..5; modes 14..17 use the true 64-byte byte address.
Hash computation consumes 92 or 60 bytes, publishes the word immediately before
that span, and then forwards actual lookup addresses/size values. A1DD4 inspection
confirms slot_state_lookup forwards and consumes the second/third ABI arguments
through its true bit-range callee; these are not invented unused formals.

Original format_string_parse uses unsigned byte and accumulator arithmetic. Its
low-three-bit switch subtracts, ORs, ANDs, XORs, multiplies, or divides for 0..5;
default adds. Native cursor and consumed-byte locals plus each branch's actual
cursor increment reproduce all 45 words under canonical relocation masks; strict
section relocation verification remains unsatisfied. The complete two-body
module contains no dummy helpers, stand-ins, pressure locals, padding, or seeded
M2C bodies. Moving real helper definition before caller did not change results.
Original caller preserves a3 across hash calls; native caller spills/reloads its
actual record pointer instead. No exhaustive tuning or scoring exceptions used.

All sources, group manifest, and sanitized fresh proof are frozen for archival.
