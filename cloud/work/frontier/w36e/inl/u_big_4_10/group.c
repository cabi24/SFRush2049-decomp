typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait; extern int gObj;
extern void RemoveBlit(Blit *); extern void Other(int); extern void RemoveObj(short);

void unload(void)
{
    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
    if (gX[0]) { RemoveBlit(gA); gA = 0; }
    if (gX[1]) { RemoveBlit(gB); gB = 0; }
    if (gX[2]) { RemoveBlit(gA); gA = 0; }
    if (gX[3]) { RemoveBlit(gB); gB = 0; }
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
    unload();
    Other(1);
}

void caller2(void)
{
    gX[2] += gX[2] * 1;
    gX[9] += gX[5] * 2;
    gX[16] += gX[8] * 3;
    gX[23] += gX[11] * 4;
    gX[30] += gX[14] * 5;
    gX[37] += gX[17] * 6;
    gX[44] += gX[20] * 7;
    gX[51] += gX[23] * 8;
    gX[58] += gX[26] * 9;
    gX[1] += gX[29] * 10;
    unload();
    Other(2);
}

void caller3(void)
{
    gX[3] += gX[3] * 1;
    gX[10] += gX[6] * 2;
    gX[17] += gX[9] * 3;
    gX[24] += gX[12] * 4;
    gX[31] += gX[15] * 5;
    gX[38] += gX[18] * 6;
    gX[45] += gX[21] * 7;
    gX[52] += gX[24] * 8;
    gX[59] += gX[27] * 9;
    gX[2] += gX[30] * 10;
    unload();
    Other(3);
}

void root(void)
{
    caller0();
    caller0();
    caller1();
    caller1();
    caller2();
    caller2();
    caller3();
    caller3();
}
