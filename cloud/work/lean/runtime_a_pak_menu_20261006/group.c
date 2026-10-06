/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Language { s32 unknown0; u8 **text; s32 unknown8; u16 *indices; u8 **indexed; } Language;
typedef struct FileData {
    void **next;
    u8 unknown04[12];
    u8 controller, id;
    u8 name[35], extension[15];
    u32 size;
} FileData;
typedef struct Controller { s8 status; u8 unknown01[15]; } Controller;
typedef struct EntryA { struct EntryA **next; s32 unknown04; FileData **file; } EntryA;
typedef struct EntryB { struct EntryB **next; FileData **file; } EntryB;
extern Language countdown_state;
extern s8 D_803B46B0,D_803B9BB8;
extern s32 D_803B9BB0;
extern s16 D_803B9BBA,D_803B9BBC,D_803B9BC4,D_803B9BC6,D_803B9E38,D_803B9E3A;
extern void **D_803B9BC0;
extern u8 D_803BA7EB;
extern Controller D_80156CF0[];
extern EntryA **D_8012E6E0;
extern EntryB **D_80152028;
extern u8 D_803B8900[],D_803B8904[],D_803B8908[],D_803B88DC[],D_803B88E4[];
extern char D_803B88D4[],D_803B889C[],D_803B88A4[],D_803B88AC[],D_803B88B4[],D_803B88BC[],D_803B88C4[],D_803B88CC[];
extern char D_803B88E0[],D_803B88E8[],D_803B88EC[],D_803B88F0[];
extern void render_helper(f32);
extern s32 object_create(s32);
extern void dispatch_handler(s32);
extern void func_800B669C(u32,u32);
extern s16 sound_pitch_diff_halved(void *,s16);
extern s16 sound_pitch_diff_calc(void *,s16);
extern s16 object_bytes_sum_global(void);
extern void state_utility(s16,s16,void *);
extern void fcvt_wrapper(char *,char *,...);
extern u8 *func_800BE6A4(u8 *,u8 *);
extern u8 *func_800BE4F0(u8 *,u8 *);
extern void camera_auto_follow(s16,s16,s16,s16,s16,s16,u8 *);
extern s8 func_800A1A3C(s32);
extern s8 func_800A35F8(s32);
extern s8 func_80094FC4(s32);
extern s32 func_800A3518(s32);
extern u32 func_800A35BC(s32);
extern void **func_80398BF0(s32);
extern void sprintf(char *,char *,...);

void func_803A6F2C(void)
{
    u8 title[128], filename[128], extension[16];
    s32 height, y;
    title[0]=0;
    func_800BE6A4(filename,((FileData *)*D_803B9BC0)->name);
    func_800BE6A4(extension,((FileData *)*D_803B9BC0)->extension);
    if(extension[0]) {
        func_800BE4F0(filename,D_803B8900);
        func_800BE4F0(filename,extension);
    }
    func_800BE6A4(title,countdown_state.text[141]);
    func_800BE4F0(filename,countdown_state.text[142]);
    object_create(11);
    dispatch_handler(1);
    height=object_bytes_sum_global();
    func_800B669C(1,1);
    y=130;
    state_utility(160,y-height*3,title);
    object_create(14);
    state_utility(160,y-height*2,filename);
    func_800B669C(0,3);
    object_create(D_803B9BB8 ? 11 : 10);
    dispatch_handler(D_803B9BB8 ? 1 : 22);
    state_utility(sound_pitch_diff_calc(countdown_state.indexed[countdown_state.indices[24]+1],145),
                  y-(D_803B9BB8 ? 0 : 1),countdown_state.indexed[countdown_state.indices[24]+1]);
    object_create(D_803B9BB8 ? 10 : 11);
    dispatch_handler(D_803B9BB8 ? 22 : 1);
    state_utility(175,y-(D_803B9BB8 ? 1 : 0),countdown_state.indexed[countdown_state.indices[24]]);
}
void func_803A7180(void)
{
    u8 text[96], small[16];
    s32 title_y=45, row_y, i, id;
    void **node;
    object_create(11);
    dispatch_handler(1);
    if(D_803B9BC0) {
        func_800BE6A4(text,countdown_state.indexed[countdown_state.indices[47]]);
        if(countdown_state.indexed[countdown_state.indices[47]+1]) {
            func_800BE4F0(text,D_803B88DC);
            func_800BE4F0(text,countdown_state.indexed[countdown_state.indices[47]+1]);
        }
        state_utility(sound_pitch_diff_halved(text,160),title_y,text);
        title_y=55;
    }
    state_utility(35,65,countdown_state.text[84]);
    state_utility(sound_pitch_diff_halved(countdown_state.text[87],255),65,countdown_state.text[87]);
    row_y=80;
    object_create(14);
    for(i=0;i<D_803B9E3A;i++) {
        id=D_803B9E38+i;
        if(id>=D_803B9BC4)id-=D_803B9BC4;
        node=func_80398BF0(id);
        dispatch_handler(id==D_803B9BC6 ? 22 : 1);
        fcvt_wrapper((char *)text,D_803B88E0,id+1);
        state_utility(sound_pitch_diff_halved(text,23),row_y,text);
        if(node) {
            func_800BE6A4(text,((FileData *)*node)->name);
            func_800BE6A4(small,((FileData *)*node)->extension);
            if(small[0]) {
                func_800BE4F0(text,D_803B88E4);
                func_800BE4F0(text,small);
            }
        } else func_800BE6A4(text,countdown_state.text[149]);
        state_utility(35,row_y,text);
        fcvt_wrapper((char *)small,D_803B88E8,node ? ((FileData *)*node)->size : 0);
        state_utility(sound_pitch_diff_halved(small,255),row_y,small);
        if(id==D_803B9BC6 && node) {
            dispatch_handler(1);
            state_utility(sound_pitch_diff_halved(text,160),title_y,text);
            title_y+=10;
        }
        row_y+=12;
    }
    object_create(11);
    dispatch_handler(1);
    if(D_803B9BC0 && countdown_state.text[83])
        state_utility(sound_pitch_diff_halved(countdown_state.text[83],160),title_y,countdown_state.text[83]);
    fcvt_wrapper((char *)text,D_803B88EC,D_803B9BBC);
    func_800BE4F0(text,D_803B9BBC==1 ? countdown_state.text[85] : countdown_state.text[86]);
    func_800BE4F0(text,D_803B9BBC==1 ? countdown_state.text[91] : countdown_state.text[92]);
    state_utility(sound_pitch_diff_halved(text,160),182,text);
    sprintf((char *)text,D_803B88F0,countdown_state.text[88],countdown_state.text[90],
                 D_803BA7EB,countdown_state.text[86],countdown_state.text[155]);
    state_utility(sound_pitch_diff_halved(text,160),192,text);
}
void func_803A7620(void)
{
    u8 status[24], header[40];
    s32 i,y=64;
    EntryA **a;
    EntryB **b;
    for(i=0;i<4;i++) {
        object_create(i==D_803B9BBA ? 10 : 11);
        dispatch_handler(i==D_803B9BBA ? 22 : 1);
        fcvt_wrapper((char *)header,D_803B88D4,countdown_state.text[123],i+1);
        state_utility(sound_pitch_diff_halved(header,160),y-(i==D_803B9BBA ? 1 : 0),header);
        if(!D_80156CF0[i].status)fcvt_wrapper((char *)status,D_803B889C,countdown_state.text[165]);
        else if(!func_800A1A3C(i))fcvt_wrapper((char *)status,D_803B88A4,countdown_state.text[168]);
        else if(func_800A35F8(i))fcvt_wrapper((char *)status,D_803B88AC,countdown_state.text[166]);
        else if(func_80094FC4(i))fcvt_wrapper((char *)status,D_803B88B4,countdown_state.text[169]);
        else {
            a=D_8012E6E0;
            while(a) {
                if((*a)->file && (*(*a)->file)->controller==i)break;
                a=(*a)->next;
            }
            b=D_80152028;
            while(b) {
                if((*b)->file && (*(*b)->file)->controller==i)break;
                b=(*b)->next;
            }
            if(a || b)fcvt_wrapper((char *)status,D_803B88BC,countdown_state.text[159]);
            else if(func_800A3518(i) && func_800A35BC(i)>=D_803BA7EB)
                fcvt_wrapper((char *)status,D_803B88C4,countdown_state.text[158]);
            else fcvt_wrapper((char *)status,D_803B88CC,countdown_state.text[161]);
        }
        state_utility(sound_pitch_diff_halved(status,160),y-(i==D_803B9BBA ? 1 : 0)+12,status);
        y+=36;
    }
}
void func_803A6F2C(void);
void func_803A7180(void);
void func_803A7620(void);
s32 func_803A7918(void *callback_context)
{
    render_helper(0.0f);
    object_create(13);
    dispatch_handler(12);
    state_utility(sound_pitch_diff_halved(countdown_state.text[157],160),10,countdown_state.text[157]);
    if(D_803B46B0) {
        switch(D_803B9BB0) {
        case 0: func_803A7620(); break;
        case 1: func_803A7180(); break;
        case 2: func_803A6F2C(); break;
        case 3: {
            u8 text[256];
            object_create(11);
            dispatch_handler(1);
            func_800BE6A4(text,countdown_state.text[106]);
            func_800BE4F0(text,D_803B8904);
            func_800BE4F0(text,countdown_state.text[102]);
            func_800B669C(1,1);
            camera_auto_follow(160,130,300,160,-1,0,text);
            func_800B669C(0,3);
            break;
        }
        case 4: {
            u8 text[256];
            object_create(11);
            dispatch_handler(1);
            func_800BE6A4(text,countdown_state.text[167]);
            func_800BE4F0(text,D_803B8908);
            func_800BE4F0(text,countdown_state.text[102]);
            func_800B669C(1,1);
            camera_auto_follow(160,130,300,160,-1,0,text);
            func_800B669C(0,3);
            break;
        }
        }
    }
    render_helper(-1.0f);
    return 1;
}
