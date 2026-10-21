typedef struct CT { unsigned char a, b, mirror, d, e; } CT;
extern int gN, gT, gR, gD; extern char gC; extern CT gCT[]; extern char gO[];
extern void *memcpy(void *, const void *, unsigned int); extern void *gV[8]; extern void *gW[8];
extern void Other(int);
void f(void)
{
    memcpy(gW[6], gW[gD], gN * 64 + 64);
    memcpy(gV[6], gV[gD], (gN * 5 + 7) * 8);
    if ((gR == 3 && gCT[gC].mirror == 1) || (gR != 3 && gO[28])) Other(1);
    if (gCT[gT].d) Other(3);
    Other(2);
}
void root(void)
{
    f();
    Other(5);
    f();
}
