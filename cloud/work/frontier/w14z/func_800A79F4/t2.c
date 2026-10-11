typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;

typedef struct {
    void *p0;   /* 0x00 */
    void *p4;   /* 0x04 */
    s16 id;     /* 0x08 */
    s16 a;      /* 0x0A */
    s16 b;      /* 0x0C */
    s16 e;      /* 0x0E */
    s16 w;      /* 0x10 */
    s16 h;      /* 0x12 */
    u8 alpha;   /* 0x14 */
    u8 f15;     /* 0x15 */
    s8 state;   /* 0x16 */
    s8 pad17;
    s16 x18;    /* 0x18 */
    s16 x1A;    /* 0x1A */
    s16 hm1;    /* 0x1C */
    s16 wm1;    /* 0x1E */
} Slot;

extern Slot D_80140BF0[];
extern s32 D_801613AC;
extern s32 D_8013C234;

s32 func_800A79F4(s32 id, void *p4, void *p0, s32 a, s32 b, s32 w, s32 h)
{
    Slot *s;
    s32 i;

    for (i = 0; i < D_801613AC; i++) {
        /*@{ld*/if (D_80140BF0[i].state == 2) {
            break;
        }/*@| if (D_80140BF0[i].state == 2) break; @}*/
    }
    /*@{ck*/if (i >= 200) {
        return -1;
    }/*@| if (i >= 200) return -1; @}*/
    /*@{sp*/s = &D_80140BF0[i];/*@| @}*/
    /*@{ci*/if (i >= D_801613AC) {
        D_801613AC++;
    }/*@| if (i >= D_801613AC) D_801613AC = i + 1; @}*/
    /*@{cm*/if (D_8013C234 < D_801613AC) {
        D_8013C234 = D_801613AC;
    }/*@| if (D_8013C234 < D_801613AC) D_8013C234 = D_801613AC; @}*/
    /*@{sp*//*@| s = &D_80140BF0[i]; @}*/
    /*@{bf*/s->b = b;/*@|@}*/
    /*@{pp*/s->p4 = p4;
    s->p0 = p0;/*@| s->p0 = p0;
    s->p4 = p4; @}*/
    s->id = id;
    /*@{ia*/s->a = a;/*@| @}*/
    s->e = 0;
    /*@{al*/s->alpha = 255;/*@| s->alpha = (u8)255; @}*/
    s->x18 = 0;
    s->x1A = 0;
    s->wm1 = w - 1;
    s->hm1 = h - 1;
    /*@{sf*/s->state = 0;
    s->f15 = 0;/*@| s->f15 = 0;
    s->state = 0; @}*/
    s->w = w;
    s->h = h;
    /*@{ia*//*@| s->a = a; @}*/
    /*@{bf*//*@| s->b = b; @}*/

    return i;
}
