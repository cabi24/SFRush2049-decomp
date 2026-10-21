/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B object-resource selection and player alpha state update.
 * New complete helpers: B10 and D38, 552 bytes each. Both deleted-procedure
 * stubs (B00/B08, 8 bytes each) are included and claimed separately. The genuine
 * outer 90F60 caller is complete but remains NONMATCH context: five words
 * (one table home, one address-schedule pair, one spill/reload home pair).
 * No accepted/image/ROM coverage is implied by this compiler match.
 *
 * Adjacent caller-less B00/B08 identities as the alpha and phase-a blocks are
 * inferred, not recovered original names or proven whole-TU membership. Stock
 * whole-program compilation inlines both; their separate bodies prevent B10
 * being incorrectly inlined into F60. Only the real F60 root is kept, owning
 * the native s0/s1/f20 saves for both internal no-argument helpers.
 *
 * The target-width pointer round-trip on delta-time reads intentionally removes
 * IDO's direct-global addressing/no-alias spelling. It restores the observed
 * address materialization; direct scalar spelling is 131/138 in both helpers.
 * unsigned long is target32 and host-pointer-width in the fixture build.
 * The 0.f spelling in phase-a keeps its zero web distinct from the alpha loop.
 * Alpha's pointer loop preserves the observed live count reload. The guarded
 * initialization is a native lifetime boundary. No unused locals, artificial
 * volatile, fake parameters, assembly, or changed compiler is used.
 *
 * Object's overlapping scene word and signed index at +14 are target-big-endian
 * union views. Unknown fields preserve observed record storage, not stack pads.
 * Native layouts: State 20 B, Slot 8 B, Player 952 B; Object is an 84-B prefix.
 * Valid domain: backed signed count/selector indices, an eligible selection
 * slot (or the native wrap loop does not terminate), phase-3 countdown 0..7,
 * finite timer/RNG results and representable conversions. Byte alpha subtraction
 * intentionally wraps before the <=16 test. Synchronous helper mutations are
 * visible in later reloads. No whole-game, helper implementation or hardware
 * equivalence is claimed, and no whole-function arcade donor is established.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Object {
    struct Object *next;
    u8 flags;
    u8 unknown05[7];
    union { s32 scene; struct { s16 upper, index; } parts; } handle;
    u8 unknown10[64];
    s16 resource;
} Object;
typedef struct Slot { Object *object; s16 active, count; } Slot;
typedef struct State {
    s8 count, selected_delay, selected_b, selected_a;
    f32 delay, phase_a, phase_b;
    Slot *slots;
} State;
typedef struct Player {
    u8 unknown000[908];
    u32 flags;
    u8 unknown390[16];
    s8 countdown;
    u8 alpha;
    s8 phase;
    u8 unknown3A3[5];
    f32 timer;
    u8 unknown3AC[12];
} Player;
extern State D_80399AE0;
extern f32 D_8002EB94;
extern u16 D_80142A82, D_80142A84, D_80142A8E;
extern s16 D_8014A108;
extern Player D_80152818[];
extern f32 func_8008B2E4(f32);
extern void func_80090770(s16, u16);
extern void model_transform_setup(s32, s32, u32);
#define gBattlePowerUps D_80399AE0
#define gNumPlayers D_8014A108
#define gGameCar D_80152818
#define gFrameSeconds (*(volatile f32 *)&D_8002EB94)
#define Random func_8008B2E4
#define ZOID_SetObjectDef func_80090770
#define MBOX_ShowObject model_transform_setup
void func_80390B00(void) /* UpdateInvs */
{
    s32 i;
    u8 invsColor[8] = {16, 32, 64, 96, 112, 144, 176, 176};
    for (i = 0; i < gNumPlayers; i++) {
        Player *car = &gGameCar[i];
        if (car->flags & 1) {
            if (car->phase == 3) {
                car->alpha = invsColor[car->countdown];
            } else if (car->phase == 1) {
                car->countdown = 0;
                if (car->timer > 0.0f) {
                    car->alpha -= 8;
                    if (car->alpha <= 16) {
                        car->alpha = 16;
                        car->phase = 3;
                    }
                    car->timer -= gFrameSeconds;
                } else {
                    car->phase = 3;
                }
            } else {
                car->countdown = 0;
                if (car->timer > 0.0f) {
                    if (car->alpha + 8 > 255) {
                        car->alpha = 255;
                    } else {
                        car->alpha = car->alpha + 8;
                    }
                    car->timer -= gFrameSeconds;
                } else {
                    car->flags &= ~1;
                    car->phase = 0;
                }
            }
        }
        if (car->countdown > 0) {
            car->countdown--;
        }
    }
}
void func_80390B08(void) /* UpdateBattleInvs */
{
    s32 i;
    func_80390B00();
    if (gBattlePowerUps.phase_a == -2.0f) {
        if (gBattlePowerUps.slots[gBattlePowerUps.selected_a].object->flags & 2) {
            return;
        }
        gBattlePowerUps.phase_a = 90.0f + Random(60.0f);
        for (i = 0; i < gBattlePowerUps.count; i++) {
            if (gBattlePowerUps.slots[i].active == 2) {
                gBattlePowerUps.slots[i].active = 1;
            }
        }
        gBattlePowerUps.slots[gBattlePowerUps.selected_a].active = 2;
        gBattlePowerUps.slots[gBattlePowerUps.selected_a].count = 1;
    } else if ((gBattlePowerUps.phase_a -= gFrameSeconds) < 0.f) {
        i = Random(gBattlePowerUps.count);
        while (gBattlePowerUps.slots[i].count != 1 || gBattlePowerUps.slots[i].active == 2) {
            if (++i >= gBattlePowerUps.count) {
                i = 0;
            }
        }
        gBattlePowerUps.selected_a = i;
        gBattlePowerUps.slots[i].active = 1;
        gBattlePowerUps.slots[i].count = 2;
        gBattlePowerUps.slots[i].object->flags |= 2;
        gBattlePowerUps.slots[i].object->resource = 354;
        ZOID_SetObjectDef(gBattlePowerUps.slots[i].object->handle.parts.index, D_80142A84);
        MBOX_ShowObject(gBattlePowerUps.slots[i].object->handle.scene, 0, 15);
        gBattlePowerUps.phase_a = -2.0f;
    }
}
void func_80390B10(void)
{
    s32 i;
    if (D_80399AE0.phase_b == -2.0f) {
        if (!(D_80399AE0.slots[D_80399AE0.selected_b].object->flags & 2)) {
            D_80399AE0.phase_b = func_8008B2E4(60.0f) + 90.0f;
            for (i = 0; i < D_80399AE0.count; i++) {
                if (D_80399AE0.slots[i].active == 3) {
                    D_80399AE0.slots[i].active = 1;
                }
            }
            D_80399AE0.slots[D_80399AE0.selected_b].active = 3;
            D_80399AE0.slots[D_80399AE0.selected_b].count = 1;
        }
    } else {
        D_80399AE0.phase_b -= *(f32 *)(unsigned long)&D_8002EB94;
        if (D_80399AE0.phase_b < 0.0f) {
            i = func_8008B2E4(D_80399AE0.count);
            while (D_80399AE0.slots[i].count != 1 || D_80399AE0.slots[i].active == 3) {
                if (++i >= D_80399AE0.count) i = 0;
            }
            D_80399AE0.selected_b = i;
            D_80399AE0.slots[i].active = 1;
            D_80399AE0.slots[i].count = 3;
            D_80399AE0.slots[i].object->flags |= 2;
            D_80399AE0.slots[i].object->resource = 359;
            func_80090770(D_80399AE0.slots[i].object->handle.parts.index, D_80142A8E);
            model_transform_setup(D_80399AE0.slots[i].object->handle.scene, 0, 15);
            D_80399AE0.phase_b = -2.0f;
        }
    }
}
void func_80390D38(void)
{
    s32 i;
    if (D_80399AE0.delay == -2.0f) {
        if (!(D_80399AE0.slots[D_80399AE0.selected_delay].object->flags & 2)) {
            D_80399AE0.delay = func_8008B2E4(10.0f) + 5.0f;
            for (i = 0; i < D_80399AE0.count; i++) {
                if (D_80399AE0.slots[i].active == 4) {
                    D_80399AE0.slots[i].active = 1;
                }
            }
            D_80399AE0.slots[D_80399AE0.selected_delay].active = 4;
            D_80399AE0.slots[D_80399AE0.selected_delay].count = 1;
        }
    } else {
        D_80399AE0.delay -= *(f32 *)(unsigned long)&D_8002EB94;
        if (D_80399AE0.delay < 0.0f) {
            i = func_8008B2E4(D_80399AE0.count);
            while (D_80399AE0.slots[i].count != 1 || D_80399AE0.slots[i].active == 4) {
                if (++i >= D_80399AE0.count) i = 0;
            }
            D_80399AE0.selected_delay = i;
            D_80399AE0.slots[i].active = 1;
            D_80399AE0.slots[i].count = 4;
            D_80399AE0.slots[i].object->flags |= 2;
            D_80399AE0.slots[i].object->resource = 353;
            func_80090770(D_80399AE0.slots[i].object->handle.parts.index, D_80142A82);
            model_transform_setup(D_80399AE0.slots[i].object->handle.scene, 0, 15);
            D_80399AE0.delay = -2.0f;
        }
    }
}
void func_80390F60(void) /* UpdateBattlePowerUps */
{
    func_80390D38();
    func_80390B10();
    func_80390B08();
}
