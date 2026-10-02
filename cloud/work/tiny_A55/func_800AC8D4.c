/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef unsigned int u32;
/* Address-only declarations: this routine never calls these callbacks. */
void physics_velocity_integrate_b();void physics_velocity_integrate_c();void physics_velocity_integrate_d();void physics_velocity_integrate_e();void physics_velocity_integrate_f();
typedef struct Callback24 {s16 status;u16 value;s16 key;u8 pad6[10];void (*callback)();u32 pad20;} Callback24;
typedef struct Object952 {u8 pad0[300];Callback24 callbacks[5];u8 pad420[532];} Object952;
typedef struct Lookup64 {u8 pad0[44];u32 values[5];} Lookup64;
extern u32 D_801174B4;extern Object952 D_80152818[];extern Lookup64 D_80139320[];
void func_800AC8D4(s16 key) {
 Object952 *object;Lookup64 *lookup;u32 a,b,c,d,e;
 if(D_801174B4&8)return;
 object=&D_80152818[key];lookup=&D_80139320[key];
 object->callbacks[0].key=key;object->callbacks[1].key=key;
 a=lookup->values[0],b=lookup->values[1],c=lookup->values[2],d=lookup->values[3],e=lookup->values[4];
 object->callbacks[0].callback=physics_velocity_integrate_f;
 object->callbacks[0].status=-1;object->callbacks[1].callback=physics_velocity_integrate_e;object->callbacks[1].status=-1;
 object->callbacks[2].callback=physics_velocity_integrate_d;object->callbacks[2].status=-1;object->callbacks[2].key=key;
 object->callbacks[3].callback=physics_velocity_integrate_c;object->callbacks[3].status=-1;object->callbacks[3].key=key;
 object->callbacks[4].callback=physics_velocity_integrate_b;object->callbacks[4].status=-1;object->callbacks[4].key=key;
 object->callbacks[0].value=a;object->callbacks[1].value=b;object->callbacks[2].value=c;object->callbacks[3].value=d;object->callbacks[4].value=e;
}
