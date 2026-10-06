/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
void func_800A3424(ResourceNode *node, s32 active)
{
    Resource *resource;
    s32 channel;
    s32 i;

    if (node != 0) {
        resource = node->resource;
        channel = resource->channel;
        if (active != D_80144030[channel].files[resource->file_no].active) {
            D_80144030[channel].files[resource->file_no].active = active;
            if (active) {
                D_80144030[channel].files_active = 1;
            } else {
                for (i = 0; i < 16; i++) {
                    if (D_80144030[channel].files[i].active) {
                        break;
                    }
                }
                if (i >= 16) {
                    D_80144030[channel].files_active = 0;
                    if (D_80144030[channel].retained == 0) {
                        D_80144030[channel].active = 0;
                    }
                }
            }
        }
    }
}
