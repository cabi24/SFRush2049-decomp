typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait;
extern void RemoveBlit(Blit *); extern void Other(int);

void unload(void)
{
    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
}

void caller0(void)
{
    gX[0] += gX[0] * 1;
    gX[7] += gX[3] * 2;
    gX[14] += gX[6] * 3;
    gX[21] += gX[9] * 4;
    gX[28] += gX[12] * 5;
    gX[35] += gX[15] * 6;
    gX[42] += gX[18] * 7;
    gX[49] += gX[21] * 8;
    gX[56] += gX[24] * 9;
    gX[63] += gX[27] * 10;
    gX[6] += gX[30] * 11;
    gX[13] += gX[33] * 12;
    gX[20] += gX[36] * 13;
    gX[27] += gX[39] * 14;
    gX[34] += gX[42] * 15;
    gX[41] += gX[45] * 16;
    gX[48] += gX[48] * 17;
    gX[55] += gX[51] * 18;
    gX[62] += gX[54] * 19;
    gX[5] += gX[57] * 20;
    gX[12] += gX[60] * 21;
    gX[19] += gX[63] * 22;
    gX[26] += gX[2] * 23;
    gX[33] += gX[5] * 24;
    gX[40] += gX[8] * 25;
    gX[47] += gX[11] * 26;
    gX[54] += gX[14] * 27;
    gX[61] += gX[17] * 28;
    gX[4] += gX[20] * 29;
    gX[11] += gX[23] * 30;
    unload();
    Other(0);
}

void root(void)
{
    caller0();
    caller0();
}
