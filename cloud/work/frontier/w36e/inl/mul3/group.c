typedef struct CT { unsigned char a, b, mirror, d, e; } CT;
extern int gN, gT, gR, gD; extern char gC; extern CT gCT[]; extern char gO[];
extern void *memcpy(void *, const void *, unsigned int); extern void *gV[8];
extern void Other(int);
void root(void)
{
    memcpy(gV[6], gV[gD], (gN * 5 + 7) * 8);
    if ((gR == 3 && ((unsigned char *)gCT)[gC * 5 + 2] == 1) || (gR != 3 && gO[28])) Other(1);
    Other(2);
}
