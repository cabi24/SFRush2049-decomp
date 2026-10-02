/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef int s32;
typedef struct PadConfig {
    void *data;
    u8 *color;
    unsigned char fields8[16];
    u8 status;
} PadConfig;
typedef struct FiveParts {
    u8 prefix[2];
    u8 border[4];
    u8 interior[4];
    u8 gap[2];
    PadConfig *part[5];
} FiveParts;
extern void Input_ApplyPadConfig(PadConfig *);
void tournament_unlock_check(FiveParts *group, u8 *border, u8 *interior)
{
    FiveParts *cursor=group;
    s32 offset;
    group->border[0]=border[0];
    group->border[1]=border[1];
    group->border[2]=border[2];
    group->border[3]=border[3];
    group->interior[0]=interior[0];
    group->interior[1]=interior[1];
    group->interior[2]=interior[2];
    group->interior[3]=interior[3];
    for(offset=0;offset<20;offset+=4,cursor=(FiveParts *)((u8 *)cursor+4)) {
        if(offset==8) {
            cursor->part[0]->color=group->border;
            cursor->part[0]->status=border[3];
        } else {
            cursor->part[0]->color=group->interior;
            cursor->part[0]->status=interior[3];
        }
        Input_ApplyPadConfig(cursor->part[0]);
    }
}
