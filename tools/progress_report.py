#!/usr/bin/env python3
"""
Generate progress report for function matching.
"""

import os
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).parent.parent
WORK_DIR = PROJECT_ROOT / "work"

def main():
    if not WORK_DIR.exists():
        print("Error: work/ directory not found")
        print("Run: python3 tools/setup_work_dirs.py")
        return

    # Count by status and category
    stats = defaultdict(lambda: defaultdict(int))
    total = defaultdict(int)

    for status_file in WORK_DIR.rglob("STATUS"):
        try:
            status = status_file.read_text().strip().split()[0]  # First word
        except:
            status = "UNKNOWN"

        # Get category (first two path components after work/)
        parts = status_file.relative_to(WORK_DIR).parts
        if len(parts) >= 2:
            category = parts[0]
            if parts[0] in ['libultra', 'game'] and len(parts) >= 3:
                category = f"{parts[0]}/{parts[1]}"
        else:
            category = "unknown"

        stats[category][status] += 1
        total[status] += 1

    # Print report
    print("=" * 60)
    print("FUNCTION MATCHING PROGRESS REPORT")
    print("=" * 60)
    print()

    # Summary
    all_count = sum(total.values())
    matching = total.get('MATCHING', 0)
    wip = total.get('WIP', 0)
    todo = total.get('TODO', 0)

    print(f"Total Functions: {all_count}")
    print(f"  MATCHING:  {matching:4} ({100*matching/all_count:.1f}%)")
    print(f"  WIP:       {wip:4}")
    print(f"  TODO:      {todo:4}")
    print()

    # Progress bar
    pct = matching / all_count if all_count > 0 else 0
    bar_len = 40
    filled = int(bar_len * pct)
    bar = "█" * filled + "░" * (bar_len - filled)
    print(f"Progress: [{bar}] {100*pct:.1f}%")
    print()

    # By category
    print("-" * 60)
    print(f"{'Category':<25} {'MATCH':>6} {'WIP':>6} {'TODO':>6} {'Total':>6}")
    print("-" * 60)

    for category in sorted(stats.keys()):
        cat_stats = stats[category]
        cat_total = sum(cat_stats.values())
        m = cat_stats.get('MATCHING', 0)
        w = cat_stats.get('WIP', 0)
        t = cat_stats.get('TODO', 0)
        print(f"{category:<25} {m:>6} {w:>6} {t:>6} {cat_total:>6}")

    print("-" * 60)
    print()

    # Functions that are MATCHING
    if matching > 0:
        print("MATCHED FUNCTIONS:")
        for status_file in WORK_DIR.rglob("STATUS"):
            try:
                if status_file.read_text().strip() == "MATCHING":
                    func_name = status_file.parent.name
                    print(f"  ✓ {func_name}")
            except:
                pass
        print()

    # Functions that are WIP
    if wip > 0:
        print("WORK IN PROGRESS:")
        for status_file in WORK_DIR.rglob("STATUS"):
            try:
                content = status_file.read_text().strip()
                if content.startswith("WIP"):
                    func_name = status_file.parent.name
                    print(f"  → {func_name}: {content}")
            except:
                pass
        print()

if __name__ == "__main__":
    main()
