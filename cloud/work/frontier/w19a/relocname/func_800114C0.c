/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern unsigned char D_80038290;
extern float D_8002D8C0;
extern float D_8002D8C4;
extern float D_8002D8C8;
extern unsigned short D_800382A8;
extern unsigned int D_800382AC;
extern void *D_8003802C;
extern OSMesgQueue D_800382B0;
extern OSMesg D_800382C8;
extern volatile unsigned char D_800382CC;
extern unsigned short D_8002C5D0[];
extern void func_80006A00(OSMesgQueue *, OSMesg *, int);
extern void func_80011F60(int);
extern void func_800123A8(unsigned short);
void func_800114C0(unsigned short count, unsigned int frequency, unsigned char mode)
{
    if (D_80038290 == 0) {
        D_800382A8 = count * ((unsigned short) (((float)frequency / 50.0f * D_8002D8C0) / 192.0f) + 1);
    } else {
        D_800382A8 = count * ((unsigned short) (((float)frequency / 25.0f * D_8002D8C4) / 192.0f) + 1);
    }
    D_800382AC = 784;
    if (D_800382AC < 192) {
        D_800382AC = 192;
    }
    D_800382AC = (unsigned int) ((float)(D_800382A8 * D_800382AC) * D_8002D8C8);
    D_800382AC = ((((D_800382AC + 63) >> 6) * 40) + 15) & 0xFFF0;
    D_8003802C = D_80038018(D_800382A8 * 2 * 12U, 0);
    func_80006A00(&D_800382B0, &D_800382C8, 1);
    D_800382CC = 0;
    func_80011F60(D_800382A8);
    func_800123A8((unsigned short)(D_8002C5D0[mode] * count));
}
