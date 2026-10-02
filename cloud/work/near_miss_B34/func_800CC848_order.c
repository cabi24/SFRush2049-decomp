/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef int s32;
typedef struct Meta {u8 pad[0x10];u8 channel;} Meta;
typedef struct Device {Meta *meta;} Device;
typedef struct Container {s32 words[2];Device *device;} Container;
typedef struct Handle {Container *container;} Handle;
extern struct Channel {u8 byte0;s8 status;u8 tail[0x302];} D_80144030[];
extern s32 func_800A1A60(Device*);
s32 func_800CC848(Handle *handle,s32 active) {
    Device *device=handle->container->device;
    s32 status;
    s32 channel;
    if(device==0) return 1;
    channel=device->meta->channel;
    status=D_80144030[channel].status;
    if(status==0) return 0;
    if(active==0) return 1;
    return func_800A1A60(device);
}
