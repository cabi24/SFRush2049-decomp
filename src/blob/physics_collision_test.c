/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Effect/collision pool reset (historical label physics_collision_test;
 * N64 code, no arcade ancestor found). Sets up the pool D_8013F1E0
 * (100 records of 88 bytes in D_8013C378, flag cleared) and builds its free
 * list (pool_linked_list_init), zeroes the six words D_801392D8[], and, when
 * D_80156994 is set or D_8014978C >= 6, calls func_800B24EC for each of six
 * entries (D_80117480[i], &D_8013F380[i], 0, (s8)(D_80140BDC - 1), 1).
 *
 * Real source form: the pool set-up is a call of the kept function
 * struct_fields_init(&D_8013F1E0, D_8013C378, 88, 100, 0) (= pool_init:
 * mem, size, count, flag, then pool_linked_list_init), which umerge inlines
 * in the whole-program unit; its statements then carry the call's .loc, so
 * as1 orders the four stores as one source line (size/count/mem/flag).
 * blob_unit score is EQUAL with that call
 * (cloud/work/frontier/w8c/physics_collision_test/unit_call.c). Standalone,
 * the one-line POOL_SETUP macro below reproduces the same single .loc; on
 * four separate lines (or a static helper, which keeps its own lines) the
 * store order is mem/count/size (4 words off).
 * D_80140BDC is u8 (lbu), re-read per iteration. Also matches at -O2.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;

typedef struct Pool {
    u8 flag;
    int count;
    int size;
    void *mem;
} Pool;

extern Pool D_8013F1E0;
extern char D_8013C378[];
extern int D_801392D8[6];
extern s8 D_80156994;
extern s8 D_8014978C;
extern int D_80117480[6];
extern s16 D_8013F380[6];
extern u8 D_80140BDC;
void pool_linked_list_init(Pool *pool);
void func_800B24EC(int a, s16 *b, int c, int d, int e);

/* one-line multi-statement macro: stands in for the inlined kept struct_fields_init (one .loc) */
#define POOL_SETUP(pool, m, n, sz) { (pool).mem = (m); (pool).count = (n); (pool).size = (sz); (pool).flag = 0; }

void physics_collision_test(void)
{
    int i;

    POOL_SETUP(D_8013F1E0, D_8013C378, 100, 88);
    pool_linked_list_init(&D_8013F1E0);
    for (i = 0; i < 6; i++) {
        D_801392D8[i] = 0;
    }
    if (D_80156994 || D_8014978C >= 6) {
        for (i = 0; i < 6; i++) {
            func_800B24EC(D_80117480[i], &D_8013F380[i], 0, (s8)(D_80140BDC - 1), 1);
        }
    }
}
