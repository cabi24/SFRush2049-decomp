/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014EE8 - lib_15a10 (IDO 7.1). Resource lookup in sub-table 3: a resource block starts
 * with four u32 byte offsets to relative-record chains; this one searches chain 3 for ID via
 * func_80014E1C. Callers: func_800150xx/80014Fxx (locked, same TU) as
 * `extern void *func_80014EE8(u16, ResourceOffsets *)`.
 * w18c: the running byte pointer `p` (base, then += offset) gives retail's operand order
 * (addu a1,a1,t6); the one-expression form gives addu a1,t6,a1 (true=10).
 * TU context: the TU defines `typedef struct ResourceOffsets {...} ResourceOffsets` only later
 * (func_80014F14's block, after this slot), so a typedef here would be a redefinition. The
 * parameter uses the struct tag (incomplete here, completed by that later typedef; same type as
 * the callers' prototype) and reads offset 3 through an unsigned int view: type view only, no
 * code difference; the chain pointer is passed as func_80014E1C's own `struct RelativeEntry *`
 * (its definition precedes this slot) (LOCK form with the full typedef and resource->offsets[3] is identical). */
struct ResourceOffsets;
struct RelativeEntry;
extern void *func_80014E1C(unsigned short, struct RelativeEntry *);

void *func_80014EE8(unsigned short id, struct ResourceOffsets *resource)
{
    unsigned char *p = (unsigned char *)resource;
    p += ((unsigned int *)resource)[3];
    return func_80014E1C(id, (struct RelativeEntry *)p);
}
