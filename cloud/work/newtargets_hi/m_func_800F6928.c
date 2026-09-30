typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct { float x, y; } Pt;
extern Pt D_80114750[];
extern float D_80114744;
extern u32 D_80149B88;
void func_8008A644(u16 a);
void func_800F6928(float x) {
    s32 i = 0;
    while (D_80114750[i].x < x && D_80114750[i].x < 32000.0f) {
        i++;
    }
    if (i > 0) {
/*@1: float x0 = D_80114750[i - 1].x;\n float y0 = D_80114750[i - 1].y;\n float x1 = D_80114750[i].x;\n float y1 = D_80114750[i].y; || float x0 = D_80114750[i - 1].x;\n float y0 = D_80114750[i - 1].y;\n float y1 = D_80114750[i].y;\n float x1 = D_80114750[i].x; || Pt *p = &D_80114750[i];\n Pt *q = &D_80114750[i - 1];\n float x0 = q->x;\n float y0 = q->y;\n float x1 = p->x;\n float y1 = p->y; || Pt *p = &D_80114750[i];\n Pt *q = &D_80114750[i - 1];\n float x0 = q->x;\n float y0 = q->y;\n float y1 = p->y;\n float x1 = p->x; */
/*@2: x = (x - x0) * (y1 - y0) / (x1 - x0) + y0; || x = (x - x0) / (x1 - x0) * (y1 - y0) + y0; || x = y0 + (x - x0) * (y1 - y0) / (x1 - x0); || x = (x - x0) * (y1 - y0) / (x1 - x0) + y0; */
    }
    D_80114744 = x;
    if (x > 0.0f) {
        D_80149B88 |= 0x10;
        func_8008A644((u32)x);
    } else {
        D_80149B88 &= ~0x10;
    }
}
