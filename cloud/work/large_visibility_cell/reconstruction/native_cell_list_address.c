/* Complete native spatial-cell visibility routine; research, not accepted. */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct VisibilityCell {
    u8 prefix[52];
    float origin[3];
    u8 gap64[4];
    s16 next;
    u8 gap70[10];
    float minimum[3], maximum[3];
} VisibilityCell;
typedef struct SceneObject { u32 flags; u8 rest[64]; } SceneObject;
typedef u32 CellMask[4];
extern VisibilityCell *D_80149B80;
extern s8 D_8014978C, D_8014061A;
extern s16 D_80149D90;
extern SceneObject D_8012E700[];
extern u8 D_8011E748[];
extern CellMask D_8011E75C[];
extern CellMask D_8011B898[], D_8011BFE8[], D_8011C738[];
extern CellMask D_8011CE88[], D_8011D618[], D_8011DC88[];
extern CellMask D_8011E428[], D_8011E438[], D_8011E448[];
extern CellMask D_8011E458[], D_8011E468[], D_8011E548[];
extern CellMask D_8011E558[], D_8011E568[], D_8011E578[];
extern CellMask D_8011E588[], D_8011E598[], D_8011E5A8[];
extern CellMask D_8011E5B8[];
extern float sqrtf(float);
#pragma intrinsic (sqrtf)

typedef int s32; typedef float f32;
void physics_float_calc(s32 view, f32 *position, s32 force_visible) {
    register VisibilityCell **cell_list = &D_80149B80;
    s32 cell_stride = sizeof(VisibilityCell);
    s32 sp84;
    s32 sp80;
    f32 sp7C;
    s32 sp78;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    register SceneObject *var_s0;
    register VisibilityCell *var_v0;
    register f32 temp_f0;
    register f32 temp_f2;
    register f32 var_f12;
    register s32 var_v0_2;
    register s32 var_v1;
    register s8 var_a1;
    register u32 *var_t0;
    register u32 *var_t2;
    u32 *sp64;
    register u8 temp_t1;

    sp84 = 0;
    sp80 = 0;
    sp78 = -1;
    if ((*cell_list) != 0) {
        var_v0 = (*cell_list);
loop_2:
        sp3C = position[0] - var_v0->origin[0];
        sp40 = position[1] - var_v0->origin[1];
        sp44 = position[2] - var_v0->origin[2];
        if ((var_v0->minimum[0] <= sp3C) && (sp3C <= var_v0->maximum[0])) {
            temp_f2 = var_v0->minimum[1];
            if ((temp_f2 <= sp40) && (var_v0->minimum[2] <= sp44) && (sp44 <= var_v0->maximum[2])) {
                temp_f0 = sqrtf((sp44 * sp44) + (sp3C * sp3C));
                var_f12 = temp_f0;
                if ((var_v0->maximum[1] * 0.75f) < sp40) {
                    var_f12 = temp_f0 + (sp40 - temp_f2);
                }
                if ((sp78 < 0) || (var_f12 < sp7C)) {
                    sp78 = sp84;
                    sp7C = var_f12;
                    sp80 = ((char *)var_v0 - (char *)(*cell_list)) / cell_stride;
                }
            }
        }
        if (var_v0->next >= 0) {
            sp84 += 1;
            var_v0 = (VisibilityCell *)((char *)(*cell_list) + var_v0->next * cell_stride);
            goto loop_2;
        }
        var_t2 = D_8011E75C[0];
        if (sp78 < 0) {
            var_a1 = D_8014978C;
        } else {
            var_a1 = D_8014978C;
            var_t2 = sp64;
            switch (var_a1) {
            case 0:
                var_t2 = D_8011B898[sp78];
                break;
            case 1:
                var_t2 = D_8011BFE8[sp78];
                break;
            case 2:
                var_t2 = D_8011C738[sp78];
                break;
            case 3:
                var_t2 = D_8011CE88[sp78];
                break;
            case 4:
                var_t2 = D_8011D618[sp78];
                break;
            case 5:
                var_t2 = D_8011DC88[sp78];
                break;
            case 6:
                var_t2 = D_8011E428[sp78];
                break;
            case 7:
                var_t2 = D_8011E438[sp78];
                break;
            case 8:
                var_t2 = D_8011E448[sp78];
                break;
            case 9:
                var_t2 = D_8011E458[sp78];
                break;
            case 10:
                var_t2 = D_8011E468[sp78];
                break;
            case 11:
                var_t2 = D_8011E548[sp78];
                break;
            case 12:
                var_t2 = D_8011E558[sp78];
                break;
            case 13:
                var_t2 = D_8011E568[sp78];
                break;
            case 14:
                var_t2 = D_8011E578[sp78];
                break;
            case 15:
                var_t2 = D_8011E588[sp78];
                break;
            case 16:
                var_t2 = D_8011E598[sp78];
                break;
            case 17:
                var_t2 = D_8011E5A8[sp78];
                break;
            case 18:
                var_t2 = D_8011E5B8[sp78];
                break;
            }
        }
        temp_t1 = D_8011E748[var_a1];
        var_v0_2 = 1;
        var_v1 = 0;
        if ((s32) temp_t1 > 0) {
            var_s0 = &D_8012E700[D_80149D90];
            var_t0 = var_t2;
            do {
                if ((var_v1 != 0) && !(var_v1 & 0x1F)) {
                    var_t0 = var_t0 + 1;
                    var_v0_2 = 1;
                }
                var_v1 += 1;
                if (((*var_t0 & var_v0_2) || (force_visible != 0)) && (D_8014061A == 0)) {
                    var_s0->flags &= 0x7FFFFFFF;
                } else {
                    var_s0->flags |= 0x80000000;
                }
                var_v0_2 *= 2;
                var_s0 = var_s0 + 1;
            } while (var_v1 != temp_t1);
        }
    }
}
