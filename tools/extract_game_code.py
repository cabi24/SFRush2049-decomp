#!/usr/bin/env python3
"""
extract_game_code.py - Extract and decompress game code from Rush 2049 ROM

The game stores its main game logic compressed at ROM offset 0xB0CB10.
It gets decompressed at runtime to RAM address 0x8010FD80+.

This tool extracts and decompresses that data so we can disassemble it.
"""

import sys
import zlib
import struct
from pathlib import Path

# ROM layout constants (from init.c and Codex analysis)
ROM_COMPRESSED_DATA = 0xB0CB10  # Source offset in ROM
RAM_DEST_START = 0x80086A50     # Destination in RAM after decompression (from init.c D_80086A50)
HEADER_SIZE = 0x1000            # N64 ROM header + IPL3

def read_rom(rom_path):
    """Read the ROM file"""
    with open(rom_path, 'rb') as f:
        return f.read()

def find_compressed_data(rom_data):
    """
    Try to find the start of the compressed data.
    DEFLATE streams typically start with specific byte patterns.
    """
    offset = ROM_COMPRESSED_DATA

    # Check for zlib header (78 xx) or raw deflate
    if offset < len(rom_data):
        header = rom_data[offset:offset+2]
        print(f"Bytes at 0x{offset:X}: {header.hex()}")

        # zlib header check
        if header[0] == 0x78:
            print("Found zlib header")
            return offset, 'zlib'

        # Try raw deflate (no header)
        return offset, 'raw'

    return None, None

def try_decompress(rom_data, offset, method='zlib'):
    """
    Try to decompress data from the given offset.
    The game uses custom DEFLATE, so we may need to experiment.
    """
    data = rom_data[offset:]

    print(f"Attempting {method} decompression from offset 0x{offset:X}")
    print(f"First 32 bytes: {data[:32].hex()}")

    try:
        if method == 'zlib':
            # Standard zlib decompress
            decompressed = zlib.decompress(data)
        else:
            # Raw deflate (no header/trailer)
            decomp = zlib.decompressobj(-zlib.MAX_WBITS)
            decompressed = decomp.decompress(data)
    except zlib.error as e:
        print(f"Decompression failed: {e}")
        return None

    print(f"Decompressed {len(decompressed)} bytes")
    return decompressed

def scan_for_compressed(rom_data, start_offset=0x100000):
    """
    Scan ROM for potential compressed data blocks.
    Look for zlib headers (78 xx) and try decompression.
    """
    results = []

    print(f"Scanning ROM from 0x{start_offset:X}...")

    for i in range(start_offset, len(rom_data) - 2, 0x1000):
        # Check for zlib header
        if rom_data[i] == 0x78 and rom_data[i+1] in [0x01, 0x5E, 0x9C, 0xDA]:
            try:
                # Try to decompress
                decompressed = zlib.decompress(rom_data[i:i+0x100000])
                if len(decompressed) > 0x1000:  # Significant data
                    results.append({
                        'offset': i,
                        'compressed_size': None,
                        'decompressed_size': len(decompressed),
                        'first_bytes': decompressed[:16].hex()
                    })
                    print(f"Found compressed data at 0x{i:X}, decompresses to {len(decompressed)} bytes")
            except:
                pass

    return results

def analyze_decompressed(data):
    """
    Analyze decompressed data to identify code sections.
    MIPS code has recognizable patterns.
    """
    if not data or len(data) < 16:
        return

    print("\nAnalyzing decompressed data:")
    print(f"  Size: {len(data)} bytes ({len(data)/1024:.1f} KB)")

    # Check for MIPS code patterns
    # MIPS instructions are 4 bytes, common patterns include:
    # - addiu $sp, $sp, -XX (27BDFFXX)
    # - sw $ra, XX($sp) (AFBFXXXX)
    # - jr $ra (03E00008)

    function_prologues = 0
    jr_ra_count = 0

    for i in range(0, len(data) - 4, 4):
        word = struct.unpack('>I', data[i:i+4])[0]

        # Function prologue: addiu $sp, $sp, -X
        if (word & 0xFFFF0000) == 0x27BD0000 and (word & 0xFFFF) > 0x8000:
            function_prologues += 1

        # jr $ra (return)
        if word == 0x03E00008:
            jr_ra_count += 1

    print(f"  Potential function prologues: {function_prologues}")
    print(f"  jr $ra (return) instructions: {jr_ra_count}")

    # Show first few instructions
    print(f"\n  First 16 words (64 bytes):")
    for i in range(0, min(64, len(data)), 4):
        word = struct.unpack('>I', data[i:i+4])[0]
        print(f"    0x{RAM_DEST_START + i:08X}: {word:08X}")

def write_output(data, output_path, ram_base):
    """Write decompressed data to file"""
    with open(output_path, 'wb') as f:
        f.write(data)
    print(f"\nWrote {len(data)} bytes to {output_path}")
    print(f"RAM base address: 0x{ram_base:08X}")

    # Also create a simple symbol file
    sym_path = output_path.with_suffix('.sym')
    with open(sym_path, 'w') as f:
        f.write(f"// Decompressed game code symbols\n")
        f.write(f"// RAM base: 0x{ram_base:08X}\n")
        f.write(f"// Size: {len(data)} bytes\n")
    print(f"Created symbol stub: {sym_path}")

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <rom.z64> [output.bin]")
        print()
        print("Extracts and decompresses the main game code from Rush 2049 ROM.")
        print("The decompressed code can then be disassembled with mips-linux-gnu-objdump")
        print("or loaded into Ghidra/IDA for analysis.")
        sys.exit(1)

    rom_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2]) if len(sys.argv) > 2 else Path('game_code.bin')

    if not rom_path.exists():
        print(f"Error: ROM file not found: {rom_path}")
        sys.exit(1)

    print(f"Reading ROM: {rom_path}")
    rom_data = read_rom(rom_path)
    print(f"ROM size: {len(rom_data)} bytes ({len(rom_data)/1024/1024:.1f} MB)")

    # First, check the known compressed data offset
    print(f"\n=== Checking known offset 0x{ROM_COMPRESSED_DATA:X} ===")
    offset, method = find_compressed_data(rom_data)

    if offset:
        decompressed = try_decompress(rom_data, offset, method)
        if decompressed:
            analyze_decompressed(decompressed)
            write_output(decompressed, output_path, RAM_DEST_START)
            return

    # If that didn't work, scan for other compressed blocks
    print("\n=== Scanning ROM for compressed blocks ===")
    results = scan_for_compressed(rom_data)

    if results:
        print(f"\nFound {len(results)} potential compressed blocks:")
        for r in results:
            print(f"  Offset 0x{r['offset']:X}: {r['decompressed_size']} bytes")
            print(f"    First bytes: {r['first_bytes']}")
    else:
        print("No zlib-compressed blocks found.")
        print("\nThe game may use a custom compression format.")
        print("Try examining the inflate code in asm/us/5610.s for details.")

if __name__ == '__main__':
    main()
