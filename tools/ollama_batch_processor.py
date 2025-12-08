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
        print("  python ollama_batch_processor.py decompile ADDR1 ADDR2 ...  - Decompile specific")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "classify":
        run_classification_job()
    elif cmd == "priority":
        run_priority_functions_impl()
    elif cmd == "batch2":
        run_batch2_functions()
    elif cmd == "decompile":
        if len(sys.argv) < 3:
            print("Provide function addresses to decompile")
            sys.exit(1)
        run_decompile_job(sys.argv[2:])
    else:
        print(f"Unknown command: {cmd}")
