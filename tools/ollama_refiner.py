#!/usr/bin/env python3
"""
Ollama Refiner for Rush 2049 Decompilation
Takes existing pseudo-C and refines it with proper types, structs, and names.
Designed for ~2 hours of processing.
"""

import json
import urllib.request
import time
import re
import os
from pathlib import Path

# Configuration
OLLAMA_URL = "http://192.168.50.7:11434/api/generate"
MODEL = "qwen2.5-coder:7b"
CONTEXT_SIZE = 16384
OUTPUT_DIR = Path("/home/cburnes/projects/rush2049-decomp/ollama_analysis")
DISASM_FILE = Path("/home/cburnes/projects/rush2049-decomp/build/game_code_disasm.txt")

# Known globals and structs
SYSTEM_PROMPT = """You are refining N64 MIPS decompilation for Rush 2049.

KNOWN GLOBALS:
- gstate = 0x801146EC (u8 game state flags)
- gstate_prev = 0x801146ED (u8 previous state)
- car_array = 0x80152818 (CarData[8])
- game_struct = 0x801461D0 (GameStruct)
- frame_counter = 0x80142AFC (u32)
- pad_array = 0x80140BF0 (PadEntry[4])

KNOWN STRUCTS:
typedef struct CarData {
    f32 dr_pos[3];      // 0x00: Dead reckoned position
    f32 dr_vel[3];      // 0x0C: Dead reckoned velocity
    f32 dr_uvs[3][3];   // 0x18: Dead reckoned orientation
    f32 RWV[3];         // Real world velocity
    f32 RWR[3];         // Real world position
    f32 mph;            // Speedometer reading
    s16 rpm;            // Engine RPM
    u8 data_valid;      // Data updated flag
    s8 place;           // Race position
    s8 laps;            // Current lap
    s8 checkpoint;      // Current checkpoint
    s8 difficulty;      // AI difficulty (DRONE_HUMAN=2)
} CarData;

STATE FLAGS:
#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

TYPES: Use s8/s16/s32/u8/u16/u32/f32 instead of int/float/char.

RULES:
1. Replace raw addresses with named globals
2. Use proper struct member access (car->mph not *(car+offset))
3. Name local variables descriptively
4. Add brief comments for non-obvious logic
5. Output ONLY compilable C code - no explanation
"""

def send_to_ollama(prompt, timeout=180):
    """Send prompt to Ollama and get response."""
    data = json.dumps({
        'model': MODEL,
        'prompt': prompt,
        'stream': False,
        'options': {'num_ctx': CONTEXT_SIZE}
    }).encode()

    req = urllib.request.Request(OLLAMA_URL, data=data)
    req.add_header('Content-Type', 'application/json')

    try:
        with urllib.request.urlopen(req, timeout=timeout) as resp:
            result = json.load(resp)
            return result.get('response', ''), None
    except Exception as e:
        return None, str(e)

def extract_functions_from_file(filepath):
    """Extract function blocks from existing decompiled file."""
    functions = []
    current_func = None
    current_code = []

    with open(filepath) as f:
        for line in f:
            # New function header
            if line.startswith('// === func_') or line.startswith('=== func_'):
                if current_func and current_code:
                    functions.append((current_func, '\n'.join(current_code)))
                # Extract address
                match = re.search(r'func_([0-9A-Fa-f]+)', line)
                if match:
                    current_func = match.group(1)
                    current_code = [line]
            elif current_func:
                current_code.append(line)

    # Last function
    if current_func and current_code:
        functions.append((current_func, '\n'.join(current_code)))

    return functions

def get_assembly_snippet(addr, lines=40):
    """Get assembly for the function."""
    try:
        with open(DISASM_FILE) as f:
            disasm = f.read()
        pattern = rf'^{addr}:.*'
        match = re.search(pattern, disasm, re.MULTILINE | re.IGNORECASE)
        if match:
            start = match.start()
            snippet_lines = disasm[start:].split('\n')[:lines]
            return '\n'.join(snippet_lines)
    except:
        pass
    return f"{addr}: (assembly not found)"

def refine_function(addr, pseudo_c, assembly):
    """Refine a single function's decompilation."""
    prompt = f"""{SYSTEM_PROMPT}

TASK: Refine this pseudo-C decompilation of func_{addr}.

CURRENT PSEUDO-C:
{pseudo_c[:2000]}

ORIGINAL ASSEMBLY:
{assembly}

OUTPUT: Refined, compilable C code with proper types and names. No explanation."""

    return send_to_ollama(prompt, timeout=120)

def main():
    print("=" * 60)
    print("Rush 2049 Ollama Refiner")
    print("=" * 60)

    # Find files to process
    source_files = [
        OUTPUT_DIR / "overnight_decompiled.txt",
        OUTPUT_DIR / "overnight2_decompiled.txt",
        OUTPUT_DIR / "decompiled_functions.txt",
        OUTPUT_DIR / "batch2_refined.txt",
        OUTPUT_DIR / "batch3_decompiled.txt",
    ]

    all_functions = []
    for sf in source_files:
        if sf.exists():
            funcs = extract_functions_from_file(sf)
            print(f"Loaded {len(funcs)} functions from {sf.name}")
            all_functions.extend(funcs)

    print(f"\nTotal functions to refine: {len(all_functions)}")

    # Remove duplicates, keep latest
    seen = {}
    for addr, code in all_functions:
        seen[addr] = code
    all_functions = list(seen.items())
    print(f"Unique functions: {len(all_functions)}")

    # Output file
    output_file = OUTPUT_DIR / "refined_c_code.txt"

    # Process with progress
    start_time = time.time()
    processed = 0
    errors = 0

    with open(output_file, 'w') as out:
        out.write("// Rush 2049 Refined C Decompilation\n")
        out.write(f"// Generated: {time.strftime('%Y-%m-%d %H:%M:%S')}\n")
        out.write("// " + "=" * 56 + "\n\n")

        for addr, pseudo_c in all_functions:
            processed += 1
            elapsed = time.time() - start_time
            rate = processed / elapsed if elapsed > 0 else 0
            remaining = (len(all_functions) - processed) / rate if rate > 0 else 0

            print(f"[{processed}/{len(all_functions)}] Refining func_{addr}... ", end='', flush=True)
            print(f"(ETA: {remaining/60:.1f}min)")

            # Get assembly
            assembly = get_assembly_snippet(addr)

            # Refine
            result, error = refine_function(addr, pseudo_c, assembly)

            if error:
                print(f"  ERROR: {error}")
                errors += 1
                out.write(f"\n// === func_{addr} ===\n")
                out.write(f"// ERROR: {error}\n")
                out.write(f"// Original pseudo-C:\n{pseudo_c[:1000]}\n")
            else:
                out.write(f"\n// === func_{addr} @ 0x{addr} ===\n")
                out.write(result)
                out.write("\n")

            out.flush()

            # Rate limit
            time.sleep(1)

    total_time = time.time() - start_time
    print("\n" + "=" * 60)
    print(f"COMPLETE!")
    print(f"Processed: {processed} functions")
    print(f"Errors: {errors}")
    print(f"Total time: {total_time/60:.1f} minutes")
    print(f"Output: {output_file}")

if __name__ == "__main__":
    main()
