/* Unchanged candidate/accepted consumer are separate translation units. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef float f32;
typedef struct Node {
    struct Node *next;
    s16 field4, field6, field8;
    u8 padA[2];
    void *fieldC;
    f32 field10;
    u32 field14;
} Node;
extern Node D_80138880[100];
Node *D_801392C8;
Node *D_801391F0;
s16 D_8012E66C, D_8012E678;
extern void func_800B2BDC(void);
extern Node *func_80090284(void);

static u32 read32(const u8 *p)
{
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}
static unsigned read16(const u8 *p) { return ((unsigned)p[0] << 8) | p[1]; }
static void emit(u32 v, unsigned n)
{
    unsigned i;
    for (i = n; i; i--) putchar((int)((v >> (8 * (i - 1))) & 255));
}
static u32 native_pointer(Node *p)
{
    if (!p) return 0;
    if (p < D_80138880 || p >= D_80138880 + 100) abort();
    return 0x80138880U + (u32)(p - D_80138880) * 24;
}
int main(void)
{
    u8 input[2404];
    Node expected[100];
    unsigned i, count, pops;
    s16 high;
    u32 bits;
    while (fread(input, 1, sizeof(input), stdin) == sizeof(input)) {
        pops = read16(input + 2400);
        high = (s16)read16(input + 2402);
        if (pops > 101) abort();
        memset(D_80138880, 0xa5, sizeof(D_80138880));
        for (i = 0; i < 100; i++) {
            Node *p = D_80138880 + i;
            u8 *b = input + 24 * i;
            p->next = D_80138880 + (i + 7) % 100;
            p->field4 = (s16)read16(b + 4);
            p->field6 = (s16)read16(b + 6);
            p->field8 = (s16)read16(b + 8);
            p->padA[0] = b[10]; p->padA[1] = b[11];
            p->fieldC = 0;
            bits = read32(b + 16); memcpy(&p->field10, &bits, 4);
            p->field14 = read32(b + 20);
        }
        memcpy(expected, D_80138880, sizeof(expected));
        for (i = 0; i < 100; i++) {
            expected[i].next = i < 99 ? D_80138880 + i + 1 : 0;
            expected[i].field6 = -1;
        }
        D_801392C8 = D_80138880 + 42;
        D_801391F0 = D_80138880 + 73;
        D_8012E66C = -123;
        D_8012E678 = high;
        func_800B2BDC();
        func_800B2BDC();
        if (memcmp(expected, D_80138880, sizeof(expected)) ||
            D_801392C8 != D_80138880 || D_801391F0 || D_8012E66C || D_8012E678 != high) abort();
        for (count = 0; count < pops; count++) {
            Node *p = func_80090284();
            if (p != (count < 100 ? D_80138880 + count : 0)) abort();
            if (count < 100) {
                Node *e = expected + count;
                e->next = 0; e->field4 = 0; e->field6 = -1; e->field8 = 0;
                e->fieldC = 0; e->field10 = 0.0f; e->field14 = 0;
            }
            if (memcmp(expected, D_80138880, sizeof(expected))) abort();
        }
        for (i = 0; i < 100; i++) {
            Node *p = D_80138880 + i;
            emit(native_pointer(p->next), 4);
            emit((unsigned short)p->field4, 2); emit((unsigned short)p->field6, 2);
            emit((unsigned short)p->field8, 2); emit(p->padA[0], 1); emit(p->padA[1], 1);
            if (p->fieldC) abort();
            emit(0, 4); memcpy(&bits, &p->field10, 4); emit(bits, 4); emit(p->field14, 4);
        }
        emit(native_pointer(D_801392C8), 4); emit(native_pointer(D_801391F0), 4);
        emit((unsigned short)D_8012E66C, 2); emit((unsigned short)D_8012E678, 2);
    }
    return ferror(stdin) ? 1 : 0;
}
