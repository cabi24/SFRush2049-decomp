/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef signed int s32;
typedef struct { f32 x,y,z; } Vec3;
typedef struct { unsigned char other[12]; Vec3 a,b,c,d; } Object;
extern unsigned char D_80142728[];
extern s32 osRecvMesg(void *,void *,s32);
extern s32 osJamMesg(void *,void *,s32);
extern void func_800D52CC();
void main_menu_input(Object *object,Vec3 *a,Vec3 *b,Vec3 *c,Vec3 *d)
{
    if(object!=(Object *)-1) {
        osRecvMesg(D_80142728,0,1);
        func_800D52CC(object);
        object->a.x=a->x; object->a.y=a->y; object->a.z=a->z;
        object->b.x=b->x; object->b.y=b->y; object->b.z=b->z;
        object->c.x=c->x; object->c.y=c->y; object->c.z=c->z;
        object->d.x=d->x; object->d.y=d->y; object->d.z=d->z;
        osJamMesg(D_80142728,0,0);
    }
}

void func_800D52CC() {}
