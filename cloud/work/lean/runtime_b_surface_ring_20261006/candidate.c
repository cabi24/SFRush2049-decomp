/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Runtime image B: project a square and update its surface-object ring. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct Record88 Record88;
typedef struct Poly Poly;
typedef struct RingState { s8 wrapped; u8 unknown01; u16 next; Record88 *objects[25]; } RingState;
typedef struct Vehicle { u8 unknown000[1516]; f32 compression[4]; u8 unknown5FC[524]; } Vehicle;
extern Vehicle D_8014A250[];
extern RingState D_80399A70;
extern u16 D_80399AD8;
extern f32 D_80394888[4][3];
extern u8 D_803948B8[][4];
extern Poly *camera_trigger_check(f32 *,f32 *,f32[3][3]);
extern void func_8008C874(Record88 *);
extern Record88 *func_800A78BC(s32,f32 *,u16,u8 *,u16,s32);
extern void func_8008C074(Record88 *,s32,f32 *,u16,u8 *,u16,s32);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
void func_8038A408(f32 *position, s32 player)
{
    f32 hit[3];
    f32 normal[3][3];
    f32 corners[4][3];
    f32 delta[3];
    s32 i, next, index, corner;
    if (D_8014A250[player].compression[0] > 8.0f ||
        D_8014A250[player].compression[1] > 8.0f ||
        D_8014A250[player].compression[2] > 8.0f ||
        D_8014A250[player].compression[3] > 8.0f) return;
    for (corner = 0; corner < 4; corner++) {
        corners[corner][0] = position[0];
        corners[corner][1] = position[1];
        corners[corner][2] = position[2];
        corners[corner][1] += 3.0f;
        switch (corner) {
        case 0: corners[0][0] += -6.0f; corners[0][2] += 6.0f; break;
        case 1: corners[1][0] += -6.0f; corners[1][2] += -6.0f; break;
        case 2: corners[2][0] += 6.0f; corners[2][2] += -6.0f; break;
        case 3: corners[3][0] += 6.0f; corners[3][2] += 6.0f; break;
        }
        if (camera_trigger_check(corners[corner], hit, normal)) {
            corners[corner][0] = hit[0];
            corners[corner][2] = hit[2];
            corners[corner][1] = hit[1] + 0.25f;
        }
    }
    delta[0] = corners[0][0] - corners[1][0];
    delta[1] = corners[0][1] - corners[1][1];
    delta[2] = corners[0][2] - corners[1][2];
    if (fabsf(delta[1]) > 3.0f) return;
    if (delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2] > 200.0f) return;
    delta[0] = corners[1][0] - corners[2][0];
    delta[1] = corners[1][1] - corners[2][1];
    delta[2] = corners[1][2] - corners[2][2];
    if (fabsf(delta[1]) > 3.0f) return;
    if (delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2] > 200.0f) return;
    delta[0] = corners[2][0] - corners[3][0];
    delta[1] = corners[2][1] - corners[3][1];
    delta[2] = corners[2][2] - corners[3][2];
    if (fabsf(delta[1]) > 3.0f) return;
    if (delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2] > 200.0f) return;
    delta[0] = corners[3][0] - corners[0][0];
    delta[1] = corners[3][1] - corners[0][1];
    delta[2] = corners[3][2] - corners[0][2];
    if (fabsf(delta[1]) > 3.0f) return;
    if (delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2] > 200.0f) return;
    if (D_80399A70.wrapped) {
        func_8008C874(D_80399A70.objects[D_80399A70.next]);
        index = D_80399A70.next++;
        if (D_80399A70.next >= 25) D_80399A70.next = 0;
    } else {
        index = D_80399A70.next;
        D_80399A70.objects[index] = func_800A78BC(4, &D_80394888[0][0], D_80399AD8, D_803948B8[0], 0x820f, 1);
        if (++D_80399A70.next >= 25) {
            D_80399A70.next = 0;
            D_80399A70.wrapped = 1;
        }
    }
    for (i = 1; i < 5; i++) {
        next = index + i;
        if (next >= 25) next -= 25;
        if (D_80399A70.objects[next]) {
            func_8008C074(D_80399A70.objects[next], 0, 0, 0, D_803948B8[i], 0, 0);
        }
    }
    func_8008C074(D_80399A70.objects[index], 0, &corners[0][0], 0, D_803948B8[0], 0x060f, 0);
}
