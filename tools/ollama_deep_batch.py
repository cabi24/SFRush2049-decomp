#!/usr/bin/env python3
"""
Ollama Deep Batch - High-quality decompilation of critical game functions
With full arcade source context for best results.
Estimated: 2 hours for ~200 priority functions.
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
PROJECT_ROOT = Path("/home/cburnes/projects/rush2049-decomp")
OUTPUT_DIR = PROJECT_ROOT / "ollama_analysis"
DISASM_FILE = PROJECT_ROOT / "build/game_code_disasm.txt"
ARCADE_DIR = PROJECT_ROOT / "reference/repos/rushtherock"

# Priority function categories with arcade file mappings
PRIORITY_FUNCS = {
    # Game state machine (game/game.c)
    "game_state": {
        "arcade_file": "game/game.c",
        "functions": [
            ("800FD464", "game_loop", "main per-frame game logic"),
            ("800CA3B4", "playgame", "active racing handler"),
            ("800DB81C", "attract", "demo/title screen"),
            ("800FBC30", "countdown", "race countdown"),
            ("800FBF88", "high_score_entry", "high score handler"),
            ("800FBF90", "state_init_handler", "init state"),
            ("800FBFE4", "state_setup_handler", "setup state"),
            ("800FC0EC", "state_menu_handler", "menu state"),
        ]
    },
    # Physics/cars (game/cars.c)
    "physics": {
        "arcade_file": "game/cars.c",
        "functions": [
            ("800B8C14", "update_car_physics", "car physics update"),
            ("800B9000", "apply_forces", "apply physics forces"),
            ("800B9400", "integrate_position", "position integration"),
            ("800BA000", "tire_physics", "tire simulation"),
            ("800BA800", "suspension_update", "suspension calc"),
        ]
    },
    # Collision (game/collision.c)
    "collision": {
        "arcade_file": "game/collision.c",
        "functions": [
            ("800BB000", "check_collisions", "main collision check"),
            ("800BB400", "car_car_collision", "car-to-car"),
            ("800BB800", "car_wall_collision", "car-to-wall"),
            ("800BBC00", "point_in_body", "point inside test"),
        ]
    },
    # AI/Drones (game/drones.c)
    "ai": {
        "arcade_file": "game/drones.c",
        "functions": [
            ("800A0000", "do_drones", "AI main update"),
            ("800A0800", "drone_steering", "AI steering logic"),
            ("800A1000", "path_follow", "path following"),
            ("800A1800", "avoid_obstacles", "obstacle avoidance"),
        ]
    },
    # Checkpoints (game/checkpoint.c)
    "checkpoint": {
        "arcade_file": "game/checkpoint.c",
        "functions": [
            ("800C0000", "check_cps", "checkpoint detection"),
            ("800C0400", "init_cps", "checkpoint init"),
            ("800C0800", "update_lap", "lap counting"),
        ]
    },
    # Camera (game/camera.c)
    "camera": {
        "arcade_file": "game/camera.c",
        "functions": [
            ("800A04C4", "camera_update", "camera system"),
            ("800A0800", "chase_camera", "chase camera mode"),
            ("800A0C00", "cockpit_camera", "cockpit view"),
        ]
    },
    # Rendering (game/visuals.c)
    "render": {
        "arcade_file": "game/visuals.c",
        "functions": [
            ("80099BFC", "render_object", "3D model render"),
            ("80087A08", "render_large", "major render func"),
            ("8009C000", "render_car", "car rendering"),
            ("8009D000", "render_wheel", "wheel rendering"),
            ("8009E000", "render_shadow", "shadow rendering"),
        ]
    },
    # Audio
    "audio": {
        "arcade_file": "game/sound.c",
        "functions": [
            ("800B37E8", "sound_control", "main audio control"),
            ("8008A750", "audio_wrapper", "audio forwarding"),
            ("80095360", "play_sfx", "sound effect play"),
            ("80095960", "bgm_playback", "music playback"),
        ]
    },
    # Input
    "input": {
        "arcade_file": None,  # N64-specific
        "functions": [
            ("800C9AE0", "process_input", "input processing"),
            ("800140B0", "read_controllers", "controller read"),
        ]
    },
}

def load_arcade_source(category):
    """Load relevant arcade source for context."""
    info = PRIORITY_FUNCS.get(category, {})
    arcade_file = info.get("arcade_file")
    if not arcade_file:
        return None

    filepath = ARCADE_DIR / arcade_file
    if filepath.exists():
        try:
            with open(filepath) as f:
                content = f.read()
            # Truncate to reasonable size for context
            if len(content) > 8000:
                content = content[:8000] + "\n// ... (truncated)"
            return content
        except:
            pass
    return None

def get_assembly_snippet(addr, lines=60):
    """Get assembly for a function."""
    try:
        with open(DISASM_FILE) as f:
            disasm = f.read()
        # Try with leading 8
        pattern = rf'^{addr}:.*'
        match = re.search(pattern, disasm, re.MULTILINE | re.IGNORECASE)
        if match:
            start = match.start()
            snippet_lines = disasm[start:].split('\n')[:lines]
            return '\n'.join(snippet_lines)
    except Exception as e:
        pass
    return f"{addr}: (assembly not found)"

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

def decompile_function(addr, name, desc, category, arcade_src):
    """Decompile a single function with context."""
    assembly = get_assembly_snippet(addr)

    prompt = f"""Decompile N64 MIPS function to C for Rush 2049.

FUNCTION: {name} @ 0x{addr}
CATEGORY: {category}
DESCRIPTION: {desc}

KNOWN GLOBALS:
- gstate = 0x801146EC (u8 game state)
- car_array = 0x80152818 (CarData[8])
- game_struct = 0x801461D0 (GameStruct)
- frame_counter = 0x80142AFC (u32)

TYPES: s8/s16/s32/u8/u16/u32/f32

"""
    if arcade_src:
        prompt += f"""ARCADE REFERENCE ({category}):
```c
{arcade_src[:4000]}
```

"""

    prompt += f"""ASSEMBLY:
{assembly}

OUTPUT: Compilable C function matching arcade style. No explanation."""

    return send_to_ollama(prompt, timeout=120)

def main():
    print("=" * 60)
    print("Rush 2049 Deep Batch Decompilation")
    print("Priority functions with arcade context")
    print("=" * 60)

    # Collect all functions to process
    all_funcs = []
    for category, info in PRIORITY_FUNCS.items():
        for addr, name, desc in info.get("functions", []):
            all_funcs.append((category, addr, name, desc))

    print(f"\nTotal priority functions: {len(all_funcs)}")

    # Load arcade sources
    arcade_sources = {}
    for category in PRIORITY_FUNCS:
        src = load_arcade_source(category)
        if src:
            arcade_sources[category] = src
            print(f"  Loaded arcade source for {category}")

    # Output file
    output_file = OUTPUT_DIR / "deep_batch_output.txt"
    output_c = PROJECT_ROOT / "src/game/decompiled_funcs.c"

    start_time = time.time()
    processed = 0
    errors = 0

    with open(output_file, 'w') as out, open(output_c, 'w') as out_c:
        # Header
        header = f"""/**
 * Rush 2049 Decompiled Functions
 * Generated: {time.strftime('%Y-%m-%d %H:%M:%S')}
 * Source: Ollama deep batch with arcade context
 */

#include "types.h"
#include "game/structs.h"

/* External declarations */
extern u8 gstate;
extern u8 gstate_prev;
extern CarData car_array[];
extern GameStruct game_struct;
extern u32 frame_counter;

"""
        out.write(header)
        out_c.write(header)

        for category, addr, name, desc in all_funcs:
            processed += 1
            elapsed = time.time() - start_time
            rate = processed / elapsed if elapsed > 0 else 0
            remaining = (len(all_funcs) - processed) / rate if rate > 0 else 0

            print(f"[{processed}/{len(all_funcs)}] {name} @ 0x{addr} ({category})")
            print(f"  ETA: {remaining/60:.1f} min remaining")

            arcade_src = arcade_sources.get(category)
            result, error = decompile_function(addr, name, desc, category, arcade_src)

            separator = f"\n// ============================================================\n"
            header_comment = f"// Function: {name}\n// Address: 0x{addr}\n// Category: {category} - {desc}\n"

            if error:
                print(f"  ERROR: {error}")
                errors += 1
                out.write(separator + header_comment + f"// ERROR: {error}\n")
            else:
                out.write(separator + header_comment + result + "\n")
                # Also write to C file if it looks like valid C
                if "void" in result or "int" in result or "f32" in result:
                    out_c.write(separator + header_comment + result + "\n")

            out.flush()
            out_c.flush()

            # Rate limit
            time.sleep(2)

    total_time = time.time() - start_time
    print("\n" + "=" * 60)
    print("DEEP BATCH COMPLETE!")
    print(f"Processed: {processed} functions")
    print(f"Errors: {errors}")
    print(f"Total time: {total_time/60:.1f} minutes")
    print(f"Output: {output_file}")
    print(f"C file: {output_c}")

if __name__ == "__main__":
    main()
