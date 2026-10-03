/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef struct Entry40 {s8 active;u8 opaque[39];} Entry40;
typedef struct Row772 {u8 first;s8 ready;u8 opaque[7];s8 busy;s8 changed;u8 other[121];Entry40 entries[16];} Row772;
typedef struct Description {u8 opaque[16];u8 port,slot;} Description;
typedef struct Handle {Description *data;} Handle;
extern Row772 D_80144030[];
void func_800A3424(Handle *handle,int value)
{
    int port,i;
    if(handle==0)return;
    port=handle->data->port;
    if(value==D_80144030[port].entries[handle->data->slot].active)return;
    D_80144030[port].entries[handle->data->slot].active=value;
    if(value!=0)D_80144030[port].changed=1;
    else {
        for(i=0;i<16;i++)if(D_80144030[port].entries[i].active)break;
        if(i>=16) {
            D_80144030[port].changed=0;
            if(D_80144030[port].busy==0)D_80144030[port].ready=0;
        }
    }
}
