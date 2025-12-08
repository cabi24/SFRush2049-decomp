#!/usr/bin/env python3
"""
local_llm_prompts.py - Generate bite-sized prompts for local LLM processing

Creates small, focused prompts that work well with 4-8K context models.
Output can be fed to Ollama, llama.cpp, or any local inference setup.
"""

import os
import re
import sys
import json
from pathlib import Path

# MIPS register names for context
MIPS_CONTEXT = """MIPS N64 Register Conventions:
- a0-a3: function arguments
- v0-v1: return values
- t0-t9: temporaries (caller-saved)
- s0-s7: saved (callee-saved)
- sp: stack pointer, fp: frame pointer, ra: return address
- f0-f31: floating point registers (f12-f14 for args, f0 for return)

Common N64 patterns:
- addiu $sp, $sp, -N = function prologue (allocate stack)
- jr $ra = function return
- jal 0x800XXXXX = function call
- lui + ori/addiu = load 32-bit address
- lw/sw = load/store word, lb/sb = byte, lh/sh = halfword
"""

def extract_functions_from_asm(asm_content, base_addr=0x80000000):
    """Extract individual functions from assembly file."""
    functions = []
    lines = asm_content.split('\n')

    current_func = None
    current_lines = []
    current_addr = None

    for line in lines:
        # Look for function labels like "glabel func_80001234" or "func_80001234:"
        func_match = re.match(r'(?:glabel\s+)?(func_[0-9A-Fa-f]+|[a-zA-Z_][a-zA-Z0-9_]*):', line)
        if not func_match:
            func_match = re.match(r'glabel\s+(func_[0-9A-Fa-f]+|[a-zA-Z_][a-zA-Z0-9_]*)', line)

        if func_match:
            # Save previous function
            if current_func and current_lines:
                functions.append({
                    'name': current_func,
                    'addr': current_addr,
                    'lines': current_lines,
                    'size': len(current_lines) * 4  # Approximate
                })

            current_func = func_match.group(1)
            # Extract address from function name if possible
            addr_match = re.search(r'[0-9A-Fa-f]{8}', current_func)
            current_addr = int(addr_match.group(), 16) if addr_match else None
            current_lines = [line]
        elif current_func:
            current_lines.append(line)
            # End function on jr $ra + delay slot
            if 'jr' in line and '$ra' in line:
                # Include one more line for delay slot
                continue

    # Don't forget last function
    if current_func and current_lines:
        functions.append({
            'name': current_func,
            'addr': current_addr,
            'lines': current_lines,
            'size': len(current_lines) * 4
        })

    return functions

def generate_decompile_prompt(func, known_symbols=None):
    """Generate a decompilation prompt for a single function."""
    asm_text = '\n'.join(func['lines'][:100])  # Limit to ~100 lines

    prompt = f"""Convert this MIPS assembly to C code.

{MIPS_CONTEXT}

Function: {func['name']} (address: 0x{func['addr']:08X if func['addr'] else 0:08X})

Assembly:
```
{asm_text}
```

Write equivalent C code. Use descriptive variable names. Include a brief comment describing what the function does.

```c
"""
    return prompt

def generate_naming_prompt(func):
    """Generate a function naming prompt."""
    # Just first 30 lines for quick analysis
    asm_preview = '\n'.join(func['lines'][:30])

    prompt = f"""Analyze this MIPS function and suggest a descriptive name.

Function: {func['name']}

First 30 lines:
```
{asm_preview}
```

Based on the instructions, what does this function likely do?
Reply with just: function_name - brief description (one line)
"""
    return prompt

def generate_arcade_compare_prompt(n64_func, arcade_code):
    """Generate a prompt to compare N64 asm with arcade C code."""
    asm_text = '\n'.join(n64_func['lines'][:60])

    prompt = f"""Compare this N64 assembly with the arcade C code and determine if they're the same function.

N64 Assembly ({n64_func['name']}):
```
{asm_text}
```

Arcade C code:
```c
{arcade_code[:2000]}
```

Are these the same function? Reply: YES/NO/MAYBE - brief explanation
"""
    return prompt

def batch_generate_prompts(asm_dir, output_dir, prompt_type='decompile'):
    """Generate prompts for all functions in assembly files."""
    asm_path = Path(asm_dir)
    out_path = Path(output_dir)
    out_path.mkdir(exist_ok=True)

    total_funcs = 0

    for asm_file in sorted(asm_path.glob('*.s')):
        print(f"Processing {asm_file.name}...")

        with open(asm_file, 'r') as f:
            content = f.read()

        functions = extract_functions_from_asm(content)

        for func in functions:
            if not func['name'].startswith('func_'):
                continue  # Skip already-named functions

            if prompt_type == 'decompile':
                prompt = generate_decompile_prompt(func)
            elif prompt_type == 'naming':
                prompt = generate_naming_prompt(func)
            else:
                continue

            # Save prompt to file
            prompt_file = out_path / f"{func['name']}_{prompt_type}.txt"
            with open(prompt_file, 'w') as f:
                f.write(prompt)

            total_funcs += 1

    print(f"\nGenerated {total_funcs} prompts in {output_dir}")

def create_ollama_batch_script(prompt_dir, output_script, model='qwen2.5-coder:7b'):
    """Create a shell script to run all prompts through Ollama."""
    prompt_path = Path(prompt_dir)

    script = f"""#!/bin/bash
# Batch process prompts through Ollama
# Model: {model}
# Generated by local_llm_prompts.py

MODEL="{model}"
PROMPT_DIR="{prompt_dir}"
OUTPUT_DIR="{prompt_dir}/results"

mkdir -p "$OUTPUT_DIR"

for prompt_file in "$PROMPT_DIR"/*.txt; do
    base=$(basename "$prompt_file" .txt)
    echo "Processing $base..."

    ollama run "$MODEL" < "$prompt_file" > "$OUTPUT_DIR/$base.out" 2>&1

    # Rate limit to avoid GPU memory issues
    sleep 1
done

echo "Done! Results in $OUTPUT_DIR"
"""

    with open(output_script, 'w') as f:
        f.write(script)
    os.chmod(output_script, 0o755)
    print(f"Created batch script: {output_script}")

def main():
    if len(sys.argv) < 2:
        print(f"""Usage: {sys.argv[0]} <command> [args]

Commands:
  extract <asm_dir> <output_dir>  - Generate decompile prompts for all functions
  naming <asm_dir> <output_dir>   - Generate naming prompts (faster, smaller)
  batch <prompt_dir> [model]      - Create Ollama batch script

Examples:
  {sys.argv[0]} extract asm/us prompts/decompile
  {sys.argv[0]} naming asm/us prompts/naming
  {sys.argv[0]} batch prompts/naming qwen2.5-coder:7b
""")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == 'extract':
        batch_generate_prompts(sys.argv[2], sys.argv[3], 'decompile')
    elif cmd == 'naming':
        batch_generate_prompts(sys.argv[2], sys.argv[3], 'naming')
    elif cmd == 'batch':
        model = sys.argv[3] if len(sys.argv) > 3 else 'qwen2.5-coder:7b'
        create_ollama_batch_script(sys.argv[2], f"{sys.argv[2]}/run_ollama.sh", model)
    else:
        print(f"Unknown command: {cmd}")
        sys.exit(1)

if __name__ == '__main__':
    main()
