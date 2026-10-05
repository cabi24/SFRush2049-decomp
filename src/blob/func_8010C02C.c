/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Collision callback: select a model using a signed 16-bit index and a
 * dead-reckoned collision record using the object's signed byte at +0x65.
 * After a bounding-sphere test, transform up to four model corners into the
 * other record's frame. The first inclusive box hit invokes random_float,
 * the historical name of the accepted corner-collision-force routine.
 * Every path returns zero. The two unused incoming slots are retained because
 * the native entry homes both a2 and a3; their concrete callback types remain
 * unknown. The force callee's five-argument ABI is independently accepted.
 *
 * Matching source-form details: the world point has named components and an
 * array view of the same three floats. Named component writes reproduce the
 * native reloads between translation and relative-position subtraction;
 * a plain float array incorrectly commoned three of those loads. The union
 * keeps array access within a real three-element object. The named radius
 * and radius_squared values are both used. Declaration order places mnum
 * before the vectors. No unused locals, stack buffers, or helper keepers.
 *
 * Arcade collision.c is a semantic relative, not an exact donor: this callback
 * handles one selected pair and only its first four corners, with N64 box
 * ordering x-min/x-max/z-max/z-min/y-max/y-min and a model stride of 0x808.
 * Read-only evidence and reproducible checks are in this directory.
 */
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef float f32;
typedef union {
    struct { f32 x, y, z; } c;
    f32 v[3];
} Vector3;
typedef struct { char unknown00[0x65]; s8 index; } CollisionObject;
typedef struct {
    char unknown00[0xF4];
    f32 corners[4][3];
    char unknown124[0x654-0x124];
    f32 radius;
    char unknown658[0x794-0x658];
    f32 position[3];
    f32 basis[3][3];
    char unknown7C4[0x7EA-0x7C4];
    s8 collidable;
    char unknown7EB[0x808-0x7EB];
} CollisionModel;
typedef struct {
    f32 xmin, xmax, zmax, zmin, ymax, ymin, radius;
    f32 mass;
} CollisionBounds;
typedef struct {
    CollisionBounds *body;
    f32 basis[3][3];
    f32 position[3];
    f32 velocity[3];
} CollisionReckon;
extern CollisionModel D_8014A250[];
extern CollisionReckon *D_80150F38;
void func_8009E820(f32 *, f32 *, f32 *);
void func_800A61B0(f32 *, f32 *, f32 *);
void random_float(s32, CollisionModel *, CollisionReckon *, f32 *, f32 *);

s32 func_8010C02C(CollisionObject *object, s16 *index, void *arg2, s32 arg3) {
    CollisionModel *car;
    CollisionReckon *other;
    s32 mnum;
    f32 radius;
    f32 radius_squared;
    f32 delta[3];
    Vector3 world;
    f32 local[3], distance;
    s32 i, hit;
    mnum = *index;
    car = &D_8014A250[mnum];
    other = &D_80150F38[object->index];
    if (!car->collidable) {
        return 0;
    }
    delta[0] = car->position[0] - other->position[0];
    delta[1] = car->position[1] - other->position[1];
    delta[2] = car->position[2] - other->position[2];
    for (i = 0, distance = 0.0; i < 3; i++) {
        distance += delta[i] * delta[i];
    }
    radius = other->body->radius + car->radius;
    radius_squared = radius * radius;
    if (distance > radius_squared) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        func_8009E820(car->corners[i], world.v, car->basis[0]);
        world.c.x = car->position[0] + world.c.x;
        world.c.y = car->position[1] + world.c.y;
        world.c.z = car->position[2] + world.c.z;
        world.c.x -= other->position[0];
        world.c.y -= other->position[1];
        world.c.z -= other->position[2];
        func_800A61B0(world.v, local, other->basis[0]);
        if (local[0] > other->body->xmax || local[0] < other->body->xmin) {
            hit = 0;
        } else if (local[1] > other->body->ymax || local[1] < other->body->ymin) {
            hit = 0;
        } else if (local[2] > other->body->zmax || local[2] < other->body->zmin) {
            hit = 0;
        } else {
            hit = 1;
        }
        if (hit) {
            random_float((s32)car, car, other, delta, local);
            return 0;
        }
    }
    return 0;
}
