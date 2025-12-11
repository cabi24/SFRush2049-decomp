#!/usr/bin/env python3
"""
progress.py - Track decompilation progress for Rush 2049

Usage:
    ./tools/progress.py              # Show summary
    ./tools/progress.py --all        # Show all functions with status
    ./tools/progress.py --category   # Group by category
    ./tools/progress.py --csv        # Export as CSV
    ./tools/progress.py --html       # Export as HTML report
"""

import os
import re
import sys
import argparse
from pathlib import Path
from collections import defaultdict

# Project root
PROJECT_ROOT = Path(__file__).parent.parent

# Status levels
STATUS_NOT_STARTED = 0      # Assembly only, no C code
STATUS_STUB = 1             # C stub exists (not implemented)
STATUS_DECOMPILED = 2       # Decompiled but not matching
STATUS_MATCHING = 3         # Verified matching (builds correctly)

STATUS_NAMES = {
    STATUS_NOT_STARTED: "Not Started",
    STATUS_STUB: "Stub",
    STATUS_DECOMPILED: "Decompiled",
    STATUS_MATCHING: "Matching",
}

STATUS_COLORS = {
    STATUS_NOT_STARTED: "\033[90m",  # Gray
    STATUS_STUB: "\033[33m",         # Yellow
    STATUS_DECOMPILED: "\033[36m",   # Cyan
    STATUS_MATCHING: "\033[32m",     # Green
}
RESET = "\033[0m"

def load_symbols():
    """Load all function symbols from symbol_addrs.us.txt"""
    symbols = {}
    symbol_file = PROJECT_ROOT / "symbol_addrs.us.txt"

    if not symbol_file.exists():
        return symbols

    with open(symbol_file) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("//"):
                continue

            # Parse: name = 0xADDRESS; // type:func comment
            match = re.match(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;?\s*(?://\s*(.*))?$', line)
            if match:
                name = match.group(1)
                addr = int(match.group(2), 16)
                comment = match.group(3) or ""

                # Check if it's a function
                if "type:func" in comment or not comment:
                    symbols[addr] = {
                        "name": name,
                        "address": addr,
                        "comment": comment,
                        "status": STATUS_NOT_STARTED,
                        "category": categorize_function(name, addr, comment),
                        "source_file": None,
                    }

    return symbols

def load_game_code_functions():
    """Load game code functions from docs/game_functions.txt"""
    functions = {}
    game_funcs = PROJECT_ROOT / "docs" / "game_functions.txt"

    if not game_funcs.exists():
        return functions

    with open(game_funcs) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            # Parse: func_XXXXXXXX = 0xXXXXXXXX;
            match = re.match(r'^(func_[0-9A-Fa-f]+)\s*=\s*0x([0-9A-Fa-f]+)\s*;?', line)
            if match:
                name = match.group(1)
                addr = int(match.group(2), 16)
                functions[addr] = {
                    "name": name,
                    "address": addr,
                    "comment": "game code",
                    "status": STATUS_NOT_STARTED,
                    "category": "game_code",
                    "source_file": None,
                }

    return functions

def categorize_function(name, addr, comment):
    """Categorize a function based on its name/address/comment"""
    name_lower = name.lower()

    # libc
    if name in ["memchr", "memset", "strchr", "strlen", "memcpy", "bcopy", "bzero"]:
        return "libc"

    # libm
    if name in ["modf", "modff", "sinf", "cosf", "sqrtf", "__isinf", "__isnan", "fcvt", "__ecvt_internal", "__round_helper"]:
        return "libm"

    # libgcc
    if name.startswith("__fix") or name.startswith("__float") or name.startswith("__") and "di" in name:
        return "libgcc"

    # libultra OS
    if name.startswith("os") or name.startswith("__os"):
        return "libultra_os"

    # libultra GU
    if name.startswith("gu"):
        return "libultra_gu"

    # PFS (controller pak)
    if "Pfs" in name or "pfs" in name:
        return "libultra_pfs"

    # Motor/rumble
    if "Motor" in name or "motor" in name:
        return "libultra_motor"

    # Inflate/decompression
    if "inflate" in name_lower or "huft" in name_lower or "lzss" in name_lower:
        return "inflate"

    # DLL/timer queue
    if name.startswith("dll_"):
        return "timer_queue"

    # Display/rendering
    if "display" in name_lower or "viewport" in name_lower:
        return "display"

    # Scheduler
    if "__sc" in name or "Scheduler" in name:
        return "scheduler"

    # DMA
    if "dma" in name_lower:
        return "dma"

    # Checksum
    if "checksum" in name_lower:
        return "util"

    # Game init
    if name in ["main", "game_init", "entrypoint"]:
        return "startup"

    # Game code (dynamic)
    if 0x80086A50 <= addr <= 0x80124AF0:
        return "game_code"

    return "unknown"

def scan_source_files():
    """Scan src/*.c files to find implemented functions"""
    implemented = {}
    src_dir = PROJECT_ROOT / "src"

    if not src_dir.exists():
        return implemented

    for c_file in src_dir.rglob("*.c"):
        with open(c_file) as f:
            content = f.read()

        # Find function definitions (not declarations)
        # Look for: returntype funcname(args) { ... }
        # Also handle: void funcname(void *arg) {
        func_pattern = re.compile(
            r'^(?:static\s+)?(?:void|int|s32|u32|s16|u16|s8|u8|char|float|double|f32|f64|OSMesgQueue|OSMesg|void)\s*\*?\s*'
            r'(\w+)\s*\([^)]*\)\s*\{',
            re.MULTILINE
        )

        for match in func_pattern.finditer(content):
            func_name = match.group(1)
            rel_path = c_file.relative_to(PROJECT_ROOT)

            # Check if it's a stub (contains only return or nothing)
            # Find the function body
            start = match.end()
            brace_count = 1
            end = start
            while end < len(content) and brace_count > 0:
                if content[end] == '{':
                    brace_count += 1
                elif content[end] == '}':
                    brace_count -= 1
                end += 1

            body = content[start:end-1].strip()

            # Determine status
            if not body or body == "return;" or body == "return 0;":
                status = STATUS_STUB
            else:
                # Has real code - check if it's marked as matching
                # Look for MATCHING or NON_MATCHING comments
                context = content[max(0, match.start()-200):match.start()]
                if "NON_MATCHING" in context or "non-matching" in context.lower():
                    status = STATUS_DECOMPILED
                elif "MATCHING" in context or "matching" in context.lower():
                    status = STATUS_MATCHING
                else:
                    # Default: decompiled but not verified
                    status = STATUS_DECOMPILED

            implemented[func_name] = {
                "source_file": str(rel_path),
                "status": status,
            }

    return implemented

def merge_data(symbols, game_funcs, implemented):
    """Merge all data sources into a single function list"""
    all_functions = {}

    # Start with symbols
    for addr, func in symbols.items():
        all_functions[addr] = func.copy()

    # Add game code functions (don't overwrite named symbols)
    for addr, func in game_funcs.items():
        if addr not in all_functions:
            all_functions[addr] = func.copy()

    # Update with implementation status
    for addr, func in all_functions.items():
        name = func["name"]
        if name in implemented:
            func["status"] = implemented[name]["status"]
            func["source_file"] = implemented[name]["source_file"]

    return all_functions

def print_summary(functions):
    """Print a summary of decompilation progress"""
    total = len(functions)
    by_status = defaultdict(int)
    by_category = defaultdict(lambda: defaultdict(int))

    for func in functions.values():
        by_status[func["status"]] += 1
        by_category[func["category"]][func["status"]] += 1

    # Overall progress
    matching = by_status[STATUS_MATCHING]
    decompiled = by_status[STATUS_DECOMPILED] + matching
    started = decompiled + by_status[STATUS_STUB]

    print("=" * 60)
    print("RUSH 2049 DECOMPILATION PROGRESS")
    print("=" * 60)
    print()

    # Progress bar
    pct_matching = (matching / total * 100) if total > 0 else 0
    pct_decompiled = (decompiled / total * 100) if total > 0 else 0

    bar_width = 40
    filled_matching = int(bar_width * matching / total) if total > 0 else 0
    filled_decompiled = int(bar_width * decompiled / total) if total > 0 else 0
    filled_decompiled = max(filled_decompiled - filled_matching, 0)

    bar = (
        f"{STATUS_COLORS[STATUS_MATCHING]}{'█' * filled_matching}{RESET}"
        f"{STATUS_COLORS[STATUS_DECOMPILED]}{'█' * filled_decompiled}{RESET}"
        f"{'░' * (bar_width - filled_matching - filled_decompiled)}"
    )

    print(f"[{bar}] {pct_decompiled:.1f}%")
    print()

    print(f"{'Status':<15} {'Count':>8} {'Percent':>10}")
    print("-" * 35)
    for status in [STATUS_MATCHING, STATUS_DECOMPILED, STATUS_STUB, STATUS_NOT_STARTED]:
        count = by_status[status]
        pct = (count / total * 100) if total > 0 else 0
        color = STATUS_COLORS[status]
        print(f"{color}{STATUS_NAMES[status]:<15}{RESET} {count:>8} {pct:>9.1f}%")
    print("-" * 35)
    print(f"{'Total':<15} {total:>8} {'100.0':>9}%")
    print()

    # By category
    print("\nProgress by Category:")
    print("-" * 60)
    print(f"{'Category':<20} {'Total':>6} {'Match':>6} {'Decomp':>6} {'%':>8}")
    print("-" * 60)

    for category in sorted(by_category.keys()):
        cat_stats = by_category[category]
        cat_total = sum(cat_stats.values())
        cat_matching = cat_stats[STATUS_MATCHING]
        cat_decompiled = cat_stats[STATUS_DECOMPILED] + cat_matching
        cat_pct = (cat_decompiled / cat_total * 100) if cat_total > 0 else 0

        print(f"{category:<20} {cat_total:>6} {cat_matching:>6} {cat_decompiled:>6} {cat_pct:>7.1f}%")

    print()

def print_all_functions(functions, show_status=None):
    """Print all functions with their status"""
    print(f"{'Address':<12} {'Name':<30} {'Status':<12} {'File':<30}")
    print("-" * 90)

    for addr in sorted(functions.keys()):
        func = functions[addr]
        if show_status is not None and func["status"] != show_status:
            continue

        color = STATUS_COLORS[func["status"]]
        status_name = STATUS_NAMES[func["status"]]
        source = func["source_file"] or "-"

        print(f"0x{addr:08X}   {color}{func['name']:<30}{RESET} {status_name:<12} {source:<30}")

def print_by_category(functions):
    """Print functions grouped by category"""
    by_category = defaultdict(list)

    for func in functions.values():
        by_category[func["category"]].append(func)

    for category in sorted(by_category.keys()):
        funcs = by_category[category]
        matching = sum(1 for f in funcs if f["status"] == STATUS_MATCHING)
        decompiled = sum(1 for f in funcs if f["status"] >= STATUS_DECOMPILED)

        print(f"\n{'=' * 60}")
        print(f"{category.upper()} ({decompiled}/{len(funcs)} decompiled, {matching} matching)")
        print("=" * 60)

        for func in sorted(funcs, key=lambda f: f["address"]):
            color = STATUS_COLORS[func["status"]]
            status_name = STATUS_NAMES[func["status"]][0]  # First letter
            source = func["source_file"] or ""
            print(f"  {color}[{status_name}]{RESET} 0x{func['address']:08X} {func['name']:<30} {source}")

def export_csv(functions, output_file):
    """Export functions to CSV"""
    with open(output_file, 'w') as f:
        f.write("Address,Name,Status,Category,Source File\n")
        for addr in sorted(functions.keys()):
            func = functions[addr]
            f.write(f"0x{addr:08X},{func['name']},{STATUS_NAMES[func['status']]},{func['category']},{func['source_file'] or ''}\n")
    print(f"Exported to {output_file}")

def export_html(functions, output_file):
    """Export functions to HTML report"""
    by_category = defaultdict(list)
    for func in functions.values():
        by_category[func["category"]].append(func)

    total = len(functions)
    matching = sum(1 for f in functions.values() if f["status"] == STATUS_MATCHING)
    decompiled = sum(1 for f in functions.values() if f["status"] >= STATUS_DECOMPILED)

    html = f"""<!DOCTYPE html>
<html>
<head>
    <title>Rush 2049 Decompilation Progress</title>
    <style>
        body {{ font-family: monospace; margin: 20px; background: #1a1a1a; color: #e0e0e0; }}
        h1, h2 {{ color: #00ff00; }}
        .progress-bar {{ width: 100%; height: 30px; background: #333; border-radius: 5px; overflow: hidden; }}
        .progress-fill {{ height: 100%; background: linear-gradient(90deg, #00ff00, #00cc00); }}
        .stats {{ margin: 20px 0; }}
        table {{ border-collapse: collapse; width: 100%; }}
        th, td {{ border: 1px solid #444; padding: 8px; text-align: left; }}
        th {{ background: #333; }}
        .matching {{ color: #00ff00; }}
        .decompiled {{ color: #00cccc; }}
        .stub {{ color: #cccc00; }}
        .not-started {{ color: #666; }}
        .category {{ margin-top: 30px; }}
    </style>
</head>
<body>
    <h1>Rush 2049 N64 Decompilation Progress</h1>

    <div class="stats">
        <h2>Overall Progress: {decompiled/total*100:.1f}%</h2>
        <div class="progress-bar">
            <div class="progress-fill" style="width: {decompiled/total*100:.1f}%"></div>
        </div>
        <p>Total: {total} | Matching: {matching} | Decompiled: {decompiled} | Not Started: {total - decompiled}</p>
    </div>

    <h2>By Category</h2>
    <table>
        <tr><th>Category</th><th>Total</th><th>Matching</th><th>Decompiled</th><th>Progress</th></tr>
"""

    for category in sorted(by_category.keys()):
        funcs = by_category[category]
        cat_total = len(funcs)
        cat_matching = sum(1 for f in funcs if f["status"] == STATUS_MATCHING)
        cat_decompiled = sum(1 for f in funcs if f["status"] >= STATUS_DECOMPILED)
        cat_pct = (cat_decompiled / cat_total * 100) if cat_total > 0 else 0

        html += f"        <tr><td>{category}</td><td>{cat_total}</td><td>{cat_matching}</td><td>{cat_decompiled}</td><td>{cat_pct:.1f}%</td></tr>\n"

    html += """    </table>

    <h2>All Functions</h2>
"""

    for category in sorted(by_category.keys()):
        funcs = sorted(by_category[category], key=lambda f: f["address"])
        html += f"""
    <div class="category">
        <h3>{category}</h3>
        <table>
            <tr><th>Address</th><th>Name</th><th>Status</th><th>Source</th></tr>
"""
        for func in funcs:
            status_class = STATUS_NAMES[func["status"]].lower().replace(" ", "-")
            html += f"""            <tr class="{status_class}">
                <td>0x{func['address']:08X}</td>
                <td>{func['name']}</td>
                <td>{STATUS_NAMES[func['status']]}</td>
                <td>{func['source_file'] or '-'}</td>
            </tr>
"""
        html += "        </table>\n    </div>\n"

    html += """
</body>
</html>
"""

    with open(output_file, 'w') as f:
        f.write(html)
    print(f"Exported to {output_file}")

def main():
    parser = argparse.ArgumentParser(description="Track decompilation progress")
    parser.add_argument("--all", action="store_true", help="Show all functions")
    parser.add_argument("--category", action="store_true", help="Group by category")
    parser.add_argument("--csv", type=str, help="Export to CSV file")
    parser.add_argument("--html", type=str, help="Export to HTML file")
    parser.add_argument("--status", type=str, choices=["matching", "decompiled", "stub", "not-started"],
                       help="Filter by status")
    args = parser.parse_args()

    # Load data
    print("Loading symbols...", file=sys.stderr)
    symbols = load_symbols()

    print("Loading game code functions...", file=sys.stderr)
    game_funcs = load_game_code_functions()

    print("Scanning source files...", file=sys.stderr)
    implemented = scan_source_files()

    print("Merging data...", file=sys.stderr)
    functions = merge_data(symbols, game_funcs, implemented)

    print(f"Found {len(functions)} total functions\n", file=sys.stderr)

    # Handle status filter
    status_filter = None
    if args.status:
        status_map = {
            "matching": STATUS_MATCHING,
            "decompiled": STATUS_DECOMPILED,
            "stub": STATUS_STUB,
            "not-started": STATUS_NOT_STARTED,
        }
        status_filter = status_map[args.status]

    # Output
    if args.csv:
        export_csv(functions, args.csv)
    elif args.html:
        export_html(functions, args.html)
    elif args.category:
        print_by_category(functions)
    elif args.all:
        print_all_functions(functions, status_filter)
    else:
        print_summary(functions)

if __name__ == "__main__":
    main()
