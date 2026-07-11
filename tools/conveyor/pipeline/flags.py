"""Flag lattice (US3): bounded variations of the confirmed IDO baseline.

docs/COMPILER_SETTINGS.md establishes `-g0 {-O1|-O2} -mips2 -G 0 -non_shared`
from perfect matches; the lattice explores adjacent plausible settings, not an
open-ended flag space (spec assumption).
"""
import json

BASE = "-mips2 -G 0 -non_shared"

LATTICE = tuple(
    f"-g0 {opt} {BASE}" for opt in ("-O0", "-O1", "-O2", "-O3")
) + tuple(
    f"-g {opt} {BASE}" for opt in ("-O1", "-O2")
)

CONFIRMED = {
    "src/libc/string.c": f"-g0 -O2 {BASE}",
    "src/libultra/os_message.c": f"-g0 -O1 {BASE}",
    "src/libultra/gu.c": f"-g0 -O2 {BASE}",
}

# Evidence stamped on the pins seed_confirmed() writes.
_CONFIRMED_EVIDENCE = json.dumps(["docs/COMPILER_SETTINGS.md: perfect match"])


def seed_confirmed(conn):
    """Pre-load flag_registry with the flags proven in docs/COMPILER_SETTINGS.md
    so the sweeper never spends pool time re-discovering them (source='confirmed',
    which sweep.py refuses to clobber alongside manual overrides).

    Idempotent by design: INSERT OR IGNORE only fills TUs that have no pin yet,
    so calling this on every coordinator start never reverts a later sweep result
    or manual override. Returns the number of new pins written.
    """
    seeded = 0
    for tu, flagset in CONFIRMED.items():
        cur = conn.execute(
            "INSERT OR IGNORE INTO flag_registry"
            " (translation_unit, pinned_flagset, evidence, source)"
            " VALUES (?, ?, ?, 'confirmed')",
            (tu, flagset, _CONFIRMED_EVIDENCE),
        )
        seeded += cur.rowcount
    conn.commit()
    return seeded
