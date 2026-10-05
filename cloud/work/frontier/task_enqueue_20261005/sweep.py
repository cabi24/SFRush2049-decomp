#!/usr/bin/env python3
"""Bounded 48-case diagnostic sweep; writes counts only, never match claims."""
from dataclasses import asdict
from itertools import permutations
import json
from pathlib import Path
import tempfile

from verify import HERE, FUNCTION, function_extent, score


def sweep():
    source = (HERE / "candidate.c").read_text()
    stores = [
        "    D_80155238.next = 0;",
        "    D_80155238.msgQueue = &D_80152750;",
        "    D_80155238.msg = 0;",
        "    D_80155238.flags = 2;",
    ]
    block = "\n".join(stores)
    assert source.count(block) == 1
    results = []
    with tempfile.TemporaryDirectory(prefix="task-enqueue-sweep-") as tmp:
        path = Path(tmp)
        for volatile in (False, True):
            for order in permutations(range(4)):
                variant = source.replace(block, "\n".join(stores[i] for i in order))
                if volatile:
                    variant = variant.replace("extern OSScTask D_80155238;",
                                              "extern volatile OSScTask D_80155238;")
                candidate, obj = path / "probe.c", path / "probe.o"
                candidate.write_text(variant)
                score.compile_single(candidate, "-g0 -O3 -mips2 -G 0 -non_shared", obj)
                comparison = score.compare(obj, FUNCTION, show=0)
                results.append({"store_order": list(order), "volatile_control": volatile,
                                "elf_function_bytes": function_extent(obj, FUNCTION)[1],
                                "comparison": asdict(comparison)})
    return {"variant_count": len(results), "claims": [], "results": results}


if __name__ == "__main__":
    print(json.dumps(sweep(), indent=2, sort_keys=True))
