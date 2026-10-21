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

void caller1(void)
{
    gX[1] += gX[1] * 1;
    gX[8] += gX[4] * 2;
    gX[15] += gX[7] * 3;
    gX[22] += gX[10] * 4;
    gX[29] += gX[13] * 5;
    gX[36] += gX[16] * 6;
    gX[43] += gX[19] * 7;
    gX[50] += gX[22] * 8;
    gX[57] += gX[25] * 9;
    gX[0] += gX[28] * 10;
    gX[7] += gX[31] * 11;
    gX[14] += gX[34] * 12;
    gX[21] += gX[37] * 13;
    gX[28] += gX[40] * 14;
    gX[35] += gX[43] * 15;
    gX[42] += gX[46] * 16;
    gX[49] += gX[49] * 17;
    gX[56] += gX[52] * 18;
    gX[63] += gX[55] * 19;
    gX[6] += gX[58] * 20;
    gX[13] += gX[61] * 21;
    gX[20] += gX[0] * 22;
    gX[27] += gX[3] * 23;
    gX[34] += gX[6] * 24;
    gX[41] += gX[9] * 25;
    gX[48] += gX[12] * 26;
    gX[55] += gX[15] * 27;
    gX[62] += gX[18] * 28;
    gX[5] += gX[21] * 29;
    gX[12] += gX[24] * 30;
    unload();
    Other(1);
}

void root(void)
{
    caller0();
    caller0();
    caller1();
    caller1();
}
