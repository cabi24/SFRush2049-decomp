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

void physics_collision_test(void)
{
    int i;

    D_8013F1E0.mem = D_8013C378;
    D_8013F1E0.count = 100;
    D_8013F1E0.size = 88;
    D_8013F1E0.flag = 0;
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
