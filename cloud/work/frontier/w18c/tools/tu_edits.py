#!/usr/bin/env python3
"""w18c: the layout-neutral TU edits some w18c sources need, applied to a COPY of src/rom.
    python3 lane/tools/tu_edits.py OUTDIR      (run from the scratch root; writes OUTDIR/<tu>.c)
Each edit is (tu, old text, new text); old must occur exactly once. Proven neutral by building the
edited TU alone and comparing with the original (tusplice.sh on OUTDIR without any splice)."""
import sys
from pathlib import Path

VS_OLD = "u32 flags24; u32 value28; u8 unknown2C[32]; u8 external4C;"
VS_NEW = "u32 flags24; u32 value28; u8 unknown2C[2]; u8 channel2E; u8 unknown2F[29]; u8 external4C;"
CL = "typedef struct ChannelLink { u8 previous; u8 next; u16 flags; } ChannelLink;\nextern ChannelLink D_80050440[32];\n"
SLOT = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F6EC.s")\n'
EDITS = [
    # lib_1f5b0: name VoiceState's byte at +0x2E (func_8001F6EC clears it; func_8001EE9C's view
    # calls it channel2E); same layout.
    ("lib_1f5b0", VS_OLD, VS_NEW),
    # lib_1f5b0: hoist func_8001F7EC's ChannelLink typedef + D_80050440 extern above the
    # func_8001F6EC slot (its first user), as wave 4 hoisted StreamState_80025264 in lib_25bb0.
    ("lib_1f5b0", CL.splitlines(True)[0], ""),
    ("lib_1f5b0", CL.splitlines(True)[1], ""),
    ("lib_1f5b0", SLOT, CL + SLOT),
]
# lib_207b0: hoist func_8001FD2C's VoiceState typedef and externs (9 lines) above the func_8001FBB0
# slot, the TU's first slot, which uses them.
HOIST207 = Path("src/rom/lib_207b0.c").read_text().splitlines(True)[13:22]
assert HOIST207[1].startswith("typedef struct VoiceState {") and HOIST207[-1].startswith("extern void func_80020610")
SLOT207 = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_8001FBB0.s")\n'
EDITS += [("lib_207b0", "".join(HOIST207), ""), ("lib_207b0", SLOT207, "".join(HOIST207) + SLOT207)]
# lib_22300: hoist func_80023818's three Macro*_80023818 typedefs and its func_80023754 prototype
# (4 lines) above the func_80023754 slot.
L223 = Path("src/rom/lib_22300.c").read_text().splitlines(True)[734:738]
assert L223[0].startswith("typedef struct MacroState_80023818") and L223[3].startswith("extern void func_80023754(")
SLOT223 = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023754.s")\n'
EDITS += [("lib_22300", "".join(L223), ""), ("lib_22300", SLOT223, "".join(L223) + SLOT223)]
# lib_1a660: hoist func_8001C1D8's ChannelState typedef + D_8004F300 extern (2 lines) above the
# func_8001BDB8 slot.
L1A6 = [l for l in Path("src/rom/lib_1a660.c").read_text().splitlines(True)
        if l.startswith("typedef struct ChannelState {") or l.startswith("extern ChannelState D_8004F300[32];")]
assert len(L1A6) == 2
SLOT1A6 = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001BDB8.s")\n'
EDITS += [("lib_1a660", L1A6[0], ""), ("lib_1a660", L1A6[1], ""), ("lib_1a660", SLOT1A6, "".join(L1A6) + SLOT1A6)]
# lib_1cf90: hoist func_8001D5C0's Vector typedef above the func_8001D0F0 slot (its first user).
VEC = "typedef struct Vector { float x, y, z; } Vector;\n"
SLOT1CF = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D0F0.s")\n'
EDITS += [("lib_1cf90", VEC, ""), ("lib_1cf90", SLOT1CF, VEC + SLOT1CF)]
# lib_11640 (func_80011F60, func_800123A8): the TU's partial node views become the full node
# layouts (same offsets for every field the promoted users read) and move above the first slot that
# uses them; the later `extern void *` pool pointers get their element types.
AN_OLD = "typedef struct AudioNode { struct AudioNode *next; unsigned char unknown04[12]; unsigned short countdown; } AudioNode;\nextern AudioNode *D_80038344;\n"
AN_NEW = "typedef struct AudioNode { struct AudioNode *next; struct AudioNode *previous; unsigned char *buffer; unsigned int unknown0C; unsigned short countdown; unsigned short unknown12; } AudioNode;\nextern AudioNode *D_80038344;\n"
AC_OLD = "typedef struct AudioCacheNode { struct AudioCacheNode *next; struct AudioCacheNode *previous; } AudioCacheNode;\nextern AudioCacheNode *D_80038354;\nextern AudioCacheNode *D_80038358;\nextern AudioCacheNode *D_8003835C;\n"
AC_NEW = "typedef struct AudioCacheNode { struct AudioCacheNode *next; struct AudioCacheNode *previous; unsigned int unknown08; unsigned int unknown0C; unsigned int unknown10; unsigned char *buffer; } AudioCacheNode;\nextern AudioCacheNode *D_80038354;\nextern AudioCacheNode *D_80038358;\nextern AudioCacheNode *D_8003835C;\n"
S11F = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011F60.s")\n'
S123 = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_800123A8.s")\n'
EDITS += [("lib_11640", AN_OLD, ""), ("lib_11640", S11F, AN_NEW + S11F),
          ("lib_11640", "extern void *D_8003833C;\nextern void *D_80038338;\n",
           "extern unsigned char *D_8003833C;\nextern AudioNode *D_80038338;\n"),
          ("lib_11640", AC_OLD, ""), ("lib_11640", S123, AC_NEW + S123),
          ("lib_11640", "extern void *D_80038350;\nextern void *D_8003834C;\n",
           "extern AudioCacheNode *D_80038350;\nextern unsigned char *D_8003834C;\n")]
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
texts = {}
for tu, old, new in EDITS:
    t = texts.get(tu) or Path(f"src/rom/{tu}.c").read_text()
    assert t.count(old) == 1, (tu, old[:60])
    texts[tu] = t.replace(old, new, 1)
for tu, t in texts.items():
    (out / f"{tu}.c").write_text(t); print("edited", out / f"{tu}.c")
