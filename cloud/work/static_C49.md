# C49 — two complete SDK mask routines, exact source proof awaiting ROM ownership

| Historical target | True SDK body | Original address | Full original bytes | Recipe | Linked words | Strict stack-sensitive |
|---|---|---|---:|---|---:|---:|
| __osPiGetCmdQueue | __osSetGlobalIntMask |0x800100C0|80|O1|20/20 exact|0|
| osEPiRawWriteIo | __osResetGlobalIntMask |0x8000FDA0|96|O1|24/24 exact|0|

Literal final flags `-g0 -O1 -mips2 -G 0 -non_shared`; complete final sources, exact SHA256 hashes and original target object hashes in static_C49/verification.json. Supported readonly exports from canonical DB/blob objects, SDK source commit e24c836796df4bf520ff8b11a5c9d2cea3a66cbd with individual source hashes. SDK bodies retained, actual old function aliases already used by accepted devmgr source; caller/callee prototype is one u32 mask and void return. Actual disable-int8000C4B0/restore-int8000C520 contracts and global mask8002C370 from the original target words agree with canonical SDK behavior. reset-mask retains the actual RCP exclusion0x401.

Fresh independent temporary-directory compile through Rocky and local GNU MIPS linkage resolves every relocation to actual original call/data addresses. Every original instruction/alignment word equals final linked source text without masks; full target extents80/96 exact. Original target ELF textVMA0 produces weighted score10 from objdump jal display C4B0/C520 instead of actual8000C4B0/8000C520. Private copy changes only .text VMA to the authoritative original caller address, asserting text-word identity with the untouched target, and strict stack-sensitive scorer becomes0. This is address metadata normalization, not an instruction edit/scorer mask. Protected DB target records remain unchanged. O2 controls do not match (795/604 weighted;20/21 raw word differences).

**No cartridge credit or shared publication yet.** Current build/layout.us.json has no slots for either function; real ROM109A0/10CC0 addresses fall under the data bin beyond current code-segment end0x10000. Root must review supported original-bin ownership/layout and static denominator before publishing. Existing static61,440-byte denominator cannot acquire extra numerator176 bytes merely because source matches in unregistered bins. No new ownership/pin/layout/scorer/global headers or source TUs changed.

Frozen verifier command: `python3 cloud/work/static_C49/verify.py --repo /home/cburnes/projects/rush2049-decomp --target-dir build/C49 --output /tmp/C49-proof.json`. Root independent source replay can reuse the private original target objects from build/C49. No retail/opcode streams or compiled objects in this packet.
