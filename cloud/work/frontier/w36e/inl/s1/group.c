typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait; extern int gObj;
extern void RemoveBlit(Blit *); extern void Other(int); extern void RemoveObj(short); extern Blit *NewBlit(int);
void unload(void)
{
    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
}
void load(void)
{
    unload();
    if (gX[5] >= 0 && gX[5] < 19) {
        gA = NewBlit(gX[5]);
        if (gX[6] == 1) gB = NewBlit(7);
    }
}
void build(void)
{
    load();
    if (gX[7] >= 0 && gX[7] < 6) { Other(1); Other(2); gX[8] = 3; }
    else if (gObj == -1) { gObj = gX[9]; Other(3); }
    gX[10] = gX[7];
}
void clear(void)
{
    unload();
    if (gObj != -1) { RemoveObj(gObj); gObj = -1; }
    while (gWait) {}
}
void anim(void)
{
    if (gX[11] != gX[12]) {
        gX[13] -= 4;
        if (gX[13] < 0) { gX[13] = 0; clear(); build(); }
    } else { gX[13] += 4; }
    Other(gX[13]);
}
void uninit(void)
{
    int i;
    for (i = 0; i < 48; i++) if (gX[i] != -1) { Other(i); gX[i] = -1; }
    clear();
    Other(99);
}
void root(void)
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
    if (gX[50] > 50) Other(50);
    if (gX[51] > 51) Other(51);
    if (gX[52] > 52) Other(52);
    if (gX[53] > 53) Other(53);
    if (gX[54] > 54) Other(54);
    if (gX[55] > 55) Other(55);
    if (gX[56] > 56) Other(56);
    if (gX[57] > 57) Other(57);
    if (gX[58] > 58) Other(58);
    if (gX[59] > 59) Other(59);
    if (gX[60] > 60) Other(60);
    if (gX[61] > 61) Other(61);
    if (gX[62] > 62) Other(62);
    if (gX[63] > 63) Other(63);
    if (gX[0] > 64) Other(64);
    if (gX[1] > 65) Other(65);
    if (gX[2] > 66) Other(66);
    if (gX[3] > 67) Other(67);
    if (gX[4] > 68) Other(68);
    if (gX[5] > 69) Other(69);
    if (gX[6] > 70) Other(70);
    if (gX[7] > 71) Other(71);
    if (gX[8] > 72) Other(72);
    if (gX[9] > 73) Other(73);
    if (gX[10] > 74) Other(74);
    if (gX[11] > 75) Other(75);
    if (gX[12] > 76) Other(76);
    if (gX[13] > 77) Other(77);
    if (gX[14] > 78) Other(78);
    if (gX[15] > 79) Other(79);
    if (gX[16] > 80) Other(80);
    if (gX[17] > 81) Other(81);
    if (gX[18] > 82) Other(82);
    if (gX[19] > 83) Other(83);
    if (gX[20] > 84) Other(84);
    if (gX[21] > 85) Other(85);
    if (gX[22] > 86) Other(86);
    if (gX[23] > 87) Other(87);
    if (gX[24] > 88) Other(88);
    if (gX[25] > 89) Other(89);
    if (gX[26] > 90) Other(90);
    if (gX[27] > 91) Other(91);
    if (gX[28] > 92) Other(92);
    if (gX[29] > 93) Other(93);
    if (gX[30] > 94) Other(94);
    if (gX[31] > 95) Other(95);
    if (gX[32] > 96) Other(96);
    if (gX[33] > 97) Other(97);
    if (gX[34] > 98) Other(98);
    if (gX[35] > 99) Other(99);
    if (gX[36] > 100) Other(100);
    if (gX[37] > 101) Other(101);
    if (gX[38] > 102) Other(102);
    if (gX[39] > 103) Other(103);
    if (gX[40] > 104) Other(104);
    if (gX[41] > 105) Other(105);
    if (gX[42] > 106) Other(106);
    if (gX[43] > 107) Other(107);
    if (gX[44] > 108) Other(108);
    if (gX[45] > 109) Other(109);
    if (gX[46] > 110) Other(110);
    if (gX[47] > 111) Other(111);
    if (gX[48] > 112) Other(112);
    if (gX[49] > 113) Other(113);
    if (gX[50] > 114) Other(114);
    if (gX[51] > 115) Other(115);
    if (gX[52] > 116) Other(116);
    if (gX[53] > 117) Other(117);
    if (gX[54] > 118) Other(118);
    if (gX[55] > 119) Other(119);
    if (gX[56] > 120) Other(120);
    if (gX[57] > 121) Other(121);
    if (gX[58] > 122) Other(122);
    if (gX[59] > 123) Other(123);
    if (gX[60] > 124) Other(124);
    if (gX[61] > 125) Other(125);
    if (gX[62] > 126) Other(126);
    if (gX[63] > 127) Other(127);
    if (gX[0] > 128) Other(128);
    if (gX[1] > 129) Other(129);
    if (gX[2] > 130) Other(130);
    if (gX[3] > 131) Other(131);
    if (gX[4] > 132) Other(132);
    if (gX[5] > 133) Other(133);
    if (gX[6] > 134) Other(134);
    if (gX[7] > 135) Other(135);
    if (gX[8] > 136) Other(136);
    if (gX[9] > 137) Other(137);
    if (gX[10] > 138) Other(138);
    if (gX[11] > 139) Other(139);
    if (gX[12] > 140) Other(140);
    if (gX[13] > 141) Other(141);
    if (gX[14] > 142) Other(142);
    if (gX[15] > 143) Other(143);
    if (gX[16] > 144) Other(144);
    if (gX[17] > 145) Other(145);
    if (gX[18] > 146) Other(146);
    if (gX[19] > 147) Other(147);
    if (gX[20] > 148) Other(148);
    if (gX[21] > 149) Other(149);

    switch (gX[20]) {
    case 1: clear(); build(); break;
    case 2: clear(); build(); break;
    case 3: anim(); break;
    case 4: uninit(); break;
    case 5: uninit(); break;
    case 6: uninit(); break;
    case 7: uninit(); break;
    }
}
