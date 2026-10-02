typedef unsigned short u16;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct Patch32 {
    u16 id;
    u16 record_index;
    u8 payload[18];
    u16 companion_index;
    u8 companion[8];
} Patch32;
typedef struct Record24 { u8 prefix[4]; u8 payload[18]; u8 tail[2]; } Record24;
extern u16 D_8015267C;
extern Patch32 *D_801525EC;
extern Record24 *D_801497F8;
extern u8 (*D_8015201C)[8];
extern void *memcpy(void *, const void *, u32);
void listener_position_set(s32 id)
{
    Patch32 *patch=D_801525EC;
    s32 i;
    for(i=0;i<D_8015267C;i++,patch++) {
        if(id==patch->id) {
            memcpy(D_801497F8[patch->record_index].payload,patch->payload,18);
            memcpy(D_8015201C[patch->companion_index],patch->companion,8);
        }
    }
}
