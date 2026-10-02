/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef struct Vec3 { f32 x,y,z; } Vec3;
typedef struct Matrix9 { f32 values[3][3]; } Matrix9;
typedef struct Model952 { u32 a,b; f32 position[3]; u8 other20[24]; Matrix9 matrix; u8 other80[32]; f32 corners[4][3]; u8 other160[792]; } Model952;
typedef struct Vehicle2056 {
    Model952 *model; u8 other4[240];
    f32 local[4][3]; u8 other292[264];
    f32 position[3]; u8 other568[12];
    f32 corners[4][3],world[4][3],previous[4][3]; u8 other724[24];
    Matrix9 matrix; u8 other784[700];
    f32 zero_a[4],zero_b[4]; u8 other1516[32];
    u32 wordflags[4]; u16 flags[4],zero_c[4],zero_d[4]; u8 other1588[160];
    f32 origin[3]; Matrix9 basis; f32 offset[3]; u8 other1808[60];
    f32 backup_a[3]; u8 other1880[60]; f32 backup_b[3]; Matrix9 backup_matrix;
    u16 other1988; s16 selector; u8 other1992[64];
} Vehicle2056;
extern s8 D_801427A1;
extern Model952 D_80152818[];
extern void math_utility(Matrix9 *,Matrix9 *);
extern void func_8009E820(f32 *,f32 *,Matrix9 *);
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
    func_8009E820(vehicle->offset,offset,matrix);
    vehicle->position[0] = offset[0] + vehicle->origin[0];
    vehicle->position[1] = offset[1] + vehicle->origin[1];
    vehicle->position[2] = offset[2] + vehicle->origin[2];
    vehicle->backup_b[0] = vehicle->position[0];
    vehicle->backup_a[0] = vehicle->position[0];
    vehicle->backup_b[1] = vehicle->position[1];
    vehicle->backup_a[1] = vehicle->position[1];
    vehicle->backup_b[2] = vehicle->position[2];
    vehicle->backup_a[2] = vehicle->position[2];
    D_80152818[vehicle->selector].position[0] = vehicle->position[0];
    D_80152818[vehicle->selector].position[1] = vehicle->position[1];
    D_80152818[vehicle->selector].position[2] = vehicle->position[2];
    for (i=0;i<4;i++) {
        func_8009E820(vehicle->model->corners[i],vehicle->corners[i],matrix);
        vehicle->corners[i][0] = vehicle->position[0] + vehicle->corners[i][0];
        vehicle->corners[i][1] = vehicle->position[1] + vehicle->corners[i][1];
        vehicle->corners[i][2] = vehicle->position[2] + vehicle->corners[i][2];
        vehicle->zero_a[i] = 0;
        vehicle->zero_b[i] = 0;
        vehicle->flags[i] = 8;
        vehicle->wordflags[i] = vehicle->flags[i];
        vehicle->zero_c[i] = 0;
        vehicle->zero_d[i] = 0;
        func_8009E820(vehicle->local[i],vehicle->world[i],matrix);
        vehicle->world[i][0] = vehicle->position[0] + vehicle->world[i][0];
        vehicle->world[i][1] = vehicle->position[1] + vehicle->world[i][1];
        vehicle->world[i][2] = vehicle->position[2] + vehicle->world[i][2];
        vehicle->previous[i][0] = vehicle->world[i][0];
        vehicle->previous[i][1] = vehicle->world[i][1];
        vehicle->previous[i][2] = vehicle->world[i][2];
    }
}
