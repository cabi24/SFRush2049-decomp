/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete physical-address semantic model, NOT an IDO matching candidate.
 * Original external C object boundaries/domain remain unresolved. The bus and
 * service API is a host verification interface, not invented original callees.
 * Each bus address denotes native memory, never a new nine-row/17-ID C object.
 */
#include "semantic_bus.h"

#define OWNER() RS16(descriptor + 8)
#define PLAYER(i) (0x80152818u + (u32)(i) * 0x3B8u)
#define PHYSICS(i) (0x8014A250u + (u32)(i) * 0x808u)
#define SCENE(i) (0x80139320u + (u32)(i) * 0x40u)
#define SLOTS(i) (0x80399120u + (u32)(i) * 0x10Cu)
#define GROUP(i) (0x80399550u + (u32)(i) * 0x148u)
#define CACHE(i) (0x80399118u + (u32)(i))
#define OFFSET_ADDR() (0x803943A4u + (u32)RS8(state + 0x384) * 0x9Cu + RU8(PHYSICS(OWNER()) + 8) * 12u)
#define SLOT_MATRIX(i) (SLOTS(OWNER()) + 0x14u + (u32)(i) * 48u)
#define GROUP_MATRIX(i) (GROUP(OWNER()) + 0x14u + (u32)(i) * 48u)
#define EXTRA_MATRIX() (GROUP(OWNER()) + 0x108u)
#define SLOT_HANDLE(i) R32(SLOTS(OWNER()) + 4u * (i))
#define GROUP_HANDLE(i) R32(GROUP(OWNER()) + 4u * (i))
#define EXTRA_HANDLE() R32(GROUP(OWNER()) + 0x104u)
#define LOCAL_VIEW() (RS8(state + 0x35C) >= 0 && RS8(state + 0x35D) < 2)
#define VIEW_MASK() (1u << (RU8(state + 0x35C) & 31))
/* Ordered stores and freshly evaluated destination owner are intentional. */
#define TRANSFORM_POSITION(destination) do { \
    for (j = 0; j < 3; ++j) { \
        product = offset[0] * RF(source + 4u * j); \
        value = product + RF(source + 0x24u + 4u * j); \
        WF((destination) + 0x24u + 4u * j, value); \
    } \
    for (j = 0; j < 3; ++j) { \
        product = offset[1] * RF(source + 12u + 4u * j); \
        address = (destination) + 0x24u + 4u * j; \
        WF(address, product + RF(address)); \
    } \
    for (j = 0; j < 3; ++j) { \
        product = offset[2] * RF(source + 24u + 4u * j); \
        address = (destination) + 0x24u + 4u * j; \
        WF(address, product + RF(address)); \
    } \
} while (0)

static void func_8038A95C(s16 player)
{
    s32 i;
    u32 address;
    for (i = 0; i < 5; ++i) {
        address = GROUP(player) + 4u * i;
        if (R32(address) != 0xFFFFFFFFu) {
            scene_remove((s16)R32(address));
            W32(address, 0xFFFFFFFFu);
        }
    }
    address = GROUP(player) + 0x104u;
    if (R32(address) != 0xFFFFFFFFu) {
        scene_remove((s16)R32(address));
        W32(address, 0xFFFFFFFFu);
        W8(GROUP(player) + 0x139u, 255);
    }
}

static void func_8038AA14(s16 player)
{
    s32 i;
    u32 address;
    for (i = 0; i < 5; ++i) {
        address = SLOTS(player) + 4u * i;
        if (R32(address) != 0xFFFFFFFFu) {
            scene_remove((s16)R32(address));
            W32(address, 0xFFFFFFFFu);
        }
    }
}

void func_8038AA8C(u32 descriptor, s16 update)
{
    u32 state, source, source_color, player_color, address, texture;
    s32 i, j, kind, resource, selected, result;
    s16 owner;
    f32 offset[3], product, value, angle;

    owner = OWNER();
    state = PLAYER(owner);
    source_color = R32(0x80394884u);
    if (update == 0 || RS8(PHYSICS(OWNER()) + 0x640u) != 0) {
        owner = OWNER();
        kind = RS8(CACHE(owner));
        if (kind == 1) func_8038AA14(owner);
        else if (kind == 0) func_8038A95C(owner);
        W8(state + 0x384u, 8);
        W8(state + 0x385u, 255);
        scene_hide(RS16(descriptor + 6), 0, 15);
        return;
    }
    player_color = R32(state + 0x34Cu);
    if (R32(state + 0x38Cu) & 1u) {
        kind = RU8(state + 0x3A1u);
        if (kind < 48) kind = 48;
        player_color = (player_color & 0xFFFFFF00u) | (u32)kind;
    }
    scene_color(RS16(SCENE(OWNER()) + 0x1Au), player_color);
    source = scene_transform(RS16(SCENE(OWNER()) + 2));
    if (RS8(state + 0x384u) == 8) {
        scene_hide(RS16(descriptor + 6), 1, 15);
    } else if (LOCAL_VIEW()) {
        scene_show(RS16(descriptor + 6), 0, 15);
        scene_hide(RS16(descriptor + 6), 1, VIEW_MASK());
    } else {
        scene_show(RS16(descriptor + 6), 0, 15);
    }
    owner = OWNER();
    if (RS8(state + 0x384u) != RS8(CACHE(owner))) {
        /* These three physical reads occur even for genuine mode 8. */
        offset[0] = RF(OFFSET_ADDR());
        offset[1] = RF(OFFSET_ADDR() + 4);
        offset[2] = RF(OFFSET_ADDR() + 8);
        owner = OWNER();
        kind = RS8(CACHE(owner));
        if (kind == 1) func_8038AA14(owner);
        else if (kind == 0) func_8038A95C(owner);
        W8(CACHE(OWNER()), RU8(state + 0x384u));
        switch (RU8(state + 0x384u)) {
        case 0:
            resource = 216;
            offset[0] += 0.16f;
            offset[1] += 0.48f;
            offset[2] += 2.57f;
            for (i = 0; i < 5; ++i) {
                matrix_copy(source, GROUP_MATRIX(i));
                TRANSFORM_POSITION(GROUP_MATRIX(i));
                result = scene_create(R32(0x80399B40u + 4u*i), GROUP_MATRIX(i), -1, 0x42000u);
                W32(GROUP(OWNER()) + 4u*i, result);
                scene_color((s16)GROUP_HANDLE(i), source_color);
                scene_hide(GROUP_HANDLE(i), 1, 15);
            }
            matrix_copy(source, EXTRA_MATRIX());
            TRANSFORM_POSITION(EXTRA_MATRIX());
            result = scene_create(R32(0x80399B58u), EXTRA_MATRIX(), -1, 0x42000u);
            W32(GROUP(OWNER()) + 0x104u, result);
            scene_color((s16)EXTRA_HANDLE(), source_color);
            scene_hide(EXTRA_HANDLE(), 1, 15);
            W8(GROUP(OWNER()) + 0x138u, 255);
            WF(GROUP(OWNER()) + 0x140u, 0.0f);
            value = random_float(1.5f);
            WF(GROUP(OWNER()) + 0x13Cu, value + 1.5f);
            break;
        case 1:
            resource = 217;
            offset[0] += 0.01f;
            offset[1] += 0.65f;
            offset[2] += 3.59f;
            for (i = 0; i < 5; ++i) {
                matrix_copy(source, SLOT_MATRIX(i));
                TRANSFORM_POSITION(SLOT_MATRIX(i));
                source_color = (source_color & 255u) | 0xFFFF2B00u;
                result = scene_create(R32(0x80399B40u + 4u*i), SLOT_MATRIX(i), -1, 0x42000u);
                W32(SLOTS(OWNER()) + 4u*i, result);
                scene_color((s16)SLOT_HANDLE(i), source_color);
                scene_hide(SLOT_HANDLE(i), 1, 15);
            }
            break;
        case 2: resource = 218; break;
        case 3: resource = 219; break;
        case 4: resource = 220; break;
        case 5: resource = 221; break;
        case 6: resource = 222; break;
        case 7: resource = 223; break;
        case 8: return;
        default:
            /* Native reaches an uninitialized resource-index stack load.
             * No defined semantic contract for this state. Fail closed. */
            invalid_domain(1); return;
        }
        scene_model(RS16(SCENE(OWNER()) + 0x1Au), RU16(0x801427C0u + (u32)resource*2u));
        scene_position(RS16(SCENE(OWNER()) + 0x1Au), OFFSET_ADDR(), 0);
    }
    /* A same-mode-8 call also performs all three alias-preserving loads. */
    offset[0] = RF(OFFSET_ADDR());
    offset[1] = RF(OFFSET_ADDR() + 4);
    offset[2] = RF(OFFSET_ADDR() + 8);
    if (RS8(CACHE(OWNER())) == 1) {
        offset[0] += 0.01f;
        offset[1] += 0.65f;
        offset[2] += 3.59f;
        for (i = 0; i < 5; ++i) {
            matrix_copy(source, SLOT_MATRIX(i));
            TRANSFORM_POSITION(SLOT_MATRIX(i));
        }
        address = SLOTS(OWNER());
        if (R32(address + 0x104u) != 0) {
            WF(address + 0x108u, RF(address + 0x108u) + RF(0x8002EB94u));
            address = SLOTS(OWNER());
            if (0.0333333f <= RF(address + 0x108u)) {
                W32(address + 0x104u, 0);
                for (i = 0; i < 5; ++i) scene_hide(SLOT_HANDLE(i), 1, 15);
            }
        }
        if (RS8(state + 0x3A0u) >= 5) {
            W32(SLOTS(OWNER()) + 0x104u, 1);
            WF(SLOTS(OWNER()) + 0x108u, 0.0f);
            scene_show(SLOT_HANDLE(0), 0, 15);
            if (LOCAL_VIEW()) scene_hide(SLOT_HANDLE(0), 1, VIEW_MASK());
            selected = (s32)random_float(4.0f) + 1;
            angle = random_float(0.5f);
            matrix_roll(angle, SLOT_MATRIX(selected));
            value = random_float(0.75f);
            address = SLOT_MATRIX(selected);
            matrix_scale(address, address, value + 0.85f);
            scene_show(SLOT_HANDLE(selected), 0, 15);
            if (LOCAL_VIEW()) scene_hide(SLOT_HANDLE(selected), 1, VIEW_MASK());
            ++selected;
            if (selected >= 5) selected = 1;
            matrix_roll(angle, SLOT_MATRIX(selected));
            value = random_float(0.75f);
            address = SLOT_MATRIX(selected);
            matrix_scale(address, address, value + 0.85f);
            scene_show(SLOT_HANDLE(selected), 0, 15);
            if (LOCAL_VIEW()) scene_hide(SLOT_HANDLE(selected), 1, VIEW_MASK());
        }
    }
    /* This is a second live cache test, rather than an else of kind 1. */
    if (RS8(CACHE(OWNER())) != 0) return;
    offset[0] += 0.16f;
    offset[1] += 0.48f;
    offset[2] += 2.57f;
    for (i = 0; i < 5; ++i) {
        matrix_copy(source, GROUP_MATRIX(i));
        TRANSFORM_POSITION(GROUP_MATRIX(i));
    }
    matrix_copy(source, EXTRA_MATRIX());
    TRANSFORM_POSITION(EXTRA_MATRIX());
    address = GROUP(OWNER());
    if (RS8(address + 0x138u) != -1) {
        WF(address + 0x140u, RF(address + 0x140u) - RF(0x8002EB94u));
        address = GROUP(OWNER());
        if (RF(address + 0x140u) <= 0.0f) {
            WF(address + 0x140u, 0.0333333f);
            address = GROUP(OWNER());
            W8(address + 0x138u, RS8(address + 0x138u) + 1);
            address = GROUP(OWNER());
            if (RS8(address + 0x138u) >= 5) {
                scene_hide(R32(address + 0x104u), 1, 15);
                W8(GROUP(OWNER()) + 0x138u, 255);
            } else {
                W8(address + 0x139u, RS8(address + 0x139u) + 1);
                address = GROUP(OWNER());
                if (RS8(address + 0x139u) >= 5) W8(address + 0x139u, 0);
                address = GROUP(OWNER());
                texture = RU16(0x80399B08u + (u32)RS8(address + 0x139u)*2u);
                texture = R32(0x80151AE8u + (texture >> 10)*8u) + (texture & 1023u)*36u;
                scene_texture((s16)R32(address + 0x104u), texture, -1);
            }
        }
    }
    address = GROUP(OWNER());
    if (RS16(address + 0x13Au) != 0) {
        WF(address + 0x144u, RF(address + 0x144u) + RF(0x8002EB94u));
        address = GROUP(OWNER());
        if (0.0666667f <= RF(address + 0x144u)) {
            W16(address + 0x13Au, 0);
            for (i = 0; i < 5; ++i) scene_hide(GROUP_HANDLE(i), 1, 15);
        }
    }
    if (RS8(state + 0x3A0u) >= 5) {
        value = random_float(1.5f);
        WF(GROUP(OWNER()) + 0x13Cu, value + 1.5f);
        WF(GROUP(OWNER()) + 0x140u, 0.0333333f);
        W8(GROUP(OWNER()) + 0x138u, 0);
        value = random_float(5.0f);
        W8(GROUP(OWNER()) + 0x139u, (s32)value);
        /* Intentionally texture slot 0, regardless of randomized state139. */
        texture = RU16(0x80399B08u);
        texture = R32(0x80151AE8u + (texture >> 10)*8u) + (texture & 1023u)*36u;
        scene_texture((s16)EXTRA_HANDLE(), texture, -1);
        scene_show(EXTRA_HANDLE(), 0, 15);
        if (LOCAL_VIEW()) scene_hide(EXTRA_HANDLE(), 1, VIEW_MASK());
        W16(GROUP(OWNER()) + 0x13Au, 1);
        WF(GROUP(OWNER()) + 0x144u, 0.0f);
        scene_show(GROUP_HANDLE(0), 0, 15);
        if (LOCAL_VIEW()) scene_hide(GROUP_HANDLE(0), 1, VIEW_MASK());
        for (i = 1; i < 5; ++i) {
            value = random_float(1.25f);
            matrix_roll(value - 0.5f, GROUP_MATRIX(i));
            value = random_float(0.25f);
            address = GROUP_MATRIX(i);
            matrix_scale(address, address, value + 0.4f);
            scene_show(GROUP_HANDLE(i), 0, 15);
            if (LOCAL_VIEW()) scene_hide(GROUP_HANDLE(i), 1, VIEW_MASK());
        }
    }
    address = GROUP(OWNER());
    matrix_scale(address + 0x108u, address + 0x108u, RF(address + 0x13Cu));
}
