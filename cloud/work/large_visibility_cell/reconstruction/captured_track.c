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

void physics_float_calc(int view, float *position, int force_visible)
{
    int iteration = 0;
    int nearest = -1;
    int nearest_cell = 0;
    float nearest_distance;
    float relative[3];
    register VisibilityCell *base = D_80149B80, *cell = base;
    register u32 *mask;
    register float distance;
    register int object_count, i, word, track;
    register u32 bit;
    register SceneObject *object;

    if (!base)
        return;
    for (;;) {
        relative[0] = position[0] - cell->origin[0];
        relative[1] = position[1] - cell->origin[1];
        relative[2] = position[2] - cell->origin[2];
        if (cell->minimum[0] <= relative[0] && relative[0] <= cell->maximum[0] &&
            cell->minimum[1] <= relative[1] && cell->minimum[2] <= relative[2] &&
            relative[2] <= cell->maximum[2]) {
            distance = sqrtf(relative[2] * relative[2] + relative[0] * relative[0]);
            if (relative[1] > cell->maximum[1] * 0.75f)
                distance += relative[1] - cell->minimum[1];
            if (nearest < 0 || distance < nearest_distance) {
                nearest = iteration;
                nearest_distance = distance;
                nearest_cell = cell - base;
            }
        }
        if (cell->next < 0)
            break;
        iteration++;
        cell = &base[cell->next];
    }

    track = D_8014978C;
    if (nearest < 0) {
        mask = D_8011E75C[0];
    } else {
        switch (track) {
        case 0: mask = D_8011B898[nearest]; break;
        case 1: mask = D_8011BFE8[nearest]; break;
        case 2: mask = D_8011C738[nearest]; break;
        case 3: mask = D_8011CE88[nearest]; break;
        case 4: mask = D_8011D618[nearest]; break;
        case 5: mask = D_8011DC88[nearest]; break;
        case 6: mask = D_8011E428[nearest]; break;
        case 7: mask = D_8011E438[nearest]; break;
        case 8: mask = D_8011E448[nearest]; break;
        case 9: mask = D_8011E458[nearest]; break;
        case 10: mask = D_8011E468[nearest]; break;
        case 11: mask = D_8011E548[nearest]; break;
        case 12: mask = D_8011E558[nearest]; break;
        case 13: mask = D_8011E568[nearest]; break;
        case 14: mask = D_8011E578[nearest]; break;
        case 15: mask = D_8011E588[nearest]; break;
        case 16: mask = D_8011E598[nearest]; break;
        case 17: mask = D_8011E5A8[nearest]; break;
        case 18: mask = D_8011E5B8[nearest]; break;
        }
    }
    object_count = D_8011E748[track];
    bit = 1;
    word = 0;
    object = &D_8012E700[D_80149D90];
    for (i = 0; i < object_count; i++, object++) {
        if (i != 0 && (i & 31) == 0) {
            word++;
            bit = 1;
        }
        if ((mask[word] & bit) || force_visible) {
            if (!D_8014061A)
                object->flags &= 0x7FFFFFFF;
            else
                object->flags |= 0x80000000;
        } else {
            object->flags |= 0x80000000;
        }
        bit <<= 1;
    }
}
