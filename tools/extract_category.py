#!/usr/bin/env python3
"""
Extract functions by category from refined_c_code_clean.txt
Creates organized source files for physics, render, AI, etc.
"""

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path("/home/cburnes/projects/rush2049-decomp")
INPUT_FILE = PROJECT_ROOT / "ollama_analysis/refined_c_code_clean.txt"

# Function address ranges by category (based on CLAUDE.md and analysis)
CATEGORIES = {
    "physics": {
        "ranges": [(0x800B8000, 0x800BBFFF)],  # Physics/car update functions
        "output": "src/game/physics.c",
        "description": "Car physics, forces, tire simulation, suspension",
    },
    "collision": {
        "ranges": [(0x800BB000, 0x800BBFFF)],  # Collision detection
        "output": "src/game/collision.c",
        "description": "Collision detection and response",
    },
    "render": {
        "ranges": [(0x80087000, 0x8009FFFF)],  # Rendering functions
        "output": "src/game/render.c",
        "description": "3D rendering, display lists, visuals",
    },
    "ai": {
        "ranges": [(0x800A0000, 0x800A1FFF)],  # AI/drone functions
        "output": "src/game/ai.c",
        "description": "AI drones, pathfinding, steering",
    },
    "camera": {
        "ranges": [(0x800A0400, 0x800A0FFF)],  # Camera functions
        "output": "src/game/camera.c",
        "description": "Camera system, chase/cockpit views",
    },
    "checkpoint": {
        "ranges": [(0x800C0000, 0x800C0FFF)],  # Checkpoint/lap functions
        "output": "src/game/checkpoint.c",
        "description": "Checkpoint detection, lap counting",
    },
    "game_state": {
        "ranges": [(0x800CA000, 0x800CAFFF), (0x800DB000, 0x800DBFFF),
                   (0x800FB000, 0x800FDFFF)],  # Game state handlers
        "output": "src/game/state.c",
        "description": "Game state machine, attract, playgame, countdown",
    },
    "audio": {
        "ranges": [(0x80095000, 0x80095FFF), (0x800B3700, 0x800B3FFF)],
        "output": "src/game/audio.c",
        "description": "Sound effects, music playback",
    },
}

def parse_functions(content):
    """Extract function blocks from the refined C file."""
    functions = {}

    # Pattern to match function headers like "// === func_800B8000 @ 0x800B8000 ==="
    pattern = r'// === func_([0-9A-Fa-f]+)(?: @ 0x[0-9A-Fa-f]+)? ==='

    lines = content.split('\n')
    current_addr = None
    current_code = []

    for i, line in enumerate(lines):
        match = re.match(pattern, line)
        if match:
            # Save previous function
            if current_addr and current_code:
                functions[current_addr] = '\n'.join(current_code)

            current_addr = int(match.group(1), 16)
            current_code = [line]
        elif current_addr:
            current_code.append(line)

    # Save last function
    if current_addr and current_code:
        functions[current_addr] = '\n'.join(current_code)

    return functions

def clean_function(code):
    """Clean up common Ollama artifacts in generated code."""
    # Remove inline assembly blocks (Ollama sometimes generates these)
    code = re.sub(r'__asm__\s*volatile\s*\([^)]+\);?\s*', '', code)
    code = re.sub(r'__asm__\s*\([^)]+\);?\s*', '', code)

    # Remove duplicate function header comments
    lines = code.split('\n')
    seen_header = False
    cleaned_lines = []
    for line in lines:
        if '// === func_' in line:
            if not seen_header:
                seen_header = True
                cleaned_lines.append(line)
        else:
            cleaned_lines.append(line)

    return '\n'.join(cleaned_lines)

def get_category_functions(functions, category):
    """Get functions that fall within the address ranges for a category."""
    ranges = CATEGORIES[category]["ranges"]
    result = {}

    for addr, code in functions.items():
        for start, end in ranges:
            if start <= addr <= end:
                result[addr] = code
                break

    return result

def generate_header(category):
    """Generate file header for a category."""
    info = CATEGORIES[category]
    return f"""/**
 * Rush 2049 - {category.title()} Functions
 * {info['description']}
 *
 * Auto-extracted from Ollama decompilation output
 * Source: ollama_analysis/refined_c_code_clean.txt
 */

#include "types.h"
#include "game/structs.h"

/* External declarations */
extern u8 gstate;
extern u8 gstate_prev;
extern CarData car_array[];
extern GameStruct* game_struct;
extern u32 frame_counter;
extern PadEntry pad_array[];

"""

def main():
    if len(sys.argv) < 2:
        print("Usage: extract_category.py <category>")
        print("Categories:", ", ".join(CATEGORIES.keys()))
        print("\nOr: extract_category.py all")
        sys.exit(1)

    category = sys.argv[1]

    # Load input file
    print(f"Loading {INPUT_FILE}...")
    with open(INPUT_FILE) as f:
        content = f.read()

    # Parse all functions
    print("Parsing functions...")
    all_functions = parse_functions(content)
    print(f"Found {len(all_functions)} total functions")

    if category == "all":
        categories_to_process = list(CATEGORIES.keys())
    elif category in CATEGORIES:
        categories_to_process = [category]
    else:
        print(f"Unknown category: {category}")
        print("Available:", ", ".join(CATEGORIES.keys()))
        sys.exit(1)

    for cat in categories_to_process:
        info = CATEGORIES[cat]
        output_path = PROJECT_ROOT / info["output"]

        # Get functions for this category
        cat_functions = get_category_functions(all_functions, cat)

        if not cat_functions:
            print(f"\n{cat}: No functions found in ranges")
            continue

        print(f"\n{cat}: Found {len(cat_functions)} functions")

        # Sort by address
        sorted_addrs = sorted(cat_functions.keys())

        # Generate output
        output = generate_header(cat)

        for addr in sorted_addrs:
            code = clean_function(cat_functions[addr])
            output += f"\n{code}\n"

        # Ensure directory exists
        output_path.parent.mkdir(parents=True, exist_ok=True)

        # Write file
        with open(output_path, 'w') as f:
            f.write(output)

        print(f"  Wrote {output_path}")
        print(f"  Address range: 0x{sorted_addrs[0]:08X} - 0x{sorted_addrs[-1]:08X}")

if __name__ == "__main__":
    main()
