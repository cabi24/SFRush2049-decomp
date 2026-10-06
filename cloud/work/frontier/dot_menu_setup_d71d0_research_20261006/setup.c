/* Native audio-options/menu setup at 0x800D71D0. Historical label retained. */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct MultiBlit MultiBlit;
typedef struct Blit Blit;
typedef struct NameEntry NameEntry;
typedef struct MenuObject64 {
    s32 kind, handle, word8;
    f32 basis[3][3];
    f32 position[3];
    u32 word60;
} MenuObject64;
typedef struct Object68 {u8 fields[60];s32 parameter;u32 word64;} Object68;
typedef struct AudioMessage {s16 id;s8 type,used;u8 payload[20];} AudioMessage;
extern MenuObject64 D_801102B4[];
extern char *D_80110290[];
extern f32 D_8011418C[3][3];
extern Object68 D_8012E700[];
extern s32 D_80110660,D_801105B4,D_801105B8;
extern s32 state_word_a,D_8011063C,D_80110640,D_80110638;
extern s8 D_80117428,D_8011062C,D_80146108[];
extern u8 D_80140BDC;
extern char D_8012F6F0[],D_8012F6F8[],D_8012F6FC[];
extern MultiBlit D_801105C0[],D_801105E4[];
extern Blit *D_80110630;
extern OSMesgQueue D_80142728,D_801427A8;
extern void drone_collision_avoid(void),particle_lifetime_set(void);
extern void *func_80007c68(void *,const void *,u32);
extern s32 string_copy_format(char *,s8,s8,s8);
extern s32 func_8008E26C(s32,void *,s16,s32);
extern NameEntry *func_800B24EC(char *,s16 *,s8,s8,s32);
extern void func_8008D870(s16,s32,s32);
extern void func_800B5898(f32,f32 [][3]);
extern void func_8008B32C(f32 [3][3],f32 [3][3],f32);
extern void particle_velocity_set(void),entity_audio_update(void);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern s32 osRecvMesg(OSMesgQueue *,void **,s32),osJamMesg(OSMesgQueue *,void *,s32);
extern AudioMessage *func_80091B00(void);
extern void speed_set(f32,f32,s32,s32),sync_entry_register(s32,s32),func_800D6160(s32);
void drone_pathfind_main(void)
{
    MenuObject64 *entry;
    s32 parameter, model, flags;
    NameEntry *texture_entry;
    f32 angle, height, depth;
    s16 texture;
    AudioMessage *message;
    D_80117428=1;
    drone_collision_avoid();
    if(state_word_a & 0x7C03FFFE) {
        particle_lifetime_set();
        parameter=D_80110660;
        for(entry=D_801102B4;entry!=D_801102B4+12;entry++) {
            if(entry->handle==-1) {
                func_80007c68(entry->basis,D_8011418C,36);
                model=string_copy_format(D_80110290[entry->kind],0,(s8)(D_80140BDC-1),1);
                if(entry->kind==0 || entry->kind==1 || entry->kind==2) flags=0x42000;
                else flags=0;
                entry->handle=func_8008E26C(model,entry->basis,-1,flags);
                D_8012E700[entry->handle].parameter=parameter;
            }
        }
        texture_entry=func_800B24EC(D_8012F6F0,&texture,0,(s8)(D_80140BDC-1),1);
        func_8008D870((s16)D_801102B4[8].handle,(s32)texture_entry,-1);
        texture_entry=func_800B24EC(D_8012F6F8,&texture,0,(s8)(D_80140BDC-1),1);
        func_8008D870((s16)D_801102B4[9].handle,(s32)texture_entry,-1);
        func_80007c68(D_801102B4[9].basis,D_8011418C,36);
        angle=-1.5707964f;
        func_800B5898(angle,D_801102B4[9].basis);
        height=55.2f;
        depth=100.0f;
        D_801102B4[9].position[0]=-75.1f;
        D_801102B4[9].position[1]=height;
        D_801102B4[9].position[2]=depth;
        func_8008B32C(D_801102B4[9].basis,D_801102B4[9].basis,0.5f);
        func_80007c68(D_801102B4[10].basis,D_8011418C,36);
        func_800B5898(angle,D_801102B4[10].basis);
        D_801102B4[10].position[0]=-87.1f;
        D_801102B4[10].position[1]=height;
        D_801102B4[10].position[2]=depth;
        texture_entry=func_800B24EC(D_8012F6FC,&texture,0,(s8)(D_80140BDC-1),1);
        func_8008D870((s16)D_801102B4[11].handle,(s32)texture_entry,-1);
        func_80007c68(D_801102B4[11].basis,D_8011418C,36);
        func_800B5898(angle,D_801102B4[11].basis);
        D_801102B4[11].position[0]=0.0f;
        D_801102B4[11].position[1]=115.0f;
        D_801102B4[11].position[2]=200.0f;
        D_801105B4=1;
        D_801105B8=1;
        particle_velocity_set();
        D_80110630=sound_control(0,0,D_801105C0,1);
    } else D_80110630=sound_control(0,0,D_801105E4,2);
    entity_audio_update();
    D_8011063C=0;
    D_80110640=D_80146108[14];
    if(D_80110638==1 || D_80110638==2) {
        osRecvMesg(&D_80142728,0,1);
        message=func_80091B00();
        message->type=1;
        osJamMesg(&D_80142728,0,0);
        osJamMesg(&D_801427A8,message,0);
        speed_set((f32)D_80146108[12]/10.0f,0.05f,1,0);
        if(D_80110640<12) sync_entry_register(D_80110640,1);
        else if(D_80110640==12) {
            if(state_word_a & 0x7C03FFFE) sync_entry_register(6,1);
            else func_800D6160(0);
        }
    }
    D_8011062C=1;
}
