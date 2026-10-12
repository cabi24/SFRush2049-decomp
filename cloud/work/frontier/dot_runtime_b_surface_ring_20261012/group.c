/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NONMATCH research: project a square and update its surface-object ring.
 * Current-tree baseline is the complete lean surface-ring source. This draft
 * adds a consumed squared-distance scalar and explicit edge statement regions.
 * The regions change stock register allocation; original macro spelling is
 * unknown. No inferred deleted-helper body is retained.
 *
 * Thirteen UNUSED s32 declarations are a DIAGNOSTIC exact-frame allowance:
 * without them the same source has a 192-byte frame, versus retail 248.
 * 13 * 4 = 52 named bytes plus 4 bytes of alignment close the 56-byte gap.
 * They do not establish original locals, capacities, or semantic source truth.
 * Existing locals are ordered to expose the native vector and index homes.
 * Even with this allowance the source remains NONMATCH (105/305 words).
 * No volatile, fictitious call/formal, forced compiler, or array resizing.
 *
 * Valid domain: backed vectors, player index, ring index 0..24, valid selected
 * handle when wrapped, and synchronous helpers honoring their declared views.
 * NaNs/infinities, asynchronous mutation, aliases with local/global structures,
 * original source identity and full-game behavior are not established.
 */
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
    f32 distance_squared;
    s32 i, corner, next;
    s32 frame_residual_00, frame_residual_01, frame_residual_02, frame_residual_03, frame_residual_04, frame_residual_05, frame_residual_06, frame_residual_07, frame_residual_08, frame_residual_09, frame_residual_10, frame_residual_11, frame_residual_12;
    s32 index;
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
    do {
    delta[0] = corners[0][0] - corners[1][0];
    delta[1] = corners[0][1] - corners[1][1];
    delta[2] = corners[0][2] - corners[1][2];
    if (fabsf(delta[1]) > 3.0f) return;
    distance_squared = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
    if (distance_squared > 200.0f) return;
    } while (0);
    do {
    delta[0] = corners[1][0] - corners[2][0];
    delta[1] = corners[1][1] - corners[2][1];
    delta[2] = corners[1][2] - corners[2][2];
    if (fabsf(delta[1]) > 3.0f) return;
    distance_squared = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
    if (distance_squared > 200.0f) return;
    } while (0);
    do {
    delta[0] = corners[2][0] - corners[3][0];
    delta[1] = corners[2][1] - corners[3][1];
    delta[2] = corners[2][2] - corners[3][2];
    if (fabsf(delta[1]) > 3.0f) return;
    distance_squared = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
    if (distance_squared > 200.0f) return;
    } while (0);
    do {
    delta[0] = corners[3][0] - corners[0][0];
    delta[1] = corners[3][1] - corners[0][1];
    delta[2] = corners[3][2] - corners[0][2];
    if (fabsf(delta[1]) > 3.0f) return;
    distance_squared = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
    if (distance_squared > 200.0f) return;
    if (D_80399A70.wrapped) {
        func_8008C874(D_80399A70.objects[D_80399A70.next]);
        index = D_80399A70.next;
        D_80399A70.next++;
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
    } while (0);
}
