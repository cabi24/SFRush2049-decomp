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
    gX[8] += gX[32] * 11;
    gX[15] += gX[35] * 12;
    gX[22] += gX[38] * 13;
    gX[29] += gX[41] * 14;
    gX[36] += gX[44] * 15;
    gX[43] += gX[47] * 16;
    gX[50] += gX[50] * 17;
    gX[57] += gX[53] * 18;
    gX[0] += gX[56] * 19;
    gX[7] += gX[59] * 20;
    gX[14] += gX[62] * 21;
    gX[21] += gX[1] * 22;
    gX[28] += gX[4] * 23;
    gX[35] += gX[7] * 24;
    gX[42] += gX[10] * 25;
    gX[49] += gX[13] * 26;
    gX[56] += gX[16] * 27;
    gX[63] += gX[19] * 28;
    gX[6] += gX[22] * 29;
    gX[13] += gX[25] * 30;
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
    gX[9] += gX[33] * 11;
    gX[16] += gX[36] * 12;
    gX[23] += gX[39] * 13;
    gX[30] += gX[42] * 14;
    gX[37] += gX[45] * 15;
    gX[44] += gX[48] * 16;
    gX[51] += gX[51] * 17;
    gX[58] += gX[54] * 18;
    gX[1] += gX[57] * 19;
    gX[8] += gX[60] * 20;
    gX[15] += gX[63] * 21;
    gX[22] += gX[2] * 22;
    gX[29] += gX[5] * 23;
    gX[36] += gX[8] * 24;
    gX[43] += gX[11] * 25;
    gX[50] += gX[14] * 26;
    gX[57] += gX[17] * 27;
    gX[0] += gX[20] * 28;
    gX[7] += gX[23] * 29;
    gX[14] += gX[26] * 30;
    unload();
    Other(3);
}

void caller4(void)
{
    gX[4] += gX[4] * 1;
    gX[11] += gX[7] * 2;
    gX[18] += gX[10] * 3;
    gX[25] += gX[13] * 4;
    gX[32] += gX[16] * 5;
    gX[39] += gX[19] * 6;
    gX[46] += gX[22] * 7;
    gX[53] += gX[25] * 8;
    gX[60] += gX[28] * 9;
    gX[3] += gX[31] * 10;
    gX[10] += gX[34] * 11;
    gX[17] += gX[37] * 12;
    gX[24] += gX[40] * 13;
    gX[31] += gX[43] * 14;
    gX[38] += gX[46] * 15;
    gX[45] += gX[49] * 16;
    gX[52] += gX[52] * 17;
    gX[59] += gX[55] * 18;
    gX[2] += gX[58] * 19;
    gX[9] += gX[61] * 20;
    gX[16] += gX[0] * 21;
    gX[23] += gX[3] * 22;
    gX[30] += gX[6] * 23;
    gX[37] += gX[9] * 24;
    gX[44] += gX[12] * 25;
    gX[51] += gX[15] * 26;
    gX[58] += gX[18] * 27;
    gX[1] += gX[21] * 28;
    gX[8] += gX[24] * 29;
    gX[15] += gX[27] * 30;
    unload();
    Other(4);
}

void caller5(void)
{
    gX[5] += gX[5] * 1;
    gX[12] += gX[8] * 2;
    gX[19] += gX[11] * 3;
    gX[26] += gX[14] * 4;
    gX[33] += gX[17] * 5;
    gX[40] += gX[20] * 6;
    gX[47] += gX[23] * 7;
    gX[54] += gX[26] * 8;
    gX[61] += gX[29] * 9;
    gX[4] += gX[32] * 10;
    gX[11] += gX[35] * 11;
    gX[18] += gX[38] * 12;
    gX[25] += gX[41] * 13;
    gX[32] += gX[44] * 14;
    gX[39] += gX[47] * 15;
    gX[46] += gX[50] * 16;
    gX[53] += gX[53] * 17;
    gX[60] += gX[56] * 18;
    gX[3] += gX[59] * 19;
    gX[10] += gX[62] * 20;
    gX[17] += gX[1] * 21;
    gX[24] += gX[4] * 22;
    gX[31] += gX[7] * 23;
    gX[38] += gX[10] * 24;
    gX[45] += gX[13] * 25;
    gX[52] += gX[16] * 26;
    gX[59] += gX[19] * 27;
    gX[2] += gX[22] * 28;
    gX[9] += gX[25] * 29;
    gX[16] += gX[28] * 30;
    unload();
    Other(5);
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
    caller4();
    caller4();
    caller5();
    caller5();
}
