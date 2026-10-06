/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct Node {u8 other0[4];struct Node *next;u8 other8[4];u32 size;u8 other16[4];s8 disabled;u8 other21[3];} Node;
typedef struct Owner {u8 other0[8];Node *head;} Owner;
typedef struct Queue Queue;
extern Queue D_80152770;
extern Owner *D_801527C8;
extern int osRecvMesg(Queue *,void **,int);
extern int osJamMesg(Queue *,void *,int);
typedef struct Slot24 {u8 flag;u8 other1[15];u8 active,marker;u16 value;void *data;} Slot24;
typedef struct Description8 {u8 other0;u8 kind;u8 payload[3];u8 other5[3];} Description8;
extern Slot24 D_8013FEE0[];
extern Description8 D_80153E88[];
extern u32 audio_output_setup(Owner *);
extern void *audio_dma_sync(int,int);
void func_800BB02C(int index,int kind,void *data)
{
    int i;
    D_8013FEE0[index].flag=0;
    D_8013FEE0[index].active=0;
    D_8013FEE0[index].marker=255;
    D_8013FEE0[index].value=0x8000;
    if(data!=0)D_8013FEE0[index].data=data;
    else if(audio_output_setup(0)<512) {
        for(i=0;i<index;i++) {
            if(kind==D_80153E88[i].kind) {
                D_80153E88[index].payload[0]=D_80153E88[i].payload[0];
                D_8013FEE0[index].data=D_8013FEE0[i].data;
                D_80153E88[index].payload[1]=D_80153E88[i].payload[1];
                D_80153E88[index].payload[2]=D_80153E88[i].payload[2];
                break;
            }
        }
        if(i==index){}
    } else D_8013FEE0[index].data=audio_dma_sync(0,512);
}
