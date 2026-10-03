/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef int s32;
typedef struct Resource { u8 unknown00[16]; u8 channel, file_no; } Resource;
typedef struct ResourceNode { Resource *resource; } ResourceNode;
typedef struct FileRecord { s8 active; u8 unknown01[39]; } FileRecord;
typedef struct ChannelControl {
    u8 unknown00;
    s8 active;
    u8 unknown02[7];
    s8 retained, files_active;
    u8 unknown0B[121];
    FileRecord files[16];
} ChannelControl;
extern ChannelControl D_80144030[];
void func_800A3424(ResourceNode *node, s32 active) {
    Resource *resource;
    ChannelControl *table = D_80144030;
    u8 channel;
    FileRecord *scan;
    s32 offset;
    if (node != (void *)0) {
        resource = node->resource;
        channel = resource->channel;
        if (active != table[channel].files[resource->file_no].active) {
            table[channel].files[resource->file_no].active = active;
            if (active) {
                table[channel].files_active = 1;
            } else {
                scan = table[channel].files;
                for (offset = 0; offset < 640; offset += 40, scan++) {
                    if (scan->active != offset) break;
                }
                if (offset >= 640) {
                                table[channel].files_active = 0;
                    if (table[channel].retained == 0) table[channel].active = 0;
                }
            }
        }
    }
}
