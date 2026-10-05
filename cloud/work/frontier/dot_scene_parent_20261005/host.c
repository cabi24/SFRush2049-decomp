/* The verifier prepends the unchanged submitted C, then this host-only fixture. */
#include <stddef.h>
#include <string.h>

SceneRecord D_8012E700[512];
s32 D_80156990;
typedef char check_record_size[(sizeof(SceneRecord) == 68) ? 1 : -1];
typedef char check_child_offset[(offsetof(SceneRecord, child) == 22) ? 1 : -1];
typedef char check_sibling_offset[(offsetof(SceneRecord, sibling) == 24) ? 1 : -1];

s32 scene_parent_host(const unsigned char *input, s32 bytes, s32 count,
                      s32 argument, unsigned char *output)
{
    s32 result;
    memset(D_8012E700, 0x6B, sizeof(D_8012E700));
    memcpy(D_8012E700, input, bytes);
    D_80156990 = count;
    result = func_800A7BF8((s16)argument);
    memcpy(output, D_8012E700, sizeof(D_8012E700));
    return result;
}
