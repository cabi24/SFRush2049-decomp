/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { f32 x,y,z; } Vec3;
typedef struct Node112 Node112;
typedef struct { u8 prefix[64]; u32 flags; } Resource68;
typedef struct {
    u8 prefix[16]; u32 flags; s16 count,action;
    u32 field24; Resource68 *resources; u32 field32;
} Scene36;
struct Node112 {
    Node112 *next; u8 flags; u8 gap5[3]; void *key;
    s16 field12,resource,metadata; u16 field18;
    Vec3 position; u8 gap32[36]; Vec3 normal;
    u8 gap80[8]; s16 texture; u8 gap90[11];
    s8 index; u8 gap102[6]; u8 *state;
};
typedef struct {
    const char *name; u32 field4; void (*callback)(Node112 *);
    void *animation; s16 kind; u16 flags; u8 gap20[2];
    s8 category,element; f32 value; u8 tail[20];
} Metadata48;
typedef struct { s32 object[3]; u8 gap12[122]; s16 active; } Slot136;
typedef struct { void *record; f32 matrix[9]; Vec3 position,velocity; } Matrix64;
typedef struct Animation24 {
    struct Animation24 *next; s16 state; u16 field6; u32 field8;
    Node112 *node; f32 time; void *data;
} Animation24;
typedef struct { u8 prefix[16]; Node112 *head; } SceneManager20;
extern u32 D_801174B4;
extern s8 D_80156994;
extern s32 D_801392D4;
extern Scene36 *D_801392D0;
extern s32 D_8014A110;
extern s8 D_80152570,D_80150DD0;
extern s8 D_80150E28[];
extern u8 D_80117510[];
extern u8 *D_80150E68[],*D_80150E98[],*D_80118DDC[];
extern s32 D_80150F78,D_80150F80;
extern Matrix64 *D_80150F38;
extern s32 D_801497EC;
extern void **D_801497C0;
extern SceneManager20 D_80143FC8;
extern Metadata48 D_80117530[];
extern u16 D_801427C0[];
extern Resource68 D_8012E700[];
extern Animation24 *D_801391F0;
extern f32 D_8011418C[];
extern u8 D_80118C10[];
extern Vec3 D_80118DFC;
extern void *audio_dma_sync(void *,u32);
extern void func_800B2CB4(s16);
extern void listener_position_set(s16);
s32 transmission_ratio_get(Scene36 *,s16,Vec3 *,void *,s32,s32,s32);
extern void *func_8008E3C0(void *);
extern void math_utility(f32 *,f32 *);
extern Animation24 *func_80090284(void);
extern void func_800AB750(s8,Vec3 *,Vec3 *,Vec3 *);
/* The original direct overlay call supplies this single signed-byte input.
   Its implementation and overlay semantics are intentionally unclaimed. */
extern void func_8039133C(s32);
