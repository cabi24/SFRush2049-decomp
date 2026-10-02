/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;
typedef struct Record24 {u8 flag0,pad1[15],flag16,marker17;u16 key18;void *data;} Record24;
typedef struct Input8 {u8 pad0,key,a,b,c,pad5[3];} Input8;
extern Record24 D_8013FEE0[];extern Input8 D_80153E88[];
typedef struct AudioList AudioList;
extern s32 audio_output_setup(AudioList *);extern void *audio_dma_sync(AudioList *,u32);
void func_800BB02C(s32 slot,s32 key,void *data) {
 Record24 *record=&D_8013FEE0[slot];s32 i;
 record->flag0=0;record->flag16=0;record->marker17=255;record->key18=0x8000;
 if(data){record->data=data;return;}
 if((u32)audio_output_setup(0)<512) {
  for(i=0;i<slot;i++)if(D_80153E88[i].key==key) {
   D_80153E88[slot].a=D_80153E88[i].a;record->data=D_8013FEE0[i].data;
   D_80153E88[slot].b=D_80153E88[i].b;D_80153E88[slot].c=D_80153E88[i].c;break;
  }
  if(i!=slot)return;
 }else record->data=audio_dma_sync(0,512);
}
