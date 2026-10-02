/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Direction24 {u16 unknown,flags;s16 axis[3];u8 tail[14];} Direction24;
typedef struct Vehicle2056 {
 u8 prefix[100];Vec3 wheel_impulse[4];u8 gap148[144];Vec3 impulse;
 u8 gap304[240];Vec3 normal,velocity;u8 gap568[180];float basis[9];
 u8 gap784[656];u16 contact[4];u8 gap1448[24];float factor;
 u8 gap1476[40];float wheel_contact[4];u8 gap1532[56];float dt;
 u8 gap1592[16];Direction24 *direction;u8 tail[444];
} Vehicle2056;
extern Direction24 *D_801497F8;
extern float D_801243C4,D_801243C8;
extern void func_800A61B0(Vec3 *,Vec3 *,float *);
extern void func_800C1A00(int,Vec3 *);
extern void func_8008E0B8(Vec3 *);
void func_800E1F80(Vehicle2056 *vehicle)
{
    Direction24 *direction=vehicle->direction;
    Vec3 delta,axis;
    float dot,force;
    int strength=(direction->flags&0xF800)>>11;
    int contacts=0,mask=0,i;
    if(direction->flags&0x10) {
        strength*=8;
        if(strength>=51) {
            axis.x=direction->axis[0]*0.00006103515625f;
            axis.y=direction->axis[1]*0.00006103515625f;
            axis.z=direction->axis[2]*0.00006103515625f;
            dot=axis.z*vehicle->normal.z+(axis.x*vehicle->normal.x+axis.y*vehicle->normal.y);
            force=(strength*D_801243C4-dot)*vehicle->factor;
            delta.x=axis.x*force;
            delta.y=axis.y*force;
            delta.z=axis.z*force;
            func_800A61B0(&delta,&axis,vehicle->basis);
            vehicle->impulse.x+=axis.x;
            vehicle->impulse.y+=axis.y;
            vehicle->impulse.z+=axis.z;
            return;
        }
    }
    for(i=0;i<4;i++) {
        if(vehicle->wheel_contact[i]<=0.0f && (D_801497F8[vehicle->contact[i]].flags&0x30)) {
            contacts++;
            mask|=1<<i;
        }
    }
    if(contacts>=3) {
        if(direction->flags&0x20) {
            func_800C1A00(strength,&axis);
            vehicle->velocity.x+=axis.x*vehicle->dt;
            vehicle->velocity.z+=axis.z*vehicle->dt;
        } else {
            force=(strength*D_801243C8)*vehicle->dt;
            vehicle->velocity.x+=(force*direction->axis[0])*0.00006103515625f;
            vehicle->velocity.z+=(force*direction->axis[2])*0.00006103515625f;
        }
        return;
    }
    for(i=0;i<4;i++) {
        if(mask&(1<<i)) {
            if(direction->flags&0x20) {
                func_800C1A00(strength,&axis);
                func_8008E0B8(&axis);
            } else {
                axis.x=direction->axis[0]*0.00006103515625f;
                axis.y=direction->axis[1]*0.00006103515625f;
                axis.z=direction->axis[2]*0.00006103515625f;
            }
            axis.x*=30.0f*vehicle->factor;
            axis.y*=30.0f*vehicle->factor;
            axis.z*=30.0f*vehicle->factor;
            func_800A61B0(&axis,&delta,vehicle->basis);
            vehicle->wheel_impulse[i].x+=delta.x;
            vehicle->wheel_impulse[i].y+=delta.y;
            vehicle->wheel_impulse[i].z+=delta.z;
        }
    }
}
