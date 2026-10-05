/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef float f32;
typedef struct Selection8 {u8 reserved;u8 model;s8 type;s8 tune;s8 variant;u8 tail[3];} Selection8;
typedef struct Vehicle2056 {u8 opaque[1996];s8 mode;u8 tail[59];} Vehicle2056;
typedef struct Archive404 {u8 prefix[352];u32 bytes[13];} Archive404;
extern Selection8 D_80153E88[13];
extern Vehicle2056 D_8014A250[6];
extern Archive404 D_8002E580;
extern s32 D_8011735C;
extern u32 D_801174B4;
extern s16 D_8014A108;
extern s16 D_80142B08[13];
extern s8 D_80111604[],D_80111648[],D_8011168C[];
extern u32 audio_output_setup(void *);
extern s32 func_80097694(s32,s32);
extern s32 audio_frame_sync();
extern void func_800BB02C(s32,s32,s32);
extern void func_8039156C();
#define NEXT_RANDOM() ((D_8011735C=(D_8011735C*1103515245+12345))>>16&32767)
void audio_interrupt_handler(s32 force)
{
    Selection8 *selected,*all;
    Archive404 *archive;
    s16 *output;
    s32 i,scan,loaded,choice;
    u32 available;
    f32 range;
    if(force) {
        func_8039156C(force);
        return;
    }
    all=D_80153E88;
    selected=all;
    archive=&D_8002E580;
    output=D_80142B08;
    loaded=0;
    for(i=0;i<13;i++,selected++,output++) {
        if(i<6 && ((D_801174B4&8) || D_8014A250[i].mode==1)) {
            do {
                selected->model=(u32)((f32)NEXT_RANDOM()*13.0f/32768.0f);
                for(scan=0;scan<i;scan++)
                    if(selected->model==all[scan].model)break;
            }while(scan!=i);
            selected->type=D_80111604[selected->model];
            selected->tune=D_80111648[selected->model];
            selected->variant=D_8011168C[selected->model];
            available=audio_output_setup(0);
            scan=0;
            if(available<archive->bytes[selected->model]+512) {
                do {
                    if(func_80097694(selected->model+88,-1)<0) {
                        available=audio_output_setup(0);
                        if(available>=archive->bytes[selected->model]+512)goto human_load;
                    }
                    scan++;
                    selected->model=(selected->model+1)%13;
                }while(scan<13);
                scan=0;
                do {
                    available=audio_output_setup(0);
                    scan++;
                    if(available>=archive->bytes[selected->model]+512)goto human_load;
                    selected->model=(selected->model+1)%13;
                }while(scan!=13);
                range=(f32)loaded;
                do {
                    choice=(s32)((f32)NEXT_RANDOM()*range/32768.0f);
                }while(D_8014A108<loaded && all[choice].model>=13);
                selected->model=all[choice].model;
                *output=D_80142B08[choice];
                goto human_setup;
            }
human_load:
            *output=audio_frame_sync(selected->model+88,1,0,0,0);
            loaded++;
human_setup:
            func_800BB02C(i,selected->model,0);
            continue;
        }
        if(i<6 && D_8014A250[i].mode==2) {
            available=audio_output_setup(0);
            if(available<archive->bytes[selected->model]+512) {
                for(selected->model=0;selected->model<13;selected->model++) {
                    available=audio_output_setup(0);
                    if(available>=archive->bytes[selected->model]+512)goto load;
                }
                selected->model=all[0].model;
                *output=D_80142B08[0];
                func_800BB02C(i,selected->model,0);
                continue;
            }
load:
            *output=audio_frame_sync(selected->model+88,1,0,0,0);
            loaded++;
            func_800BB02C(i,selected->model,0);
        } else {
            selected->model=all[0].model;
            *output=D_80142B08[0];
        }
    }
}
