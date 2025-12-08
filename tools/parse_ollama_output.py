#!/usr/bin/env python3
"""
Parse Ollama decompilation output and extract useful information.
Designed to be run by Codex or other local LLMs to save Claude tokens.

Usage:
  python parse_ollama_output.py extract-symbols   # Extract function names/purposes
  python parse_ollama_output.py clean-c           # Clean up C code output
  python parse_ollama_output.py summary           # Generate summary report
"""

import re
import sys
from pathlib import Path

OLLAMA_DIR = Path(__file__).parent.parent / "ollama_analysis"

def extract_symbols_from_classification():
    """Extract function addresses and purposes from classification file."""
    classify_file = OLLAMA_DIR / "all_functions_classified.txt"
    if not classify_file.exists():
        print("Classification file not found")
        return

    content = classify_file.read_text()

    # Pattern: func_XXXXXXXX or 800XXXXX followed by description
    patterns = [
        r'func_([0-9A-Fa-f]{8}).*?:\s*(.+?)(?:\n|$)',
        r'\*\*func_([0-9A-Fa-f]{8})\*\*:\s*(.+?)(?:\n|$)',
        r'---\s*([0-9A-Fa-f]{8})\s*---\s*(.+?)(?:\n|$)',
        r'([0-9A-Fa-f]{8}):\s*(.+?)(?:\n|$)',
    ]

    symbols = {}
    for pattern in patterns:
        for match in re.finditer(pattern, content, re.IGNORECASE):
            addr = match.group(1).upper()
            desc = match.group(2).strip()
            # Clean up description
            desc = re.sub(r'[*\[\]`]', '', desc)
            desc = desc.split('.')[0]  # First sentence only
            desc = desc[:60]  # Limit length
            if addr not in symbols and len(desc) > 3:
                symbols[addr] = desc

    # Output as symbol_addrs format
    print("// Extracted from Ollama classification")
    print("// Review and merge into symbol_addrs.us.txt")
    print()
    for addr, desc in sorted(symbols.items()):
        # Convert to valid C identifier
        name = desc.lower()
        name = re.sub(r'[^a-z0-9_]', '_', name)
        name = re.sub(r'_+', '_', name)
        name = name.strip('_')[:30]
        if name:
            print(f"func_{addr} = 0x{addr}; // {desc}")

def extract_c_code_from_decompiled():
    """Extract C code blocks from decompiled output files."""
    for filename in ["priority_decompiled.txt", "batch2_decompiled.txt"]:
        filepath = OLLAMA_DIR / filename
        if not filepath.exists():
            continue

        content = filepath.read_text()

        # Find function headers and C code blocks
        func_pattern = r'//\s*===\s*([0-9A-Fa-f]+):\s*(.+?)\s*==='
        code_pattern = r'```c?\n(.+?)```'

        print(f"\n// === From {filename} ===\n")

        current_func = None
        for line in content.split('\n'):
            func_match = re.match(r'//\s*===\s*([0-9A-Fa-f]+):\s*(.+?)\s*===', line)
            if func_match:
                current_func = (func_match.group(1), func_match.group(2))
                print(f"\n// func_{current_func[0]}: {current_func[1]}")

        # Extract code blocks
        for match in re.finditer(code_pattern, content, re.DOTALL):
            code = match.group(1).strip()
            # Basic cleanup
            code = re.sub(r'^\s*```\w*\s*', '', code)
            code = re.sub(r'\s*```\s*$', '', code)
            if 'void' in code or 'int' in code or 'u32' in code:
                print(code[:500])  # First 500 chars of each

def generate_summary():
    """Generate a summary of Ollama analysis results."""
    print("=== Ollama Analysis Summary ===\n")

    # Count functions in classification
    classify_file = OLLAMA_DIR / "all_functions_classified.txt"
    if classify_file.exists():
        content = classify_file.read_text()
        func_count = len(re.findall(r'func_[0-9A-Fa-f]{8}', content))
        batch_count = len(re.findall(r'BATCH \d+', content))
        print(f"Classification: {func_count} functions in {batch_count} batches")

    # Count decompiled functions
    for filename in ["priority_decompiled.txt", "batch2_decompiled.txt"]:
        filepath = OLLAMA_DIR / filename
        if filepath.exists():
            content = filepath.read_text()
            func_count = len(re.findall(r'//\s*===\s*[0-9A-Fa-f]+:', content))
            size = filepath.stat().st_size
            print(f"{filename}: {func_count} functions, {size/1024:.1f}KB")

    # Categories found
    print("\nCategories identified:")
    categories = {
        'graphics': r'graphics|render|display|draw|sprite',
        'audio': r'audio|sound|music|sfx',
        'physics': r'physics|collision|velocity|gravity',
        'ai': r'ai|drone|pathfind|behavior',
        'input': r'input|controller|button|joystick',
        'state': r'state|game.*loop|attract|menu',
    }

    if classify_file.exists():
        content = classify_file.read_text().lower()
        for cat, pattern in categories.items():
            count = len(re.findall(pattern, content))
            if count > 0:
                print(f"  {cat}: ~{count} mentions")

def main():
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python parse_ollama_output.py extract-symbols")
        print("  python parse_ollama_output.py clean-c")
        print("  python parse_ollama_output.py summary")
        sys.exit(1)

    cmd = sys.argv[1]
    if cmd == "extract-symbols":
        extract_symbols_from_classification()
    elif cmd == "clean-c":
        extract_c_code_from_decompiled()
    elif cmd == "summary":
        generate_summary()
    else:
        print(f"Unknown command: {cmd}")
        sys.exit(1)

if __name__ == "__main__":
    main()
