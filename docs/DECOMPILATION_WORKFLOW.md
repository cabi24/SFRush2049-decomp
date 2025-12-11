# Rush 2049 Decompilation Workflow

This document describes the systematic process for decompiling N64 functions to matching C code.

## Overview

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         DECOMPILATION PIPELINE                          │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  1. TRIAGE          2. ANALYZE         3. DECOMPILE      4. MATCH      │
│  ┌─────────┐       ┌─────────┐        ┌─────────┐      ┌─────────┐    │
│  │ Pick    │──────>│ Ghidra  │───────>│ Write C │─────>│ diff.py │    │
│  │ Function│       │ + Tools │        │ Code    │      │ permuter│    │
│  └─────────┘       └─────────┘        └─────────┘      └─────────┘    │
│       │                 │                  │                │          │
│       v                 v                  v                v          │
│  func_list.txt    understand.txt      src/game/*.c    MATCHING!       │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

## Phase 1: Triage (Pick a Function)

### Decision Criteria
Pick functions in this priority order:

1. **Leaf functions** (no `jal` calls) - Easiest to verify
2. **Small functions** (<100 instructions) - Quick wins
3. **Functions with arcade matches** - Reference code available
4. **Functions called by already-decompiled code** - Fills gaps

### Tools
```bash
# List functions by size (smallest first)
./tools/func_list.py --sort-size

# Find leaf functions (no calls)
./tools/func_list.py --leaf-only

# Find functions matching arcade names
./tools/arcade_match.py func_800CA3B4
```

### What to Look For
- **Switch statements**: `sltiu` + jump table = state machine
- **Loops**: `bne`/`beq` jumping backwards
- **Struct access**: `lw $reg, offset($base)` patterns
- **Float ops**: `lwc1`/`swc1` = likely physics/math

---

## Phase 2: Analyze (Understand the Function)

### Step 2.1: Get the Assembly
```bash
# Extract single function
./tools/extract_func.py 0x800CA3B4 > func.s

# Or from game code disasm
grep -A 200 "^800CA3B4:" build/game_code_disasm.txt
```

### Step 2.2: Run Through Ghidra
1. Open ROM in Ghidra (MIPS:BE:32:default)
2. Go to function address
3. Look at decompiler output (Window → Decompile)
4. **Key actions:**
   - Rename variables to meaningful names
   - Set correct types (u32, s32, f32, void*)
   - Define structs for repeated offset patterns
   - Mark function signatures

### Step 2.3: Pattern Recognition Checklist

| Pattern | Assembly | C Equivalent |
|---------|----------|--------------|
| Function call | `jal 0x800XXXXX` | `func_800XXXXX()` |
| Return value | `move $v0, $reg` before `jr $ra` | `return reg;` |
| If statement | `beq/bne $reg, $zero, label` | `if (reg)` or `if (!reg)` |
| Switch | `sltiu` + `sll` + jump table | `switch (var)` |
| For loop | Init, compare, branch back, increment | `for (i=0; i<n; i++)` |
| While loop | Compare at top, branch back | `while (cond)` |
| Struct field | `lw $t0, 0x10($a0)` | `obj->field_10` |
| Array access | `sll $t0, $i, 2` + `addu` + `lw` | `arr[i]` |
| Float load | `lwc1 $f0, offset($reg)` | `float f = *(float*)addr` |
| Float compare | `c.lt.s` + `bc1t/bc1f` | `if (f1 < f2)` |

### Step 2.4: Check Arcade Reference
```bash
# Search arcade source for similar code
grep -r "function_name_hint" reference/repos/rushtherock/

# Common mappings:
# N64 game loop → arcade game/game.c
# N64 physics   → arcade game/cars.c
# N64 collision → arcade game/collision.c
# N64 rendering → arcade game/visuals.c
```

---

## Phase 3: Decompile (Write C Code)

### Step 3.1: Generate Initial Skeleton
```bash
# Use mips_to_c for rough output
./tools/m2c.py func_800CA3B4 > initial.c

# Or use Ghidra's decompiler output
```

### Step 3.2: Clean Up the Code

**Variable naming:**
```c
// BAD (raw output)
int temp_f0;
int temp_v0;
void *temp_a0;

// GOOD (meaningful names)
f32 speed;
s32 result;
Car *car;
```

**Type corrections:**
```c
// BAD
int var = *(int*)(ptr + 0x10);

// GOOD
s32 health = car->health;  // offset 0x10 in Car struct
```

**Control flow cleanup:**
```c
// BAD (goto soup)
if (cond) goto label1;
...
label1:

// GOOD
if (!cond) {
    ...
}
```

### Step 3.3: Match Known Patterns

**N64 SDK patterns:**
```c
// Message queue wait
osRecvMesg(&queue, &msg, OS_MESG_BLOCK);

// DMA transfer
osPiStartDma(&ioMsg, OS_MESG_PRI_NORMAL, OS_READ, romAddr, ramAddr, size);

// Display list
gSPDisplayList(glistp++, dl_object);
gDPPipeSync(glistp++);
```

**Game-specific patterns:**
```c
// Game state check (0x801146EC = gstate)
if (gstate == PLAYGAME) {
    // ...
}

// Car array iteration (0x80152818 = cars)
for (i = 0; i < num_cars; i++) {
    Car *car = &cars[i];
    // ...
}
```

---

## Phase 4: Match (Verify and Refine)

### Step 4.1: Build and Compare
```bash
# Build the project
make VERSION=us -j$(nproc)

# Compare function
./tools/diff.py func_name -m -w

# Score: 0 = perfect match
# Look for:
# - Instruction reordering
# - Register allocation differences
# - Missing/extra instructions
```

### Step 4.2: Common Mismatches and Fixes

| Symptom | Likely Cause | Fix |
|---------|--------------|-----|
| Different registers | Variable ordering | Reorder declarations |
| Extra instructions | Wrong type width | Check s32 vs s16 vs s8 |
| Different branch | Inverted condition | Flip if/else |
| Missing store | Dead store eliminated | Add volatile or use result |
| Loop unrolling | Compiler optimization | Try different loop form |

### Step 4.3: Use decomp-permuter (Last Resort)
```bash
# Only when functionally correct but not matching
cd tools/decomp-permuter
./import.py func_name ../src/game/file.c ../asm/func.s
./permuter.py func_name -j8
```

---

## Algorithmic Tools Reference

### tools/func_list.py
Lists all functions with metadata:
- Address, size, call count
- Leaf function detection
- Arcade match suggestions

### tools/extract_func.py
Extracts clean assembly for a single function.

### tools/struct_finder.py
Analyzes offset patterns to suggest struct layouts:
```bash
./tools/struct_finder.py 0x80152818
# Output:
# Struct at 0x80152818, size ~0x200
# +0x00: pointer (referenced 45 times)
# +0x10: s32 (referenced 23 times)
# +0x14: f32 (referenced 18 times, used in float ops)
```

### tools/call_graph.py
Shows function call relationships:
```bash
./tools/call_graph.py func_800FD464
# Output:
# func_800FD464 (game_loop)
#   ├── func_800C9AE0 (called 2x)
#   ├── func_800CA3B4 (playgame)
#   │   ├── func_800B8C14 (physics)
#   │   └── func_800C1234 (checkpoint)
#   └── func_800DB81C (attract)
```

### tools/arcade_match.py
Searches arcade source for similar code patterns:
```bash
./tools/arcade_match.py --disasm build/game_code_disasm.txt --addr 0x800CA3B4
# Output:
# Best matches:
# 1. game/game.c:playgame() - 73% similarity
# 2. game/game.c:preplay() - 45% similarity
```

### tools/type_propagate.py
Propagates type information through call chains:
```bash
./tools/type_propagate.py --known-types types.json
# Updates symbol_addrs.us.txt with inferred types
```

---

## Quick Reference: MIPS Registers

| Register | ABI Name | Purpose |
|----------|----------|---------|
| $zero | zero | Always 0 |
| $at | at | Assembler temp |
| $v0-$v1 | v0-v1 | Return values |
| $a0-$a3 | a0-a3 | Arguments |
| $t0-$t9 | t0-t9 | Temporaries (caller-saved) |
| $s0-$s7 | s0-s7 | Saved (callee-saved) |
| $gp | gp | Global pointer |
| $sp | sp | Stack pointer |
| $fp | fp | Frame pointer |
| $ra | ra | Return address |
| $f0-$f31 | f0-f31 | FPU registers |

---

## Quick Reference: Common Instructions

| Instruction | Meaning |
|-------------|---------|
| `lw $t, off($s)` | Load word: t = *(s + off) |
| `sw $t, off($s)` | Store word: *(s + off) = t |
| `addiu $t, $s, imm` | t = s + imm (unsigned) |
| `addu $d, $s, $t` | d = s + t |
| `sll $d, $t, n` | d = t << n (shift left) |
| `srl $d, $t, n` | d = t >> n (logical) |
| `sra $d, $t, n` | d = t >> n (arithmetic) |
| `and/or/xor` | Bitwise ops |
| `beq $s, $t, label` | Branch if s == t |
| `bne $s, $t, label` | Branch if s != t |
| `jal addr` | Jump and link (function call) |
| `jr $ra` | Return from function |
| `lui $t, imm` | t = imm << 16 |
| `lwc1 $f, off($s)` | Load float |
| `swc1 $f, off($s)` | Store float |
| `add.s $fd, $fs, $ft` | fd = fs + ft (float) |
| `mul.s $fd, $fs, $ft` | fd = fs * ft (float) |
| `c.lt.s $fs, $ft` | Set flag if fs < ft |
| `bc1t/bc1f label` | Branch on float flag |

---

## Workflow Summary

```
1. Pick function (small, leaf, or arcade-matched)
2. Extract assembly
3. Analyze in Ghidra (or by hand)
4. Identify: variables, types, control flow, calls
5. Check arcade source for reference
6. Write initial C
7. Build and diff
8. Fix mismatches
9. Repeat until score = 0
```

**Tips:**
- Start with the easiest functions to build confidence
- Document struct layouts as you discover them
- Keep symbol_addrs.us.txt updated
- Use arcade source liberally - it's your secret weapon
