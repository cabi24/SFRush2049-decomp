typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait; extern int gObj;
extern void RemoveBlit(Blit *); extern void Other(int); extern void RemoveObj(short);

void unload(void)
{
    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
}
void clear(void)
{
    unload();
    if (gObj != -1) { RemoveObj(gObj); gObj = -1; }
    while (gWait) {}
}

void big(void)
{
    if (gX[0] > 0) Other(0);
    if (gX[1] > 1) Other(1);
    if (gX[2] > 2) Other(2);
    if (gX[3] > 3) Other(3);
    if (gX[4] > 4) Other(4);
    if (gX[5] > 5) Other(5);
    if (gX[6] > 6) Other(6);
    if (gX[7] > 7) Other(7);
    if (gX[8] > 8) Other(8);
    if (gX[9] > 9) Other(9);
    if (gX[10] > 10) Other(10);
    if (gX[11] > 11) Other(11);
    if (gX[12] > 12) Other(12);
    if (gX[13] > 13) Other(13);
    if (gX[14] > 14) Other(14);
    if (gX[15] > 15) Other(15);
    if (gX[16] > 16) Other(16);
    if (gX[17] > 17) Other(17);
    if (gX[18] > 18) Other(18);
    if (gX[19] > 19) Other(19);
    if (gX[20] > 20) Other(20);
    if (gX[21] > 21) Other(21);
    if (gX[22] > 22) Other(22);
    if (gX[23] > 23) Other(23);
    if (gX[24] > 24) Other(24);
    if (gX[25] > 25) Other(25);
    if (gX[26] > 26) Other(26);
    if (gX[27] > 27) Other(27);
    if (gX[28] > 28) Other(28);
    if (gX[29] > 29) Other(29);
    if (gX[30] > 30) Other(30);
    if (gX[31] > 31) Other(31);
    if (gX[32] > 32) Other(32);
    if (gX[33] > 33) Other(33);
    if (gX[34] > 34) Other(34);
    if (gX[35] > 35) Other(35);
    if (gX[36] > 36) Other(36);
    if (gX[37] > 37) Other(37);
    if (gX[38] > 38) Other(38);
    if (gX[39] > 39) Other(39);
    if (gX[40] > 40) Other(40);
    if (gX[41] > 41) Other(41);
    if (gX[42] > 42) Other(42);
    if (gX[43] > 43) Other(43);
    if (gX[44] > 44) Other(44);
    if (gX[45] > 45) Other(45);
    if (gX[46] > 46) Other(46);
    if (gX[47] > 47) Other(47);
    if (gX[48] > 48) Other(48);
    if (gX[49] > 49) Other(49);
    if (gX[3]) clear();
    if (gX[4]) clear();
}

void small(void)
{
    gX[1] = 2;
    clear();
    Other(9);
}

void root(void)
{
    big();
    big();
    small();
    small();
}
