#!/usr/bin/env python3
"""
simple_mips_disasm.py - Basic MIPS disassembler for N64 code analysis

This is a minimal disassembler to analyze extracted game code when
the full mips toolchain isn't available.
"""

import sys
import struct
from pathlib import Path

# MIPS register names
REGS = [
    'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
    't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
    's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
    't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
]

# Common MIPS opcodes
def decode_r_type(instr):
    """Decode R-type instruction"""
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F
    shamt = (instr >> 6) & 0x1F
    funct = instr & 0x3F

    if funct == 0x00 and rd != 0:
        return f"sll     ${REGS[rd]}, ${REGS[rt]}, {shamt}"
    elif funct == 0x02:
        return f"srl     ${REGS[rd]}, ${REGS[rt]}, {shamt}"
    elif funct == 0x03:
        return f"sra     ${REGS[rd]}, ${REGS[rt]}, {shamt}"
    elif funct == 0x08:
        return f"jr      ${REGS[rs]}"
    elif funct == 0x09:
        return f"jalr    ${REGS[rd]}, ${REGS[rs]}"
    elif funct == 0x20:
        return f"add     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x21:
        return f"addu    ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x22:
        return f"sub     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x23:
        return f"subu    ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x24:
        return f"and     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x25:
        if rs == 0:
            return f"move    ${REGS[rd]}, ${REGS[rt]}"
        return f"or      ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x26:
        return f"xor     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x27:
        return f"nor     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x2A:
        return f"slt     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x2B:
        return f"sltu    ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
    elif funct == 0x00 and instr == 0:
        return "nop"

    return f"r-type  funct=0x{funct:02x}"

def decode_i_type(opcode, instr, addr):
    """Decode I-type instruction"""
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    imm = instr & 0xFFFF
    simm = imm if imm < 0x8000 else imm - 0x10000

    if opcode == 0x04:
        target = addr + 4 + (simm << 2)
        return f"beq     ${REGS[rs]}, ${REGS[rt]}, 0x{target:08X}"
    elif opcode == 0x05:
        target = addr + 4 + (simm << 2)
        return f"bne     ${REGS[rs]}, ${REGS[rt]}, 0x{target:08X}"
    elif opcode == 0x06:
        target = addr + 4 + (simm << 2)
        return f"blez    ${REGS[rs]}, 0x{target:08X}"
    elif opcode == 0x07:
        target = addr + 4 + (simm << 2)
        return f"bgtz    ${REGS[rs]}, 0x{target:08X}"
    elif opcode == 0x08:
        return f"addi    ${REGS[rt]}, ${REGS[rs]}, {simm}"
    elif opcode == 0x09:
        return f"addiu   ${REGS[rt]}, ${REGS[rs]}, {simm}"
    elif opcode == 0x0A:
        return f"slti    ${REGS[rt]}, ${REGS[rs]}, {simm}"
    elif opcode == 0x0B:
        return f"sltiu   ${REGS[rt]}, ${REGS[rs]}, {imm}"
    elif opcode == 0x0C:
        return f"andi    ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04X}"
    elif opcode == 0x0D:
        return f"ori     ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04X}"
    elif opcode == 0x0E:
        return f"xori    ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04X}"
    elif opcode == 0x0F:
        return f"lui     ${REGS[rt]}, 0x{imm:04X}"
    elif opcode == 0x20:
        return f"lb      ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x21:
        return f"lh      ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x23:
        return f"lw      ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x24:
        return f"lbu     ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x25:
        return f"lhu     ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x28:
        return f"sb      ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x29:
        return f"sh      ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x2B:
        return f"sw      ${REGS[rt]}, {simm}(${REGS[rs]})"
    elif opcode == 0x31:
        return f"lwc1    $f{rt}, {simm}(${REGS[rs]})"
    elif opcode == 0x35:
        return f"ldc1    $f{rt}, {simm}(${REGS[rs]})"
    elif opcode == 0x39:
        return f"swc1    $f{rt}, {simm}(${REGS[rs]})"
    elif opcode == 0x3D:
        return f"sdc1    $f{rt}, {simm}(${REGS[rs]})"

    return f"i-type  op=0x{opcode:02x}"

def decode_j_type(opcode, instr, addr):
    """Decode J-type instruction"""
    target = (instr & 0x3FFFFFF) << 2
    target |= (addr + 4) & 0xF0000000

    if opcode == 0x02:
        return f"j       0x{target:08X}"
    elif opcode == 0x03:
        return f"jal     0x{target:08X}"

    return f"j-type  op=0x{opcode:02x}"

def decode_regimm(instr, addr):
    """Decode REGIMM instructions"""
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    imm = instr & 0xFFFF
    simm = imm if imm < 0x8000 else imm - 0x10000
    target = addr + 4 + (simm << 2)

    if rt == 0x00:
        return f"bltz    ${REGS[rs]}, 0x{target:08X}"
    elif rt == 0x01:
        return f"bgez    ${REGS[rs]}, 0x{target:08X}"
    elif rt == 0x10:
        return f"bltzal  ${REGS[rs]}, 0x{target:08X}"
    elif rt == 0x11:
        return f"bgezal  ${REGS[rs]}, 0x{target:08X}"

    return f"regimm  rt=0x{rt:02x}"

def disassemble(instr, addr):
    """Disassemble a single MIPS instruction"""
    opcode = (instr >> 26) & 0x3F

    if opcode == 0x00:
        return decode_r_type(instr)
    elif opcode == 0x01:
        return decode_regimm(instr, addr)
    elif opcode in [0x02, 0x03]:
        return decode_j_type(opcode, instr, addr)
    else:
        return decode_i_type(opcode, instr, addr)

def find_functions(data, base_addr):
    """Find function boundaries in code"""
    functions = []
    in_func = False
    func_start = 0

    for i in range(0, len(data) - 4, 4):
        word = struct.unpack('>I', data[i:i+4])[0]
        addr = base_addr + i

        # Function prologue: addiu $sp, $sp, -X
        if (word & 0xFFFF0000) == 0x27BD0000:
            imm = word & 0xFFFF
            if imm > 0x8000:  # Negative offset
                if in_func:
                    functions.append((func_start, addr - func_start))
                func_start = addr
                in_func = True

        # jr $ra (function return)
        if word == 0x03E00008 and in_func:
            # Include delay slot
            func_end = addr + 8
            functions.append((func_start, func_end - func_start))
            in_func = False

    return functions

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <binary.bin> [base_addr] [count]")
        print()
        print("Disassembles MIPS binary code.")
        print("  base_addr: RAM address (default: 0x8010FD80)")
        print("  count: Number of instructions to disassemble (default: all)")
        sys.exit(1)

    bin_path = Path(sys.argv[1])
    base_addr = int(sys.argv[2], 16) if len(sys.argv) > 2 else 0x8010FD80
    count = int(sys.argv[3]) if len(sys.argv) > 3 else None

    with open(bin_path, 'rb') as f:
        data = f.read()

    print(f"Disassembling {bin_path}")
    print(f"Base address: 0x{base_addr:08X}")
    print(f"Size: {len(data)} bytes")
    print()

    # Find functions
    functions = find_functions(data, base_addr)
    print(f"Found {len(functions)} potential functions")
    print()

    # Show first few functions
    num_funcs = min(10, len(functions))
    print(f"First {num_funcs} functions:")
    for i, (addr, size) in enumerate(functions[:num_funcs]):
        print(f"  func_{addr:08X}: {size} bytes")
    print()

    # Disassemble
    print("=" * 60)
    instructions_shown = 0
    max_instructions = count or (len(data) // 4)

    for i in range(0, len(data) - 4, 4):
        if instructions_shown >= max_instructions:
            break

        word = struct.unpack('>I', data[i:i+4])[0]
        addr = base_addr + i

        # Mark function starts
        for func_addr, func_size in functions:
            if addr == func_addr:
                print()
                print(f"// func_{addr:08X} ({func_size} bytes)")

        asm = disassemble(word, addr)
        print(f"{addr:08X}:  {word:08X}  {asm}")

        instructions_shown += 1

if __name__ == '__main__':
    main()
