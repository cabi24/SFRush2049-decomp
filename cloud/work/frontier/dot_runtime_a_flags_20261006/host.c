/* Storage and entry wrapper only; the matching function is compiled unchanged. */
#include <string.h>

unsigned char D_803BA7E0[16];
unsigned char D_803BA7F0[16];
int D_803BA830[4];
signed char D_803B65A4;
extern void func_80393004(void);

void host_run(int index, const int *counts, const unsigned char *initial,
              unsigned char *result)
{
    memcpy(D_803BA830, counts, sizeof(D_803BA830));
    memcpy(D_803BA7E0, initial, 16);
    memcpy(D_803BA7F0, initial + 16, 16);
    D_803B65A4 = (signed char)index;
    func_80393004();
    memcpy(result, D_803BA7E0, 16);
    memcpy(result + 16, D_803BA7F0, 16);
    memcpy(result + 32, D_803BA830, sizeof(D_803BA830));
    result[48] = (unsigned char)D_803B65A4;
}
