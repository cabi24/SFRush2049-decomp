/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef struct Vec3 { f32 x,y,z; } Vec3;
typedef struct Matrix9 { f32 values[3][3]; } Matrix9;
typedef struct Model952 { u32 a,b; Vec3 position; u8 other20[24]; Matrix9 matrix; u8 other80[32]; Vec3 corners[4]; u8 other160[792]; } Model952;
typedef struct Vehicle2056 {
    Model952 *model; u8 other4[240];
    Vec3 local[4]; u8 other292[264];
    Vec3 position; u8 other568[12];
    Vec3 corners[4],world[4],previous[4]; u8 other724[24];
    Matrix9 matrix; u8 other784[700];
    f32 zero_a[4],zero_b[4]; u8 other1516[32];
    u32 wordflags[4]; u16 flags[4],zero_c[4],zero_d[4]; u8 other1588[160];
    Vec3 origin; Matrix9 basis; Vec3 offset; u8 other1808[60];
    Vec3 backup_a; u8 other1880[60]; Vec3 backup_b; Matrix9 backup_matrix;
    u16 other1988; s16 selector; u8 other1992[64];
} Vehicle2056;
extern s8 D_801427A1;
extern Model952 D_80152818[];
extern void math_utility(Matrix9 *,Matrix9 *);
extern void func_8009E820(Vec3 *,Vec3 *,Matrix9 *);
extern void menu_video_settings(Vehicle2056 *);
void menu_control_settings(Vehicle2056 *vehicle)
{
    int i;
    Matrix9 *matrix = &vehicle->matrix;
    f32 offset[3];
    math_utility(&vehicle->basis,matrix);
    if (D_801427A1) menu_video_settings(vehicle);
    math_utility(&vehicle->basis,&vehicle->backup_matrix);
    math_utility(&vehicle->basis,&D_80152818[vehicle->selector].matrix);
    func_8009E820(&vehicle->offset,(Vec3 *)offset,matrix);
    vehicle->position.x = offset[0] + vehicle->origin.x;
    vehicle->position.y = offset[1] + vehicle->origin.y;
    vehicle->position.z = offset[2] + vehicle->origin.z;
    vehicle->backup_b.x = vehicle->position.x;
    vehicle->backup_a.x = vehicle->position.x;
    vehicle->backup_b.y = vehicle->position.y;
    vehicle->backup_a.y = vehicle->position.y;
    vehicle->backup_b.z = vehicle->position.z;
    vehicle->backup_a.z = vehicle->position.z;
    D_80152818[vehicle->selector].position.x = vehicle->position.x;
    D_80152818[vehicle->selector].position.y = vehicle->position.y;
    D_80152818[vehicle->selector].position.z = vehicle->position.z;
    for (i=0;i<4;i++) {
        func_8009E820(&vehicle->model->corners[i],&vehicle->corners[i],matrix);
        vehicle->corners[i].x += vehicle->position.x;
        vehicle->corners[i].y += vehicle->position.y;
        vehicle->corners[i].z += vehicle->position.z;
        vehicle->zero_a[i] = 0;
        vehicle->zero_b[i] = 0;
        vehicle->flags[i] = 8;
        vehicle->wordflags[i] = vehicle->flags[i];
        vehicle->zero_c[i] = 0;
        vehicle->zero_d[i] = 0;
        func_8009E820(&vehicle->local[i],&vehicle->world[i],matrix);
        vehicle->world[i].x += vehicle->position.x;
        vehicle->world[i].y += vehicle->position.y;
        vehicle->world[i].z += vehicle->position.z;
        vehicle->previous[i].x = vehicle->world[i].x;
        vehicle->previous[i].y = vehicle->world[i].y;
        vehicle->previous[i].z = vehicle->world[i].z;
    }
}
