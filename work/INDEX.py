#!/usr/bin/env python3
"""
Index of all function work directories and their status.
Run from the work/ directory to get a fresh listing.
"""

import os
from pathlib import Path

def main():
    work_dir = Path(__file__).parent

    # Find all directories with base.c
    entries = []
    for base_c in work_dir.rglob("base.c"):
        func_dir = base_c.parent
        rel_path = func_dir.relative_to(work_dir)

        # Read status
        status_file = func_dir / "STATUS"
        if status_file.exists():
            status = status_file.read_text().strip().split('\n')[0]  # First line only
        else:
            status = "UNKNOWN"

        entries.append((str(rel_path), status))

    # Print table
    if not entries:
        print("No function directories found.")
        return

    # Calculate column widths
    max_path = max(len(e[0]) for e in entries)
    max_status = max(len(e[1]) for e in entries)

    # Header
    print(f"{'Directory':<{max_path}}  {'Status':<{max_status}}")
    print(f"{'-' * max_path}  {'-' * max_status}")

    # Rows
    for path, status in entries:
        print(f"{path:<{max_path}}  {status:<{max_status}}")

    # Summary
    print(f"\nTotal: {len(entries)} functions")

    # Count by status
    from collections import Counter
    status_counts = Counter(s.split()[0] for _, s in entries)  # First word of status
    for status, count in sorted(status_counts.items()):
        print(f"  {status}: {count}")

if __name__ == "__main__":
    import signal
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)  # Handle broken pipe gracefully
    main()
