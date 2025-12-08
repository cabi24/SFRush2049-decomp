#!/usr/bin/env python3
"""
Ollama Batch Processor for Rush 2049 Decompilation
Processes game functions in batches with 16K context window.
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
CONTEXT_SIZE = 16384  # 16K context
BATCH_SIZE = 25       # Functions per batch (bigger with 16K)
ASM_LINES_PER_FUNC = 20  # Assembly lines to include per function

# Overnight batch settings - stay within 16K context
# ~20 lines ASM * ~50 chars = ~1000 chars/func
# 16K context = ~12K usable for prompt = ~12 functions per batch safely
OVERNIGHT_BATCH_SIZE = 10  # Conservative to avoid context overflow
OVERNIGHT_FUNCS_TARGET = 400  # Process all remaining functions
OUTPUT_DIR = Path("/home/cburnes/projects/rush2049-decomp/ollama_analysis")
DISASM_FILE = Path("/home/cburnes/projects/rush2049-decomp/build/game_code_disasm.txt")

def load_disasm():
    """Load the full disassembly file."""
    with open(DISASM_FILE) as f:
        return f.read()

def get_all_functions(disasm):
    """Extract all function addresses from disassembly."""
    # Look for func_XXXXXXXX throughout the file
    pattern = r'func_([0-9A-Fa-f]{8})'
    matches = re.findall(pattern, disasm)
    if matches:
        return sorted(set(matches))
    # Fallback: look for address patterns at line starts
    pattern = r'^([0-9A-Fa-f]{8}):'
    matches = re.findall(pattern, disasm, re.MULTILINE)
    # Sample every ~100 addresses to get function starts
    return sorted(set(matches[::100]))

def get_function_snippet(disasm, addr, lines=20):
    """Get N lines of assembly starting at address."""
    pattern = rf'^{addr}:.*'
    match = re.search(pattern, disasm, re.MULTILINE | re.IGNORECASE)
    if match:
        start = match.start()
        snippet_lines = disasm[start:].split('\n')[:lines]
        return '\n'.join(snippet_lines)
    return f"{addr}: (not found)"

def send_to_ollama(prompt, timeout=120):
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

def classify_batch(functions, disasm, batch_num):
    """Classify a batch of functions."""
    prompt_lines = [
        "Classify each N64 racing game function by its likely PURPOSE (2-5 words).",
        "Look for patterns: graphics (lui 0xE7/0xFC), audio (lui 0x8013), physics (float ops), AI (loops+branches).",
        "Format: ADDRESS: purpose_description",
        ""
    ]

    for addr in functions:
        snippet = get_function_snippet(disasm, addr, ASM_LINES_PER_FUNC)
        prompt_lines.append(f"--- {addr} ---")
        prompt_lines.append(snippet)
        prompt_lines.append("")

    prompt = '\n'.join(prompt_lines)
    print(f"  Batch {batch_num}: {len(functions)} functions, {len(prompt)} chars")

    response, error = send_to_ollama(prompt)
    if error:
        return f"ERROR: {error}"
    return response

def decompile_function(addr, disasm, context=""):
    """Decompile a single function with more context."""
    snippet = get_function_snippet(disasm, addr, 80)  # More lines for decompilation

    prompt = f"""Decompile this N64 MIPS function to C code.
Game: San Francisco Rush 2049 (racing game)
Known globals: 0x801146EC=gstate, 0x80152818=car_array, 0x801461D0=game_struct

{context}

Assembly for func_{addr}:
{snippet}

Output clean C code only, no explanations."""

    response, error = send_to_ollama(prompt, timeout=180)
    if error:
        return f"// ERROR: {error}"
    return response

def run_classification_job(output_file="all_functions_classified.txt"):
    """Run full classification of all game functions."""
    print("Loading disassembly...")
    disasm = load_disasm()

    print("Finding all functions...")
    all_funcs = get_all_functions(disasm)
    print(f"Found {len(all_funcs)} functions")

    results = []
    total_batches = (len(all_funcs) + BATCH_SIZE - 1) // BATCH_SIZE

    print(f"\nProcessing in {total_batches} batches of {BATCH_SIZE}...")

    for i in range(0, len(all_funcs), BATCH_SIZE):
        batch = all_funcs[i:i + BATCH_SIZE]
        batch_num = i // BATCH_SIZE + 1

        result = classify_batch(batch, disasm, batch_num)
        results.append(f"=== BATCH {batch_num}/{total_batches} ===\n{result}")

        # Progress save every 5 batches
        if batch_num % 5 == 0:
            save_results(results, output_file + ".partial")
            print(f"  Progress saved ({batch_num}/{total_batches})")

        time.sleep(0.5)  # Brief pause between batches

    save_results(results, output_file)
    print(f"\nDone! Results saved to {OUTPUT_DIR / output_file}")
    return results

def run_decompile_job(addresses, output_file="decompiled_functions.txt", context=""):
    """Decompile specific functions."""
    print("Loading disassembly...")
    disasm = load_disasm()

    results = []
    for i, addr in enumerate(addresses):
        print(f"Decompiling {addr} ({i+1}/{len(addresses)})...")
        result = decompile_function(addr, disasm, context)
        results.append(f"// === func_{addr} ===\n{result}\n")
        time.sleep(0.5)

    save_results(results, output_file)
    print(f"\nDone! Results saved to {OUTPUT_DIR / output_file}")
    return results

def save_results(results, filename):
    """Save results to output directory."""
    OUTPUT_DIR.mkdir(exist_ok=True)
    with open(OUTPUT_DIR / filename, 'w') as f:
        f.write('\n\n'.join(results))

def run_priority_functions():
    """Decompile high-priority game functions identified earlier."""
    priority = [
        ("800FD464", "game_loop - main state machine"),
        ("800CA3B4", "game_update - per-frame gameplay"),
        ("800DB81C", "attract - demo/title screen"),
        ("80099BFC", "render_object - 3D rendering (10KB)"),
        ("800A04C4", "render_scene - viewport setup"),
        ("800B37E8", "audio_control - sound system"),
        ("800C9AE0", "input_handler - controller/init"),
        ("800FBF88", "countdown - race start"),
        ("800FBC30", "preplay - pre-race setup"),
    ]

def run_batch2_functions():
    """Decompile second batch - physics, collision, and helpers."""
    batch2 = [
        ("80087A08", "render_large - 10KB major rendering"),
        ("800974EC", "audio_processing - sound helper"),
        ("80097798", "vehicle_physics - collision detection"),
        ("800B8C14", "collision_check - car collision"),
        ("800BAE24", "physics_calc - physics calculations"),
        ("800BD2C8", "physics_helper - called from countdown"),
        ("800BEAA0", "race_logic - called from countdown"),
        ("800C997C", "screen_update - state update"),
        ("800EC0DC", "state_helper - called from countdown"),
        ("800F8D9C", "race_helper - called from countdown"),
        ("800F93A0", "large_unknown - 5.6KB function"),
        ("800C85F0", "checkpoint_check - checkpoint area"),
        ("800C8A00", "track_logic - track handling"),
    ]

    print("Loading disassembly...")
    disasm = load_disasm()

    results = []
    for addr, desc in batch2:
        print(f"Decompiling {addr} ({desc})...")
        context = f"This function is: {desc}"
        result = decompile_function(addr, disasm, context)
        results.append(f"// === {addr}: {desc} ===\n{result}\n")
        time.sleep(1)

    save_results(results, "batch2_decompiled.txt")
    print(f"\nDone! Batch 2 functions saved.")


def run_batch3_functions():
    """Decompile third batch - state handlers and game flow functions."""
    batch3 = [
        # State machine handlers (from symbol_addrs.us.txt)
        ("800FBF90", "state_init_handler - GSTATE_INIT 0x00040000"),
        ("800FBFE4", "state_setup_handler - GSTATE_SETUP 0x00080000"),
        ("800FC0EC", "state_menu_handler - GSTATE_MENU 0x00100000"),
        ("800FC038", "state_camera_handler - GSTATE_CAMERA 0x00200000"),
        ("800FC228", "state_ready_handler - GSTATE_READY 0x00400000"),
        ("800FC25C", "state_play_handler - GSTATE_PLAY 0x00800000"),
        ("800FC89C", "state_cars_handler - GSTATE_CARS 0x01000000"),
        ("800FC960", "state_finish_handler - GSTATE_FINISH 0x02000000"),
        # Camera functions
        ("80097CA0", "camera_view_transform - camera and view"),
        ("80098574", "camera_update_nested - camera update helper"),
        ("800F8D9C", "camera_race_setup - camera setup for race"),
        # Scene/object update
        ("800F733C", "UpdateActiveObjects - per-frame scene object dispatcher"),
        ("800FD238", "state_dispatch - state machine bitmask ladder"),
        ("800DE4DC", "vi_retrace_callback - video timing config"),
    ]


def run_batch4_functions():
    """Decompile fourth batch - AI, drones, and race logic."""
    batch4 = [
        # AI/Drone functions (from classification showing AI patterns)
        ("800C0000", "drone_init - AI drone initialization"),
        ("800C0400", "drone_update - per-frame drone AI"),
        ("800C1000", "pathfind_calc - pathfinding calculation"),
        ("800C1800", "drone_steering - AI steering logic"),
        # Race/checkpoint logic
        ("800C85F0", "checkpoint_check - checkpoint collision"),
        ("800C8A00", "track_position - race position calc"),
        ("800C8E00", "lap_complete - lap completion handler"),
        # Menu/UI functions
        ("800D0000", "menu_draw - menu rendering"),
        ("800D2000", "menu_input - menu input handler"),
        ("800D4000", "hud_draw - in-race HUD"),
        # Audio helpers
        ("800B37E8", "audio_control - sound system control"),
        ("800B4000", "sfx_play - sound effect trigger"),
        ("800B4800", "music_control - music playback"),
        # Physics helpers
        ("800BAE24", "physics_integrate - physics integration"),
    ]

    print("Loading disassembly...")
    disasm = load_disasm()

    results = []
    for addr, desc in batch4:
        print(f"Decompiling {addr} ({desc})...")
        context = f"This function is: {desc}"
        result = decompile_function(addr, disasm, context)
        results.append(f"// === {addr}: {desc} ===\n{result}\n")
        time.sleep(1)

    save_results(results, "batch4_decompiled.txt")
    print(f"\nDone! Batch 4 functions saved.")


def get_processed_addresses():
    """Get set of already-processed function addresses."""
    processed = set()
    for filename in OUTPUT_DIR.glob("*_decompiled.txt"):
        content = filename.read_text()
        # Match patterns like "// === 800FD464:" or "// === func_800FD464"
        for match in re.finditer(r'===\s*(?:func_)?([0-9A-Fa-f]{8})', content):
            processed.add(match.group(1).upper())
    return processed

def run_overnight_batch():
    """Run overnight decompilation of ~350 functions (one at a time for reliability)."""
    print("=== OVERNIGHT BATCH JOB ===")
    print(f"Target: {OVERNIGHT_FUNCS_TARGET} functions")
    print(f"Context: {CONTEXT_SIZE} tokens (16K)")
    print()

    print("Loading disassembly...")
    disasm = load_disasm()

    print("Finding all functions...")
    all_funcs = get_all_functions(disasm)
    print(f"Found {len(all_funcs)} total functions")

    print("Checking already processed...")
    processed = get_processed_addresses()
    print(f"Already processed: {len(processed)} functions")

    # Filter to unprocessed functions
    remaining = [f for f in all_funcs if f.upper() not in processed]
    print(f"Remaining unprocessed: {len(remaining)} functions")

    # Take first OVERNIGHT_FUNCS_TARGET
    to_process = remaining[:OVERNIGHT_FUNCS_TARGET]
    print(f"Will process: {len(to_process)} functions")
    print()

    if not to_process:
        print("No functions to process!")
        return

    results = []
    start_time = time.time()

    for i, addr in enumerate(to_process):
        elapsed = time.time() - start_time
        rate = (i / elapsed * 3600) if elapsed > 0 and i > 0 else 0
        eta_hours = ((len(to_process) - i) / rate) if rate > 0 else 0

        print(f"[{i+1}/{len(to_process)}] Decompiling {addr} "
              f"({rate:.0f}/hr, ETA: {eta_hours:.1f}h)...")

        result = decompile_function(addr, disasm)
        results.append(f"// === func_{addr} ===\n{result}\n")

        # Save progress every 25 functions
        if (i + 1) % 25 == 0:
            save_results(results, "overnight2_decompiled.txt.partial")
            print(f"  Progress saved ({i+1}/{len(to_process)})")

        time.sleep(0.5)  # Brief pause between requests

    # Final save
    save_results(results, "overnight2_decompiled.txt")
    elapsed = time.time() - start_time
    print(f"\n=== OVERNIGHT BATCH COMPLETE ===")
    print(f"Processed: {len(to_process)} functions")
    print(f"Time: {elapsed/3600:.1f} hours")
    print(f"Output: {OUTPUT_DIR / 'overnight_decompiled.txt'}")

def run_priority_functions_impl():
    """Actually run priority decompilation."""
    priority = [
        ("800FD464", "game_loop - main state machine"),
        ("800CA3B4", "game_update - per-frame gameplay"),
        ("800DB81C", "attract - demo/title screen"),
        ("80099BFC", "render_object - 3D rendering (10KB)"),
        ("800A04C4", "render_scene - viewport setup"),
        ("800B37E8", "audio_control - sound system"),
        ("800C9AE0", "input_handler - controller/init"),
        ("800FBF88", "countdown - race start"),
        ("800FBC30", "preplay - pre-race setup"),
    ]

    print("Loading disassembly...")
    disasm = load_disasm()

    results = []
    for addr, desc in priority:
        print(f"Decompiling {addr} ({desc})...")
        context = f"This function is: {desc}"
        result = decompile_function(addr, disasm, context)
        results.append(f"// === {addr}: {desc} ===\n{result}\n")
        time.sleep(1)

    save_results(results, "priority_decompiled.txt")
    print(f"\nDone! Priority functions saved.")

if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print("Usage:")
        print("  python ollama_batch_processor.py classify    - Classify ALL functions")
        print("  python ollama_batch_processor.py priority    - Decompile priority functions")
        print("  python ollama_batch_processor.py overnight   - Decompile 350 functions (multi-hour job)")
        print("  python ollama_batch_processor.py decompile ADDR1 ADDR2 ...  - Decompile specific")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "classify":
        run_classification_job()
    elif cmd == "priority":
        run_priority_functions_impl()
    elif cmd == "batch2":
        run_batch2_functions()
    elif cmd == "batch3":
        run_batch3_functions()
    elif cmd == "batch4":
        run_batch4_functions()
    elif cmd == "overnight":
        run_overnight_batch()
    elif cmd == "decompile":
        if len(sys.argv) < 3:
            print("Provide function addresses to decompile")
            sys.exit(1)
        run_decompile_job(sys.argv[2:])
    else:
        print(f"Unknown command: {cmd}")
