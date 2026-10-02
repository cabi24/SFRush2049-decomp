/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef struct Description {u8 opaque[16];u8 kind;} Description;
typedef struct ChildHandle {Description *data;} ChildHandle;
typedef struct Object {u8 opaque[8];ChildHandle *child;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Row772 {u8 opaque[49];s8 flag;u8 tail[722];} Row772;
extern Row772 D_80144000[];
extern int func_800A1A60(ChildHandle *);
int func_800CC848(Handle *handle,int activate)
{
    ChildHandle *child=handle->object->child;
    if(child==0)return 1;
    {
        int kind=child->data->kind;
        int flag=D_80144000[kind].flag;
        if(flag==0)return 0;
    }
    if(activate==0)return 1;
    return func_800A1A60(child);
}
