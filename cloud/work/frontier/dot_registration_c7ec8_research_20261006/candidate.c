/* Native record registration reconstruction. Historical label retained. */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef void **Handle;
typedef void (*Callback)(void);
#define F(p,t,o) (*(t *)((u8 *)(p)+(o)))
#define NULL ((void *)0)
extern u8 D_8012E6D8[], D_8013C128[], D_8013E5D8[];
extern s8 D_80146108[], D_80117428;
extern s16 D_8013FEC8;
extern void drone_set_catchup(Handle,s32,s32);
extern s32 draw_speedometer(Handle,s32);
extern s32 reverb_setup(Handle,u8);
extern void func_80007c68(void *,void *,u32);
extern void func_800A510C(void), func_80001f2c(s32,s32);
extern s32 func_8008AD04(void *,void *);
extern void func_80091FBC(void *,Handle,Handle);
extern void draw_text(Handle), object_render_cleanup(Handle);
extern void audio_volume_pan(void),func_800949D4(void);
Handle func_800C7EC8(Handle slot)
{
    Handle cursor, created;
    u8 *record, *name, *list;
    u8 port, index;
    s32 i, value;
    s8 *option;
    list=D_8012E6D8;
    cursor=F(list,Handle,8);
    while(cursor) {
        if(F(*cursor,Handle,8)==slot) return NULL;
        cursor=F(*cursor,Handle,0);
    }
    drone_set_catchup(slot,0,F(*slot,s32,64));
    if(!F(*slot,Handle,72)) return NULL;
    port=F(*slot,u8,16);
    index=F(*slot,u8,17);
    created=(Handle)(D_8013C128+port*64+index*4);
    record=D_8013E5D8+port*768+index*48;
    *created=record;
    F(record,Handle,8)=slot;
    F(*slot,Callback,8)=audio_volume_pan;
    F(*F(record,Handle,8),Callback,12)=func_800949D4;
    F(record,Handle,44)=F(*F(record,Handle,8),Handle,72);
    if(draw_speedometer(created,0) && !draw_speedometer(created,1)) return NULL;
    name=record+20;
    func_80007c68(name,(u8 *)*F(record,Handle,44)+13,13);
    func_80007c68(record+12,(u8 *)*F(record,Handle,44)+4,8);
    func_80007c68(record+33,(u8 *)*F(record,Handle,44)+60,11);
    if(!D_80117428 && !F(list,Handle,8)) {
        option=D_80146108;
        i=0;
        do {
            *option++=reverb_setup(created,(u8)i);
            i++;
        } while(i!=21);
        D_80146108[17]=1;
        value=reverb_setup(created,16);
        D_80146108[16]=value;
        D_8013FEC8=value;
        func_800A510C();
        func_80001f2c(D_80146108[19],D_80146108[20]);
    }
    cursor=F(list,Handle,8);
    while(cursor) {
        if(func_8008AD04((u8 *)*cursor+20,name)>0) break;
        cursor=F(*cursor,Handle,0);
    }
    func_80091FBC(list,created,cursor);
    draw_text(created);
    object_render_cleanup(created);
    return created;
}
