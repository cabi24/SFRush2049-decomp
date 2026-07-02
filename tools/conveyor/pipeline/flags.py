"""Flag lattice (US3): bounded variations of the confirmed IDO baseline.

docs/COMPILER_SETTINGS.md establishes `-g0 {-O1|-O2} -mips2 -G 0 -non_shared`
from perfect matches; the lattice explores adjacent plausible settings, not an
open-ended flag space (spec assumption).
"""

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
