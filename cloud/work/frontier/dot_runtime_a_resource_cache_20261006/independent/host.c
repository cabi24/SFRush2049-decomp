#include REVIEW_SOURCE
ResourceCache D_803BA230[16];
s16 D_80142B08[32768];
unsigned out, trace[128];
int trace_count, mutation, return_handle, input_id;
static void snapshot(void) {
 int i;
 for(i=0;i<16;i++) trace[trace_count++]=(unsigned)D_803BA230[i].handle;
 for(i=0;i<16;i++) trace[trace_count++]=(unsigned char)D_803BA230[i].loaded;
 for(i=0;i<16;i++) trace[trace_count++]=(unsigned char)D_803BA230[i].id;
 trace[trace_count++]=out;
 trace[trace_count++]=input_id>=0 ? (unsigned short)D_80142B08[input_id] : 0x5a5a;
}
s32 audio_frame_sync(s32 kind,s32 skip,s32 async,s32 flag,void *buffer) {
 trace[trace_count++]=0x80097798;
 trace[trace_count++]=kind;trace[trace_count++]=skip;trace[trace_count++]=async;trace[trace_count++]=flag;trace[trace_count++]=(unsigned long)buffer;
 snapshot();
 if(mutation) {D_803BA230[3].loaded=(s8)0x80; D_803BA230[4].handle=0x76543210;}
 return return_handle;
}
void func_800BB02C(s32 slot,s32 kind,void *data) {
 trace[trace_count++]=0x800BB02C;trace[trace_count++]=slot;trace[trace_count++]=kind;trace[trace_count++]=(unsigned long)data;
 snapshot();
 if(mutation) {D_803BA230[3].loaded=(s8)0x80; D_803BA230[4].handle=0x76543210;out=0x12345678;D_80142B08[input_id]=(s16)0xfedc;}
}
int run(int id,int kind,int handle,int mut) {
 out=0x89abcdef; trace_count=0;mutation=mut;return_handle=handle;input_id=id;
 return func_80390BC0((s16)id,(s16)kind,(s32*)&out);
}
