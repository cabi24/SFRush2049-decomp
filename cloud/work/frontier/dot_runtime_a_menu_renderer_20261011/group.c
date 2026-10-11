/* Research only: both complete functions remain NONMATCH.
 * Genuine 7918 menu callback supplies the private-register renderer context.
 * Scalar/text buffer capacities follow this reconstruction, not a recovered
 * original declaration. No buffer expansion or unused padding is permitted
 * to close the residual. See README.md for extent, call and ownership limits.
 */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Record { u8 prefix[18]; char name[35]; char suffix[16]; } Record;
typedef struct TextIndices { u8 prefix[48]; u16 choice; } TextIndices;
typedef struct Language { void *header; char **labels; void *unknown8; TextIndices *indices; char **choices; } Language;
extern Record **D_803B9BC0;
extern Language D_8017A4E0;
extern s8 D_803B9BB8,D_803B46B0;
extern s32 D_803B9BB0;
extern char D_803B8900[],D_803B8904[],D_803B8908[];
extern char *func_800BE6A4(char *,const char *);
extern char *func_800BE4F0(char *,const char *);
extern void func_800B42F0(s32);
extern void func_800B74A0(s32);
extern void func_800B669C(s32,s32);
extern s32 func_800B3F50(void);
extern s32 func_800B7170(const char *,s16);
extern s32 func_800B71A0(const char *,s16);
extern void func_800B71D4(s16,s16,const char *);
extern void func_800B65B4(float);
extern void func_800BE078(s16,s16,s16,s16,s32,s32,const char *);
extern void func_803A7620(void),func_803A7180(void);

void func_803A6F2C(void)
{
    s16 x;
    char title[128];
    char name[128];
    char suffix[16];
    s32 height;
    s32 y=130;
    title[0]=0;
    func_800BE6A4(name,(*D_803B9BC0)->name);
    func_800BE6A4(suffix,(*D_803B9BC0)->suffix);
    if (suffix[0]) {
        func_800BE4F0(name,D_803B8900);
        func_800BE4F0(name,suffix);
    }
    func_800BE6A4(title,D_8017A4E0.labels[141]);
    func_800BE4F0(name,D_8017A4E0.labels[142]);
    func_800B42F0(11);
    func_800B74A0(1);
    height=func_800B3F50();
    func_800B669C(1,1);
    func_800B71D4(160,y-height*3,title);
    func_800B42F0(14);
    func_800B71D4(160,y-height*2,name);
    func_800B669C(0,3);
    func_800B42F0(D_803B9BB8 ? 11 : 10);
    func_800B74A0(D_803B9BB8 ? 1 : 22);
    x=func_800B7170(D_8017A4E0.choices[D_8017A4E0.indices->choice+1],145);
    func_800B71D4(x,y-(D_803B9BB8 ? 0 : 1),D_8017A4E0.choices[D_8017A4E0.indices->choice+1]);
    func_800B42F0(D_803B9BB8 ? 10 : 11);
    func_800B74A0(D_803B9BB8 ? 22 : 1);
    func_800B71D4(175,y-(D_803B9BB8 ? 1 : 0),D_8017A4E0.choices[D_8017A4E0.indices->choice]);
}

s32 func_803A7918(void *callback)
{
    char unavailable[256];
    char failed[256];
    func_800B65B4(0.0f);
    func_800B42F0(13);
    func_800B74A0(12);
    func_800B71D4(func_800B71A0(D_8017A4E0.labels[157],160),10,D_8017A4E0.labels[157]);
    if (D_803B46B0) {
        switch (D_803B9BB0) {
        case 0: func_803A7620(); break;
        case 1: func_803A7180(); break;
        case 2: func_803A6F2C(); break;
        case 3:
            func_800B42F0(11);
            func_800B74A0(1);
            func_800BE6A4(unavailable,D_8017A4E0.labels[106]);
            func_800BE4F0(unavailable,D_803B8904);
            func_800BE4F0(unavailable,D_8017A4E0.labels[102]);
            func_800B669C(1,1);
            func_800BE078(160,130,300,160,-1,0,unavailable);
            func_800B669C(0,3);
            break;
        case 4:
            func_800B42F0(11);
            func_800B74A0(1);
            func_800BE6A4(failed,D_8017A4E0.labels[167]);
            func_800BE4F0(failed,D_803B8908);
            func_800BE4F0(failed,D_8017A4E0.labels[102]);
            func_800B669C(1,1);
            func_800BE078(160,130,300,160,-1,0,failed);
            func_800B669C(0,3);
            break;
        }
    }
    func_800B65B4(-1.0f);
    return 1;
}
