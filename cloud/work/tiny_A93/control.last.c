/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Slot24 {u8 flag;u8 other1[15];u8 active,marker;u16 value;void *data;} Slot24;
typedef struct Description8 {u8 other0;u8 kind;u8 payload[3];u8 other5[3];} Description8;
extern Slot24 D_8013FEE0[];
extern Description8 D_80153E88[];
extern u32 audio_output_setup(int);
extern void *audio_dma_sync(int,int);
void func_800BB02C(int index,int kind,void *data)
{
    Slot24 *slot=&D_8013FEE0[index];
    slot->flag=0;
    slot->active=0;
    slot->marker=255;
    slot->value=0x8000;
    if(data!=0)slot->data=data;
    else if(audio_output_setup(0)<512) {
        int i;
        Description8 *entry=D_80153E88;
        for(i=0;i<index;i++,entry++) {
            if(kind==entry->kind) {
                D_80153E88[index].payload[0]=entry->payload[0];
                slot->data=D_8013FEE0[i].data;
                D_80153E88[index].payload[1]=entry->payload[1];
                D_80153E88[index].payload[2]=entry->payload[2];
                break;
            }
        }
    } else slot->data=audio_dma_sync(0,512);
}
