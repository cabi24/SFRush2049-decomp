/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Game-over results screens, results-table family (group frontier_results_table, w27c/w28a/w29a).
 * func_80102F30 (kept): results dispatcher (GC gameoverscr.c draw dispatch) + race standings table
 *   (GC drawRaceScore: rows = laps+3, cols = players+1, str1[40] = value, c[4] = color);
 * func_8010221C = GC drawMain (menu items 0..6 via validOption = func_800F84B0, then the per-mode table);
 * func_80101904 / func_80101D84 (stunt/battle standings tables, GC drawBattle/StuntStandings family);
 * func_80100D5C = GC drawRect(x, y, w, h, color) (IPA registers t1,t3,t2,t4,a2).
 * func_80102448 = per-player stunt records table (13 rows; F30 calls it for gameplay_mode 4).
 * func_80100E58, func_80102980 stay asm (unmatched drafts: w29a/<fn>/best.c).
 * Helpers: rt_gfx_lock/rt_gfx_unlock (the D_801461D0 font lock), rt_font_set (lock; slot_state_setup;
 *   unlock, 24 B inline area) and rt_font_set_b (lock helper + osSendMesg written out, 16 B inline area).
 *   Non-static so umerge leaves no deleted-procedure stubs; internal (not on keep). HELPER IDENTITY
 *   HYPOTHESIS (w29a): F30's six font-11 sites use the rt_font_set_b shape and its three font-3 sites the
 *   rt_font_set shape; this is what gives retail's 640-byte frame and every stack offset (all nine
 *   inlined sites are code-identical, only the inline-area size differs).
 * DEVICES (disclosed):
 *  - func_80101904/func_80101D84: written-out font sites with `if (previous) {}` holders; unused locals as
 *    exact frame residual `other` (1904, D84) and block `s32 i` (D84); colour-init on four lines (w27c).
 *  - func_80102F30: two unused locals `unused1`, `unused2` = exact frame residual (retail frame 640;
 *    without them 632, with one 632 and 89 words off; 2 slots at the top of the frame, between `cell`
 *    and `y`). `cell = 60` is a real named local (it makes column*60 a propagated expression, which
 *    numbers the x web after the left/colour-table webs as in retail).
 *  - func_8010221C: none (w29a: `if (!func_800F84B0(item)) continue;` replaced w28a's do {} while (0)).
 *  - func_80102448: written-out pre-loop font site with `if (previous) {}` holder (w27c); VARIABLE REUSE:
 *    `left = 60` before the loops and `x = column*left` in the fill loop (left is reassigned in the header
 *    block): makes column*60 a propagated expression so x is numbered after `160 - width/2` as in retail
 *    (same cause as F30's `cell`; 2448's frame is exact without a new local).
 * Unit: blob_unit score F30 221C 1904 D84 D5C --with this file --internal <all but F30, rt_*> --neighbours
 *   -> 5/5 EQUAL, 0 locked bodies differ. See cloud/work/frontier/w29a/RESULTS.md.
 */
/* Shared source model for native results callbacks. External record gaps are
 * observed offsets/strides, not compiler-pressure stack storage. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct { u32 w0; u32 w1; } Gwords;
typedef union { Gwords words; long long int force_structure_alignment; } Gfx;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define G_RDPPIPESYNC 0xE7
#define G_SETPRIMDEPTH 0xEE
#define G_SETPRIMCOLOR 0xFA
#define G_SETCOMBINE 0xFC
#define G_FILLRECT 0xF6

#define gDPNoParam(pkt, cmd) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = _SHIFTL(cmd, 24, 8); _g->words.w1 = 0; }
#define gDPPipeSync(pkt) gDPNoParam(pkt, G_RDPPIPESYNC)
#define gDPSetPrimDepth(pkt, z, dz) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = _SHIFTL(G_SETPRIMDEPTH, 24, 8); \
    _g->words.w1 = _SHIFTL(z, 16, 16) | _SHIFTL(dz, 0, 16); }
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) { Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
    _g->words.w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); }
#define gDPSetCombineLERP_PRIM(pkt) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = 0xFCFFFFFF; _g->words.w1 = 0xFFFDF6FB; }
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) { Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL((lrx), 14, 10) | _SHIFTL((lry), 2, 10)); \
    _g->words.w1 = (_SHIFTL((ulx), 14, 10) | _SHIFTL((uly), 2, 10)); }

extern Gfx *D_80149438;
extern s32 D_8014A248;

void func_80100D5C(s32 x, s32 y, s32 w, s32 h, u8 *color)
{
    gDPPipeSync(D_80149438++);
    gDPSetPrimDepth(D_80149438++, 0x10, 0);
    gDPSetPrimColor(D_80149438++, 0, 0, color[0], color[1], color[2], color[3]);
    gDPSetCombineLERP_PRIM(D_80149438++);
    gDPFillRectangle(D_80149438++, x, y, x + w, y + h);
    D_8014A248 = -1;
}

typedef struct RGBA {u8 r,g,b,a;} RGBA;
typedef struct PlayerRow {u8 vehicle;u8 other[71];char **name;} PlayerRow;
typedef struct VehicleScore {s8 score;u8 other[951];} VehicleScore;
typedef struct PlayerStats {s32 words[18];s16 laps[24];} PlayerStats;
typedef struct TextIndices {u8 other[74];u16 records;u8 other2[14];u16 menu;} TextIndices;
typedef struct TextState {void *word0;char **fixed;void *word8;TextIndices *indices;char **strings;} TextState;
extern s8 D_80152744,D_80143F54[],D_8012E67C[];
extern s8 D_80149428[][4];
extern s16 active_player_count;
extern PlayerRow input_rec0[];
extern VehicleScore D_80152BBB[];
extern PlayerStats D_80152038[];
extern s32 D_801146AC[],D_80118E20[2],D_801391E0,gameplay_mode;
extern u8 D_80114724[];
extern char D_8012039C[],D_801203A0[],D_80120390[],D_80120394[],D_80120398[],D_80120354[],D_8012035C[],D_80120344[],D_8012034C[],D_80120350[];
typedef struct CarB6 {s8 lap;u8 rest[951];} CarB6;
typedef struct CarB7 {s8 flag;u8 rest[951];} CarB7;
typedef struct CarF8 {f32 best;u8 rest[948];} CarF8;
typedef struct Rec808 {s16 car;u8 rest[2054];} Rec808;
typedef struct TextIdx2 {u8 other[108];u16 cars;} TextIdx2;
extern CarB6 D_80152906[];
extern CarB7 D_80152907[];
extern CarF8 D_80152908[];
extern Rec808 D_8014AA16[];
extern s16 D_801543CA,D_80152734;
extern u8 D_801543D4,D_80144018[];
extern f32 D_80144DA8[];
extern char D_8012031C[],D_80120324[],D_80120334[];
extern OSMesgQueue D_801461D0;
extern TextState countdown_state;
extern s32 D_8014A248;
extern void render_helper(f32);
extern void dispatch_handler(s32);
extern void race_position_update(char *,char *,s32);
extern void state_utility(s16,s16,char *);
extern s32 fcvt_wrapper(char *,char *,...);
extern s32 osRecvMesg(OSMesgQueue *,void **,s32),osSendMesg(OSMesgQueue *,void *,s32);
extern s32 slot_state_setup(s32);
void func_80100D5C(s32 x, s32 y, s32 w, s32 h, u8 *color);
extern void func_80101904(void),func_80101D84(void),func_80100E58(void),drawBattleScore(void);
void func_80102448(void);
extern s32 func_800F84B0(s32),object_bytes_sum_global(void);
void rt_gfx_lock(void) {osRecvMesg(&D_801461D0,0,1);}
void rt_gfx_unlock(void) {osSendMesg(&D_801461D0,0,0);}
s32 rt_font_set(s32 font) {
    s32 previous;
    rt_gfx_lock();previous=slot_state_setup(font);rt_gfx_unlock();
    return previous;
}
void func_80101904(void)
{
    s16 x;
    s32 previous;
    s16 y;
    char *text;
    char value[40];
    s32 rows,columns;
    s32 row,column,total,other;
    RGBA color;
    char label[212];
    rows=D_80152744+1;
    columns=3;
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                if(column>0 && row>0) {
                    color.r=0;color.g=0;color.b=0;color.a=192;
                    switch(D_8012E67C[D_80143F54[row-1]]) {
                    case 0:color.b=128;break;
                    case 1:color.r=128;break;
                    case 2:color.r=128;color.g=128;break;
                    case 3:color.g=128;break;
                    }
                } else {
                    color.r=0;
                    color.g=0;
                    color.b=0;
                    color.a=192;
                }
                func_80100D5C(column*60+70,row*14+36,60,14,(u8 *)&color);
            }
        }
    }
    render_helper(0.0f);
    D_80118E20[0]=1;D_80118E20[1]=1;
    osRecvMesg(&D_801461D0,0,1); previous = slot_state_setup(11); osSendMesg(&D_801461D0,0,0); if (previous) {}
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                if(column==0) {
                    x=column*60+100;
                    y=row*14+43;
                    osRecvMesg(&D_801461D0,0,1); previous = slot_state_setup(11); osSendMesg(&D_801461D0,0,0); if (previous) {}
                    if(D_80143F54[row-1]<active_player_count) {
                        dispatch_handler(D_801146AC[D_8012E67C[D_80143F54[row-1]]]);
                        text=input_rec0[D_80143F54[row-1]].name[0]+20;
                    } else {
                        text=D_80120354;
                        dispatch_handler(4);
                    }
                    race_position_update(label,text,60);
                    state_utility(x,y,label);
                } else if(row==0) {
                    if(column==1) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[130];
                    } else if(column==2) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[12];
                    }
                } else {
                    s32 i;
                    total=0;
                    text=value;
                    if(column==1) {
                        total=D_80152BBB[input_rec0[D_80143F54[row-1]].vehicle].score;
                    } else if(column==2) {
                        for(i=0;i<active_player_count;i++) {
                            total+=(D_80149428[i][D_80143F54[row-1]]>=0 ? D_80149428[i][D_80143F54[row-1]] : -D_80149428[i][D_80143F54[row-1]]);
                        }
                    }
                    dispatch_handler(1);
                    fcvt_wrapper(value,D_8012035C,total);
                }
                if(column!=0) state_utility((s16)(column*60+100),(s16)(row*14+43),text);
            }
        }
    }
    render_helper(-1.0f);
    D_80118E20[1]=3;D_80118E20[0]=0;
}

void func_80101D84(void)
{
    s16 x;
    s32 previous;
    s16 y;
    char *text;
    char value[40];
    s32 rows,columns;
    s32 row,column,total,other;
    RGBA color;
    char label[204];
    rows=D_80152744+1;
    columns=3;
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                if(column>0 && row>0) {
                    color.r=0;color.g=0;color.b=0;color.a=192;
                    switch(D_80143F54[row-1]) {
                    case 0:color.b=128;break;
                    case 1:color.r=128;break;
                    case 2:color.r=128;color.g=128;break;
                    case 3:color.g=128;break;
                    }
                } else {
                    color.r=0;
                    color.g=0;
                    color.b=0;
                    color.a=192;
                }
                func_80100D5C(column*60+70,row*14+36,60,14,(u8 *)&color);
            }
        }
    }
    render_helper(0.0f);
    D_80118E20[0]=1;D_80118E20[1]=1;
    osRecvMesg(&D_801461D0,0,1); previous = slot_state_setup(11); osSendMesg(&D_801461D0,0,0); if (previous) {}
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                if(column==0) {
                    s32 i;
                    x=column*60+100;
                    y=row*14+43;
                    rt_font_set(11);
                    if(D_80143F54[row-1]<active_player_count) {
                        dispatch_handler(D_801146AC[D_80143F54[row-1]]);
                        text=input_rec0[D_80143F54[row-1]].name[0]+20;
                    } else {
                        text=D_80120344;
                        dispatch_handler(4);
                    }
                    race_position_update(label,text,60);
                    state_utility(x,y,label);
                } else if(row==0) {
                    rt_font_set(12);
                    if(column==1) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[130];
                    } else if(column==2) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[213];
                    }
                } else if(column==1) {
                    dispatch_handler(1);
                    text=value;
                    fcvt_wrapper(value,D_8012034C,D_80152038[D_80143F54[row-1]].words[5]);
                } else if(column==2) {
                    dispatch_handler(1);
                    text=value;
                    fcvt_wrapper(value,D_80120350,D_80152038[D_80143F54[row-1]].words[2]);
                }
                if(column!=0) state_utility((s16)(column*60+100),(s16)(row*14+43),text);
                osRecvMesg(&D_801461D0,0,1); previous = slot_state_setup(11); osSendMesg(&D_801461D0,0,0); if (previous) {}
            }
        }
    }
    render_helper(-1.0f);
    D_80118E20[1]=3;D_80118E20[0]=0;
}




void func_80102448(void)
{
    s32 x;
    s32 previous;
    s32 y;
    char *text;
    char value[40];
    s32 rows,columns;
    s32 row,column,left,width;
    RGBA color;
    char label[180];
    rows=13;
    columns=active_player_count+1;
    left=60;
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                width=columns*60;
                x=column*left;
                y=row*14+19;
                if(row>=3) {
                    if(row==3 && column==0) {
                        color.r=0;color.g=0;color.b=0;color.a=192;
                        func_80100D5C(160-width/2+x,y,width,7,(u8 *)&color);
                    }
                    y+=7;
                }
                if(column>0 && row>0) {
                    color.r=0;color.g=0;color.b=0;color.a=192;
                    switch(column-1) {
                    case 0:color.b=128;break;
                    case 1:color.r=128;break;
                    case 2:color.r=128;color.g=128;break;
                    case 3:color.g=128;break;
                    }
                } else {
                    color.r=0;
                    color.g=0;
                    color.b=0;
                    color.a=192;
                }
                func_80100D5C(160-width/2+x,y,60,14,(u8 *)&color);
            }
        }
    }
    render_helper(0.0f);
    D_80118E20[0]=1;D_80118E20[1]=1;
    osRecvMesg(&D_801461D0,0,1); previous = slot_state_setup(11); osSendMesg(&D_801461D0,0,0); if (previous) {}
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                y=row*14+19;
                if(row>=3) y+=7;
                if(row==0) {
                    s32 i;
                    dispatch_handler(D_801146AC[column-1]);
                    text=input_rec0[column-1].name[0]+20;
                    left=160-columns*60/2;
                    x=column*60;
                    race_position_update(label,text,60);
                    state_utility(left+x+30,y+7,label);
                } else if(column==0) {
                    rt_font_set(12);
                    if(row==1) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[130];
                    } else if(row==2) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[213];
                    } else {
                        dispatch_handler(1);
                        fcvt_wrapper(value,countdown_state.strings[countdown_state.indices->records+row-3]);
                        text=value;
                    }
                } else if(row==1) {
                                        dispatch_handler(1);
                    fcvt_wrapper(value,D_80120390,D_80152038[column-1].words[5]);
                    text=value;
                } else if(row==2) {
                                        dispatch_handler(1);
                    fcvt_wrapper(value,D_80120394,D_80152038[column-1].words[2]);
                    text=value;
                } else {
                                        dispatch_handler(1);
                    fcvt_wrapper(value,D_80120398,D_80152038[column-1].laps[row-3]);
                    text=value;
                }
                if(row!=0) state_utility(column*60-columns*60/2+190,y+7,text);
                rt_font_set(11);
            }
        }
    }
    render_helper(-1.0f);
    D_80118E20[1]=3;D_80118E20[0]=0;
}

/* The only native direct caller of both callbacks. */
void drawMain(void)
{
    char *text;
    s32 item,color;
    s16 x;
    s16 y;
    render_helper(0.0f);
    D_80118E20[0]=1;D_80118E20[1]=3;
    rt_font_set(13);
    dispatch_handler(1);
    x=D_80114724[0];y=D_80114724[1];
    text=countdown_state.fixed[63];
    state_utility(x,y,text);
    y=118;
    rt_font_set(10);
    for(item=0;item<7;item++) {
        if(!func_800F84B0(item)) continue;
        {
            text=countdown_state.strings[countdown_state.indices->menu+item];
            if(item==0 && gameplay_mode!=4 && gameplay_mode!=6)
                text=countdown_state.fixed[0];
            if(item==D_801391E0) color=22; else color=1;
            dispatch_handler(color);
            state_utility(x,y,text);
            y+=object_bytes_sum_global();
        }
    }
    D_80118E20[1]=3;D_80118E20[0]=0;
    render_helper(-1.0f);
    switch(gameplay_mode) {
    case 4:func_80101D84();break;
    case 6:func_80101904();break;
    case 0:case 2:case 3:case 5:func_80100E58();break;
    }
}


extern s32 D_80138870;
extern f32 D_80144DA4[];
extern f32 D_80149A78[][8];
extern char D_80120360[],D_80120370[],D_80120380[];
void drawBattleScore(void)
{
    s32 x;
    s32 previous;
    s32 y;
    char *text;
    char value[40];
    s32 rows,columns;
    s32 row,column,total,other;
    RGBA color;
    char name[188];
    char label[144];
    rows=active_player_count+2;
    columns=active_player_count+1;
    other=60;
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                y=row*16-rows*16/2+110;
                x=column*other;
                if(row>=2) {
                    if(row==2 && column==0) {
                        color.r=0;color.g=0;color.b=0;color.a=192;
                        func_80100D5C(160-columns*other/2+x,y,columns*60,8,(u8 *)&color);
                    }
                    y+=8;
                }
                if(column==0 || row==0) {
                    color.r=0;
                    color.g=0;
                    color.b=0;
                    color.a=192;
                } else if(column+1==row) {
                    color.r=0;
                    color.g=0;
                    color.b=0;
                    color.a=192;
                    switch(D_8012E67C[column-1]) {
                    case 0:color.b=64;break;
                    case 1:color.r=64;break;
                    case 2:color.r=64;color.g=64;break;
                    case 3:color.g=64;break;
                    }
                } else {
                    color.r=0;
                    color.g=0;
                    color.b=0;
                    color.a=192;
                    switch(D_8012E67C[column-1]) {
                    case 0:color.b=128;break;
                    case 1:color.r=128;break;
                    case 2:color.r=128;color.g=128;break;
                    case 3:color.g=128;break;
                    }
                }
                func_80100D5C(160-columns*other/2+x,y,60,16,(u8 *)&color);
            }
        }
    }
    render_helper(0.0f);
    D_80118E20[0]=1;D_80118E20[1]=1;
    rt_gfx_lock(); previous = slot_state_setup(11); osSendMesg(&D_801461D0,0,0); if (previous) {}
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                y=row*16-rows*16/2+110;
                if(row>=2) y+=8;
                if(row==0) {
                    dispatch_handler(D_801146AC[D_8012E67C[column-1]]);
                    text=input_rec0[column-1].name[0]+20;
                    total=160-columns*other/2;
                    x=column*other;
                    race_position_update(name,text,60);
                    state_utility(total+x+30,y+8,name);
                } else if(column==0) {
                    if(row==1) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[130];
                    } else {
                        dispatch_handler(D_801146AC[D_8012E67C[row-2]]);
                        text=input_rec0[row-2].name[0]+20;
                        total=160-columns*other/2;
                        x=column*other;
                        race_position_update(label,text,60);
                        state_utility(total+x+30,y+8,label);
                    }
                } else {
                    if(row==1) {
                        total=D_80152BBB[input_rec0[column-1].vehicle].score;
                        dispatch_handler(1);
                        fcvt_wrapper(value,D_8012039C,total);
                        text=value;
                    } else {
                        if(column+1==row) dispatch_handler(4);
                        else dispatch_handler(1);
                        fcvt_wrapper(value,D_801203A0,D_80149428[column-1][row-2]);
                        text=value;
                    }
                }
                if(row!=0 && (column!=0 || row==1)) state_utility(column*other-columns*other/2+190,y+8,text);
            }
        }
    }
    render_helper(-1.0f);
    D_80118E20[1]=3;D_80118E20[0]=0;
}
s32 rt_font_set_b(s32 font) {
    s32 previous;
    rt_gfx_lock();previous=slot_state_setup(font);osSendMesg(&D_801461D0,0,0);
    return previous;
}
s32 func_80102F30(s32 arg0)
{
    s32 x;
    s32 cell;
    s32 unused1;
    s32 unused2;
    s32 y;
    char *text;
    char value[40];
    s32 rows,columns;
    s32 row,column,left,width;
    RGBA color;
    char label[252];
    f32 t;
    switch(D_80138870) {
    case 1:
        drawMain();
        break;
    case 2:
        switch(gameplay_mode) {
        case 4:
            func_80102448();
            break;
        case 6:
            drawBattleScore();
            break;
        case 0:case 2:case 3:case 5:
            rows=D_80152734+3;
            columns=active_player_count+1;
            cell=60;
            for(row=0;row<rows;row++) {
                for(column=0;column<columns;column++) {
                    if(row!=0 || column!=0) {
                        width=columns*60;
                        x=column*cell;
                        y=row*16-rows*16/2+110;
                        if(row>=3) {
                            if(row==3 && column==0) {
                                color.r=0;color.g=0;color.b=0;color.a=192;
                                func_80100D5C(160-width/2+x,y,width,8,(u8 *)&color);
                            }
                            y+=8;
                        }
                        if(column>0 && row>0) {
                            color.r=0;color.g=0;color.b=0;color.a=192;
                            switch(column-1) {
                            case 0:color.b=128;break;
                            case 1:color.r=128;break;
                            case 2:color.r=128;color.g=128;break;
                            case 3:color.g=128;break;
                            }
                        } else {
                            color.r=0;
                            color.g=0;
                            color.b=0;
                            color.a=192;
                        }
                        func_80100D5C(160-width/2+x,y,60,16,(u8 *)&color);
                    }
                }
            }
            render_helper(0.0f);
            D_80118E20[0]=1;D_80118E20[1]=1;
            rt_font_set_b(11);
            for(row=0;row<rows;row++) {
                for(column=0;column<columns;column++) {
                    if(row==0 && column==0) continue;
                    {
                        y=row*16-rows*16/2+110;
                        if(row>=3) y+=8;
                        if(row==0) {
                            rt_font_set_b(11);
                            dispatch_handler(D_801146AC[column-1]);
                            text=input_rec0[column-1].name[0]+20;
                            left=160-columns*60/2;
                            x=column*cell;
                            race_position_update(label,text,60);
                            state_utility(left+x+30,y+8,label);
                        } else if(column==0) {
                            rt_font_set_b(11);
                            if(row==1) {
                                dispatch_handler(1);
                                text=countdown_state.fixed[208];
                            } else if(row==2) {
                                dispatch_handler(1);
                                text=countdown_state.fixed[210];
                            } else {
                                                                dispatch_handler(1);
                                fcvt_wrapper(value,countdown_state.fixed[205],row-2);
                                text=value;
                            }
                        } else if(row==1) {
                            if(D_80144018[input_rec0[column-1].vehicle]==D_80152734) {
                                t=D_80152908[input_rec0[column-1].vehicle].best;
                                rt_font_set(3);
                                dispatch_handler(1);
                                fcvt_wrapper(value,D_80120360,(u32)(t/60.0f),(u32)t%60,(u32)(t*1000.0f)%1000);
                                text=value;
                            } else {
                                rt_font_set_b(11);
                                dispatch_handler(1);
                                text=countdown_state.fixed[215];
                            }
                        } else if(row==2) {
                            if(D_80144018[input_rec0[column-1].vehicle]>0) {
                                t=D_80144DA4[column];
                                rt_font_set(3);
                                dispatch_handler(1);
                                fcvt_wrapper(value,D_80120370,(u32)(t/60.0f),(u32)t%60,(u32)(t*1000.0f)%1000);
                                text=value;
                            } else {
                                rt_font_set_b(11);
                                dispatch_handler(1);
                                text=countdown_state.fixed[215];
                            }
                        } else {
                            if(row-3<D_80144018[input_rec0[column-1].vehicle]) {
                                t=D_80149A78[column-1][row-3];
                                rt_font_set(3);
                                dispatch_handler(1);
                                fcvt_wrapper(value,D_80120380,(u32)(t/60.0f),(u32)t%60,(u32)(t*1000.0f)%1000);
                                text=value;
                            } else {
                                rt_font_set_b(11);
                                dispatch_handler(1);
                                text=countdown_state.fixed[215];
                            }
                        }
                        if(row!=0) state_utility(column*60-columns*60/2+190,y+8,text);
                    }
                }
            }
            render_helper(-1.0f);
            D_80118E20[1]=3;D_80118E20[0]=0;
            break;
        }
        break;
    }
    return 1;
}

