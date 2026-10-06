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
