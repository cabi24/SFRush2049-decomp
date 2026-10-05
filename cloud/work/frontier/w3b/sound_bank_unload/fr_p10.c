/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete native reconstruction; historical name is not audio semantics. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Resource24 { u8 directory_fields[20]; u16 *data; } Resource24;
typedef union Handle { s32 value; struct { s16 bank, id; } halves; } Handle;
typedef struct Handles64 { Handle root; u32 unknown04[3]; Handle secondary, body; u32 unknown18[10]; } Handles64;
typedef struct Model68 { u8 model_fields[56]; Resource24 *palette; u32 texture0, texture1; } Model68;
extern u8 D_8013FE90[];
extern u32 D_8013F300[];
extern Resource24 D_8013FEE0[];
extern Handles64 D_80139320[];
extern Model68 D_8012E700[];
extern volatile u8 D_80140BDC;
extern char D_80123498[];
extern s32 D_80152770;
extern void *audio_dma_sync(void *,u32);
extern Resource24 *sound_bank_load(char *,u16 *,s8,s8,s32);
extern s32 sprintf(char *,char *,...);
extern void *memcpy(void *,const void *,u32);
extern s32 osRecvMesg(void *,void **,s32);
extern s32 osJamMesg(void *,void *,s32);
extern void audio_reverb_update(u32,s32);

u32 func_800B0EA0(s32 ratio, u32 left, u32 right)
{
    u32 inverse, red, green, blue;
    if (ratio == 0) return left;
    if (ratio >= 255) return right;
    inverse = 255U - (u32)ratio;
    red = ((s32)((left & 0xF800) * inverse + (right & 0xF800) * (u32)ratio) >> 8) & 0xF800;
    green = ((s32)((left & 0x7C0) * inverse + (right & 0x7C0) * (u32)ratio) >> 8) & 0x7C0;
    blue = ((s32)((left & 0x3E) * inverse + (right & 0x3E) * (u32)ratio) >> 8) & 0x3E;
    return red | green | blue | 1;
}


void func_800B0F60(s32 id, Resource24 *descriptor)
{
    D_8012E700[id].palette=descriptor;
}

void sound_bank_unload(s16 index,s16 palette,u8 first,u8 second,u8 third)
{
    char name[32];
    u16 *colors;
    u16 handle;
    s32 pad[10];
    u16 *cursor;
    s32 i, ratio, value;
    u32 first_color, second_color, third_color;
    Resource24 *descriptor;
    D_8013FE90[index]=1;
    colors=audio_dma_sync(0,512);
    sprintf(name,D_80123498,palette+1);
    memcpy(colors,sound_bank_load(name,&handle,0,D_80140BDC-1,1)->data,512);
    if(first==0) {
        for(i=1;i<32;i++) {
            u16 *p = &colors[i];
            value=(s32)((((colors[i]>>11)&0x7f)<<3)*1.25f);
            if(value>255) value=255;
            colors[i]=((value<<8)&0xf800)|((value<<3)&0x7c0)|((value>>2)&0x3e)|1;
        }
    } else if(second==0) {
        for(i=33;i<64;i++) {
            u16 *p = &colors[i];
            value=(s32)((((colors[i]>>11)&0x7f)<<3)*1.25f);
            if(value>255) value=255;
            colors[i]=((value<<8)&0xf800)|((value<<3)&0x7c0)|((value>>2)&0x3e)|1;
        }
    }
    first_color=D_8013F300[first];
    second_color=D_8013F300[second];
    third_color=D_8013F300[third];
    colors[32]=first_color;
    for(ratio=0,cursor=&colors[33];ratio<248;ratio+=8) *cursor++=func_800B0EA0(ratio,first_color,1);
    colors[64]=second_color;
    for(ratio=0,cursor=&colors[65];ratio<248;ratio+=8) *cursor++=func_800B0EA0(ratio,second_color,1);
    colors[96]=third_color;
    for(ratio=0,cursor=&colors[97];ratio<240;ratio+=8) *cursor++=func_800B0EA0(ratio,third_color,1);
    descriptor=&D_8013FEE0[index];
    memcpy(descriptor->data,colors,512);
    osRecvMesg(&D_80152770,0,1);
    audio_reverb_update((u32)colors,0);
    osJamMesg(&D_80152770,0,0);
    func_800B0F60(D_80139320[index].root.halves.id,descriptor);
    func_800B0F60(D_80139320[index].secondary.halves.id,descriptor);
}
