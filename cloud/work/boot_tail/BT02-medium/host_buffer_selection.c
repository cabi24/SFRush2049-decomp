/* Host-only tests for buffer selection; separate from pointer-slot tests. */
#include <assert.h>

unsigned char D_8003829E;
unsigned short D_800382E0[2];
unsigned short *D_800382D8[2];
unsigned short *D_800382D4, *D_800382D0;
unsigned int D_80038034, D_80038030;
void func_80013964(void);

int main(void)
{
    unsigned short buffers[2][12];
    unsigned int cursor;
    int i, j, k;
    D_800382D8[0] = buffers[0];
    D_800382D8[1] = buffers[1];
    for (i = 0; i < 2; ++i) {
        for (j = 0; j < 2; ++j) {
            D_800382E0[j] = 123;
            for (k = 0; k < 12; ++k) buffers[j][k] = 456;
        }
        D_8003829E = i;
        func_80013964();
        assert(D_800382D4 == &D_800382E0[i]);
        assert(D_800382D0 == buffers[i]);
        cursor = (unsigned int)buffers[i] + 16;
        assert(D_80038030 == cursor && D_80038034 == (cursor & ~15U));
        assert(D_800382E0[i] == 0 && D_800382E0[1-i] == 123);
        assert(buffers[i][0] == 0 && buffers[1-i][0] == 456);
        for (k = 1; k < 12; ++k) assert(buffers[i][k] == 456);
        assert(D_800382D8[0] == buffers[0] && D_800382D8[1] == buffers[1]);
    }
    return 0;
}
