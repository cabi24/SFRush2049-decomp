/* GENERATED ROM-aligned TU — segment 0xa5f0 (rom/lib_a5f0)
 * layout map 781cab99bcf2ac23f7306a8a1c5086e888aad7a08163bcdb0fce3260f94e2a1c; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __d_to_ll (ultralib name since w23g; was __fixdfdi)
 * Source:   cloud/work/frontier/w16g/sources/__fixdfdi.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__fixdfdi.c:__fixdfdi (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
long long __d_to_ll(double d) {
    return d;
}

/* PROMOTED 2026-10-11 — __f_to_ll (ultralib name since w23g; was __fixsfdi)
 * Source:   cloud/work/frontier/w16g/sources/__fixsfdi.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__fixsfdi.c:__fixsfdi (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
long long __f_to_ll(float f) {
    return f;
}

/* PROMOTED 2026-10-11 — __d_to_ull (ultralib name since w23g; was __fixunsdfdi)
 * Source:   cloud/work/frontier/w16g/sources/__fixunsdfdi.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__fixunsdfdi.c:__fixunsdfdi (score0); rom_tu.h context: cloud/work/frontier/w19c/ctx/ctx_w19c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned long long __d_to_ull(double d) {
    return d;
}

/* PROMOTED 2026-10-11 — __f_to_ull (ultralib name since w23g; was __fixunssfdi)
 * Source:   cloud/work/frontier/w16g/sources/__fixunssfdi.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__fixunssfdi.c:__fixunssfdi (score0); rom_tu.h context: cloud/work/frontier/w19c/ctx/ctx_w19c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned long long __f_to_ull(float f) {
    return f;
}

/* PROMOTED 2026-10-11 — __ll_to_d (ultralib name since w23g; was __floatdidf)
 * Source:   cloud/work/frontier/w16g/sources/__floatdidf.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__floatdidf.c:__floatdidf (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
double __ll_to_d(long long s) {
    return s;
}

/* PROMOTED 2026-10-11 — __ll_to_f (ultralib name since w23g; was __floatdisf)
 * Source:   cloud/work/frontier/w16g/sources/__floatdisf.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__floatdisf.c:__floatdisf (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
float __ll_to_f(long long s) {
    return s;
}

/* PROMOTED 2026-10-11 — __ull_to_d (ultralib name since w23g; was __floatundidf)
 * Source:   cloud/work/frontier/w16g/sources/__floatundidf.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__floatundidf.c:__floatundidf (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
double __ull_to_d(unsigned long long u) {
    return u;
}

/* PROMOTED 2026-10-11 — __ull_to_f (ultralib name since w23g; was __floatundisf)
 * Source:   cloud/work/frontier/w16g/sources/__floatundisf.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__floatundisf.c:__floatundisf (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
float __ull_to_f(unsigned long long u) {
    return u;
}

