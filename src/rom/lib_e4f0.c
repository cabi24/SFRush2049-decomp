/* GENERATED ROM-aligned TU — segment 0xe4f0 (rom/lib_e4f0)
 * layout map 9fdb0972e66349f97b9135d40fbc75a0c69ad96b11e39f9421f3c0b3ce527617; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __ull_rshift (ultralib name since w23g; was __lshrdi3)
 * Source:   cloud/work/frontier/w16g/sources/__lshrdi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__lshrdi3.c:__lshrdi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned long long __ull_rshift(unsigned long long a0, unsigned long long a1) {
    return a0 >> a1;
}

/* PROMOTED 2026-10-11 — __ull_rem (ultralib name since w23g; was __umoddi3)
 * Source:   cloud/work/frontier/w16g/sources/__umoddi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__umoddi3.c:__umoddi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned long long __ull_rem(unsigned long long a0, unsigned long long a1) {
    return a0 % a1;
}

/* PROMOTED 2026-10-11 — __ull_div (ultralib name since w23g; was __udivdi3)
 * Source:   cloud/work/frontier/w16g/sources/__udivdi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__udivdi3.c:__udivdi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned long long __ull_div(unsigned long long a0, unsigned long long a1) {
    return a0 / a1;
}

/* PROMOTED 2026-10-11 — __ll_lshift (ultralib name since w23g; was __ashldi3)
 * Source:   cloud/work/frontier/w16g/sources/__ashldi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__ashldi3.c:__ashldi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned long long __ll_lshift(unsigned long long a0, unsigned long long a1) {
    return a0 << a1;
}

/* PROMOTED 2026-10-11 — __ll_rem (ultralib name since w23g; was __umoddi3_alt)
 * Source:   cloud/work/frontier/w16g/sources/__umoddi3_alt.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__umoddi3_alt.c:__umoddi3_alt (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
long long __ll_rem(unsigned long long a0, long long a1) {
    return a0 % a1;
}

/* PROMOTED 2026-10-11 — __ll_div (ultralib name since w23g; was __divdi3)
 * Source:   cloud/work/frontier/w16g/sources/__divdi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__divdi3.c:__divdi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
long long __ll_div(long long a0, long long a1) {
    return a0 / a1;
}

/* PROMOTED 2026-10-11 — __ll_mul (ultralib name since w23g; was __muldi3)
 * Source:   cloud/work/frontier/w16g/sources/__muldi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__muldi3.c:__muldi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned long long __ll_mul(unsigned long long a0, unsigned long long a1) {
    return a0 * a1;
}

/* PROMOTED 2026-10-11 — __ull_divremi (ultralib name since w23g; was __qdivrem)
 * Source:   cloud/work/frontier/w16g/sources/__qdivrem.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__qdivrem.c:__qdivrem (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __ull_divremi(unsigned long long* div, unsigned long long* rem, unsigned long long a2, unsigned short a3) {
    *div = a2 / a3;
    *rem = a2 % a3;
}

/* PROMOTED 2026-10-11 — __ll_mod (ultralib name since w23g; was __moddi3)
 * Source:   cloud/work/frontier/w16g/sources/__moddi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__moddi3.c:__moddi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
long long __ll_mod(long long a0, long long a1) {
    long long tmp = a0 % a1;

    if ((tmp < 0 && a1 > 0) || (tmp > 0 && a1 < 0)) {
        tmp += a1;
    }
    return tmp;
}

/* PROMOTED 2026-10-11 — __ll_rshift (ultralib name since w23g; was __ashrdi3)
 * Source:   cloud/work/frontier/w16g/sources/__ashrdi3.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips3 -32 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__ashrdi3.c:__ashrdi3 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_ll.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
long long __ll_rshift(long long a0, long long a1) {
    return a0 >> a1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e4f0/__osGetCause.s")
