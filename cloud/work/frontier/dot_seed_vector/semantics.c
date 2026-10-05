/* Host-side contract checks. The three callees are observational test doubles. */
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "func_8010C02C.c"

CollisionModel D_8014A250[3];
static CollisionReckon records[256];
CollisionReckon *D_80150F38 = records + 128;
static CollisionBounds bounds;
static CollisionObject object;
static int transform_count, inverse_count, force_count, cases;
static int force_first;
static CollisionModel *force_car;
static CollisionReckon *force_other;
static f32 force_delta[3], force_local[3];

void func_8009E820(f32 *in, f32 *out, f32 *basis) {
    int i;
    transform_count++;
    for (i = 0; i < 3; i++) {
        out[i] = (in[0] * basis[i] + in[1] * basis[3 + i]) + in[2] * basis[6 + i];
    }
}

void func_800A61B0(f32 *in, f32 *out, f32 *basis) {
    int i;
    inverse_count++;
    for (i = 0; i < 3; i++) {
        out[i] = (in[0] * basis[3 * i] + in[1] * basis[3 * i + 1]) + in[2] * basis[3 * i + 2];
    }
}

void random_float(s32 first, CollisionModel *car, CollisionReckon *other,
                  f32 *delta, f32 *local) {
    force_count++;
    force_first = first;
    force_car = car;
    force_other = other;
    memcpy(force_delta, delta, sizeof(force_delta));
    memcpy(force_local, local, sizeof(force_local));
}

static void identity(f32 basis[3][3]) {
    int i;
    memset(basis, 0, 9 * sizeof(f32));
    for (i = 0; i < 3; i++) basis[i][i] = 1;
}

static void reset(int record_index) {
    CollisionModel *car = &D_8014A250[1];
    CollisionReckon *other;
    memset(D_8014A250, 0, sizeof(D_8014A250));
    memset(records, 0, sizeof(records));
    memset(&object, 0, sizeof(object));
    object.index = (s8)record_index;
    other = D_80150F38 + record_index;
    bounds.xmin = bounds.ymin = bounds.zmin = -1;
    bounds.xmax = bounds.ymax = bounds.zmax = 1;
    bounds.radius = car->radius = 10;
    other->body = &bounds;
    car->collidable = 1;
    identity(car->basis);
    identity(other->basis);
    transform_count = inverse_count = force_count = 0;
    force_car = 0;
    force_other = 0;
}

/* Independent expectation: sphere guard, then each fully transformed corner.
 * Closed-box rejection deliberately preserves the native unordered comparisons.
 */
static void check(void) {
    CollisionModel *car = &D_8014A250[1];
    CollisionReckon *other = D_80150F38 + object.index;
    f32 delta[3], local[3], world[3], sum, radius;
    int corner, axis, expected_calls = 0, expected_force = 0;
    s16 index = 1;
    sum = 0;
    for (axis = 0; axis < 3; axis++) {
        delta[axis] = car->position[axis] - other->position[axis];
        sum += delta[axis] * delta[axis];
    }
    radius = car->radius + bounds.radius;
    if (car->collidable && !(sum > radius * radius)) {
        for (corner = 0; corner < 4; corner++) {
            expected_calls++;
            for (axis = 0; axis < 3; axis++) {
                world[axis] = (car->corners[corner][0] * car->basis[0][axis]
                              + car->corners[corner][1] * car->basis[1][axis])
                              + car->corners[corner][2] * car->basis[2][axis];
                world[axis] = car->position[axis] + world[axis];
                world[axis] -= other->position[axis];
            }
            for (axis = 0; axis < 3; axis++) {
                local[axis] = (world[0] * other->basis[axis][0]
                             + world[1] * other->basis[axis][1])
                             + world[2] * other->basis[axis][2];
            }
            if (local[0] > bounds.xmax || local[0] < bounds.xmin ||
                local[1] > bounds.ymax || local[1] < bounds.ymin ||
                local[2] > bounds.zmax || local[2] < bounds.zmin) continue;
            expected_force = 1;
            break;
        }
    }
    assert(func_8010C02C(&object, &index, (void *)(uintptr_t)0x1234, -777) == 0);
    assert(transform_count == expected_calls);
    assert(inverse_count == expected_calls);
    assert(force_count == expected_force);
    if (expected_force) {
        assert(force_first == (s32)(uintptr_t)car);
        assert(force_car == car && force_other == other);
        for (axis = 0; axis < 3; axis++) {
            assert(force_delta[axis] == delta[axis] || (isnan(force_delta[axis]) && isnan(delta[axis])));
            assert(force_local[axis] == local[axis] || (isnan(force_local[axis]) && isnan(local[axis])));
        }
    }
    cases++;
}

static unsigned rng = 0x273401;
static f32 value(void) {
    rng = rng * 1664525u + 1013904223u;
    return ((int)((rng >> 16) % 65) - 32) * 0.125f;
}

int main(void) {
    int i, j, k, side;
    static const int indices[] = {-128, -1, 0, 127};
    assert(sizeof(Vector3) == 12);
    assert(sizeof(CollisionModel) == 0x808);
    assert(offsetof(CollisionModel, corners) == 0xF4);
    assert(offsetof(CollisionModel, radius) == 0x654);
    assert(offsetof(CollisionModel, position) == 0x794);
    assert(offsetof(CollisionModel, basis) == 0x7A0);
    assert(offsetof(CollisionModel, collidable) == 0x7EA);
    assert(sizeof(CollisionBounds) == 0x20);
    for (i = 0; i < 4; i++) {
        reset(indices[i]); check();
        reset(indices[i]); D_8014A250[1].collidable = 0; check();
        reset(indices[i]); D_8014A250[1].collidable = -1; check();
    }
    /* The first hit stops the scan; each previous corner is rejected. */
    for (i = 0; i < 4; i++) {
        reset(0);
        for (j = 0; j < i; j++) D_8014A250[1].corners[j][0] = 4;
        check(); assert(force_count == 1 && transform_count == i + 1);
    }
    /* All six closed faces, then just outside each face. */
    for (i = 0; i < 3; i++) for (side = -1; side <= 1; side += 2) {
        reset(0);
        for (j = 0; j < 4; j++) D_8014A250[1].corners[j][i] = (f32)side;
        check(); assert(force_count == 1);
        reset(0);
        for (j = 0; j < 4; j++) D_8014A250[1].corners[j][i] = side * 1.125f;
        check(); assert(force_count == 0 && transform_count == 4);
    }
    reset(0); D_8014A250[1].position[0] = 20; check(); assert(transform_count == 4);
    reset(0); D_8014A250[1].position[0] = 20.125f; check(); assert(transform_count == 0);
    reset(0); D_8014A250[1].position[0] = NAN; check(); assert(force_count == 1);
    reset(0); bounds.radius = NAN; check(); assert(force_count == 1);
    reset(0); bounds.xmin = bounds.xmax = NAN; check(); assert(force_count == 1);
    /* Translation and nonidentity bases, over two independent signed slots. */
    for (i = 0; i < 1000; i++) {
        CollisionModel *car;
        CollisionReckon *other;
        reset((i & 1) ? -1 : 127);
        car = &D_8014A250[1]; other = D_80150F38 + object.index;
        for (j = 0; j < 3; j++) {
            car->position[j] = value(); other->position[j] = value();
            for (k = 0; k < 4; k++) car->corners[k][j] = value();
        }
        if (i & 2) {
            car->basis[0][0] = car->basis[1][1] = 0;
            car->basis[0][1] = 1; car->basis[1][0] = -1;
        }
        if (i & 4) {
            other->basis[0][0] = other->basis[2][2] = 0;
            other->basis[0][2] = -1; other->basis[2][0] = 1;
        }
        car->radius = (i & 8) ? 1 : 10; bounds.radius = car->radius;
        check();
    }
    printf("%d collision contract cases passed\n", cases);
    return 0;
}
