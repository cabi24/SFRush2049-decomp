/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A menu-renderer group, superseding dot_runtime_a_menu_renderers_20261011.
 * New claim: func_803A7918, 576 bytes. Preserve both existing members,
 * func_803A6F2C and func_803A7620, without claiming their bytes again.
 * Complete real func_803A7180 remains unclaimed private-ABI context.
 * Reconstructed from native targets; no proven arcade-source ancestry.
 * 7918 renders the title, dispatches three menu renderers and displays
 * either of two explanatory messages. Its callback argument is unused.
 *
 * No executable source change from the accepted group's genuine context.
 * The new evidence authenticates the existing asset and full image A, then
 * proves all 144 code words plus the complete 20-byte owned switch table
 * with the unchanged canonical verifier. Without those image bytes the
 * default scorer still reports two own-section references unverified.
 *
 * INHERITED SHAPING: 6F2C has two unused s32 locals: frame312 +8 =320.
 * 7620 has four unused s32 locals: frame128 +16 =144; buffer homes move
 * upward12 to native80/120. 7918 names the real text-X result in an s16
 * carrier and has three unused s32 locals: named area516 +12 =528,
 * rounded frame600 ->608. These explain the measured frame residuals;
 * they do not recover original variable names, counts, types or purpose.
 * No fake signatures, dummy calls, forced register, padding arrays,
 * artificial read/write or diagnostic compiler alteration is used.
 *
 * 7180 still differs only in frame192 versus168. All its native buffer
 * homes agree, and this work does not shrink either live buffer.
 * All inherited live capacities are unchanged: 6F2C title128/name128/
 * suffix16; 7180 text100/suffix16; 7620 status20/title40; 7918 two256-byte
 * buffers. These are reconstruction hypotheses, not authenticated bounds
 * or guarantees of string safety. The real kept root is essential IPA
 * context for the private-register helpers; do not install them alone.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
/* Accessed field offsets are known; the view extents are not string-capacity
 * or termination guarantees. Buffer hypotheses are frozen for this research. */
typedef struct Record { u8 prefix[18]; char name[35]; char suffix[15]; u32 blocks; } Record;
typedef struct TextIndices { u8 prefix[48]; u16 choice; u8 unknown32[44]; u16 file_action; } TextIndices;
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
extern s16 func_800B3F50(void);
extern s16 func_800B7170(const char *,s16);
extern s16 func_800B71A0(const char *,s16);
extern void func_800B71D4(s16,s16,const char *);
extern void func_800B65B4(float);
extern void func_800BE078(s16,s16,s16,s16,s32,s32,const char *);
extern s16 D_803B9BBC,D_803B9BC6,D_803B9BC4,D_803B9E38,D_803B9E3A,D_803B9BBA;
extern u8 D_803BA7EB;
extern char D_803B88DC[],D_803B88E0[],D_803B88E4[],D_803B88E8[],D_803B88EC[],D_803B88F0[];
extern char D_803B88D4[],D_803B889C[],D_803B88A4[],D_803B88AC[],D_803B88B4[],D_803B88BC[],D_803B88C4[],D_803B88CC[];
extern void func_800B4360(char *,const char *,...);
extern s32 func_80004990(char *,const char *,...);
extern Record **func_80398BF0(s32);
typedef struct Slot { s8 present; u8 unknown[15]; } Slot;
typedef struct Owner { u8 prefix[16]; u8 slot; } Owner;
typedef struct OpenRecord OpenRecord;
struct OpenRecord { OpenRecord **next; void *previous; Owner **owner; };
typedef struct SavedRecord SavedRecord;
struct SavedRecord { SavedRecord **next; Owner **owner; };
extern Slot D_80156CF0[];
extern OpenRecord **D_8012E6E0;
extern SavedRecord **D_80152028;
extern s8 func_800A1A3C(s32),func_800A35F8(s32),func_80094FC4(s32);
extern s32 func_800A3518(s32);
extern u32 func_800A35BC(s32);

void func_803A7180(void)
{
    s32 top=45;
    s32 y;
    s32 row;
    s32 index;
    Record **record;
    s16 x;
    s16 row_y;
    char text[100];
    char suffix[16];
    func_800B42F0(11);
    func_800B74A0(1);
    if (D_803B9BC0) {
        func_800BE6A4(text,D_8017A4E0.choices[D_8017A4E0.indices->file_action]);
        if (D_8017A4E0.choices[D_8017A4E0.indices->file_action+1]) {
            func_800BE4F0(text,D_803B88DC);
            func_800BE4F0(text,D_8017A4E0.choices[D_8017A4E0.indices->file_action+1]);
        }
        x=func_800B71A0(text,160);
        func_800B71D4(x,top,text);
        top+=10;
    }
    func_800B71D4(35,65,D_8017A4E0.labels[84]);
    x=func_800B71A0(D_8017A4E0.labels[87],255);
    func_800B71D4(x,65,D_8017A4E0.labels[87]);
    y=80;
    func_800B42F0(14);
    for (row=0;row<D_803B9E3A;row++,y+=12) {
        index=D_803B9E38+row;
        if (index>=D_803B9BC4) index-=D_803B9BC4;
        record=func_80398BF0(index);
        func_800B74A0(index==D_803B9BC6 ? 22 : 1);
        func_800B4360(text,D_803B88E0,index+1);
        x=func_800B71A0(text,23);
        row_y=y;
        func_800B71D4(x,row_y,text);
        if (record) {
            func_800BE6A4(text,(*record)->name);
            func_800BE6A4(suffix,(*record)->suffix);
            if (suffix[0]) {
                func_800BE4F0(text,D_803B88E4);
                func_800BE4F0(text,suffix);
            }
        } else func_800BE6A4(text,D_8017A4E0.labels[149]);
        func_800B71D4(35,row_y,text);
        func_800B4360(suffix,D_803B88E8,record ? (*record)->blocks : 0);
        x=func_800B71A0(suffix,255);
        func_800B71D4(x,row_y,suffix);
        if (index==D_803B9BC6 && record) {
            func_800B74A0(1);
            x=func_800B71A0(text,160);
            func_800B71D4(x,top,text);
            top+=10;
        }
    }
    func_800B42F0(11);
    func_800B74A0(1);
    if (D_803B9BC0 && D_8017A4E0.labels[83]) {
        x=func_800B71A0(D_8017A4E0.labels[83],160);
        func_800B71D4(x,top,D_8017A4E0.labels[83]);
    }
    func_800B4360(text,D_803B88EC,D_803B9BBC);
    func_800BE4F0(text,D_803B9BBC==1 ? D_8017A4E0.labels[85] : D_8017A4E0.labels[86]);
    func_800BE4F0(text,D_803B9BBC==1 ? D_8017A4E0.labels[91] : D_8017A4E0.labels[92]);
    x=func_800B71A0(text,160);
    func_800B71D4(x,182,text);
    func_80004990(text,D_803B88F0,D_8017A4E0.labels[88],D_8017A4E0.labels[90],D_803BA7EB,D_8017A4E0.labels[86],D_8017A4E0.labels[155]);
    x=func_800B71A0(text,160);
    func_800B71D4(x,192,text);
}

void func_803A7620(void)
{
    s32 residual_slot0;
    char status[20];
    char title[40];
    s32 residual_slot1, residual_slot2, residual_slot3;
    s32 player;
    s32 y=64;
    s16 x;
    OpenRecord **open;
    SavedRecord **saved;
    for(player=0;player<4;player++,y+=36) {
        func_800B42F0(player==D_803B9BBA ? 10 : 11);
        func_800B74A0(player==D_803B9BBA ? 22 : 1);
        func_800B4360(title,D_803B88D4,D_8017A4E0.labels[123],player+1);
        x=func_800B71A0(title,160);
        func_800B71D4(x,y-(player==D_803B9BBA ? 1 : 0),title);
        if (!D_80156CF0[player].present) func_800B4360(status,D_803B889C,D_8017A4E0.labels[165]);
        else if (!func_800A1A3C(player)) func_800B4360(status,D_803B88A4,D_8017A4E0.labels[168]);
        else if (func_800A35F8(player)) func_800B4360(status,D_803B88AC,D_8017A4E0.labels[166]);
        else if (func_80094FC4(player)) func_800B4360(status,D_803B88B4,D_8017A4E0.labels[169]);
        else {
            for (open=D_8012E6E0;open;open=(*open)->next) {
                if ((*open)->owner && (*(*open)->owner)->slot==player) break;
            }
            for (saved=D_80152028;saved;saved=(*saved)->next) {
                if ((*saved)->owner && (*(*saved)->owner)->slot==player) break;
            }
            if (open || saved) func_800B4360(status,D_803B88BC,D_8017A4E0.labels[159]);
            else if (func_800A3518(player) && func_800A35BC(player)>=D_803BA7EB)
                func_800B4360(status,D_803B88C4,D_8017A4E0.labels[158]);
            else func_800B4360(status,D_803B88CC,D_8017A4E0.labels[161]);
        }
        x=func_800B71A0(status,160);
        func_800B71D4(x,y-(player==D_803B9BBA ? 1 : 0)+12,status);
    }
}


void func_803A6F2C(void)
{
    s32 residual_slot0, residual_slot1;
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
    s32 residual_slot0;
    s16 x;
    char unavailable[256];
    s32 residual_slot1, residual_slot2;
    char failed[256];
    func_800B65B4(0.0f);
    func_800B42F0(13);
    func_800B74A0(12);
    x = func_800B71A0(D_8017A4E0.labels[157],160);
    func_800B71D4(x,10,D_8017A4E0.labels[157]);
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
