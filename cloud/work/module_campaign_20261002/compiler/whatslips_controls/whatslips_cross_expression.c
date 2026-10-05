/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/drivetra.c:whatslips.
 * The N64 port uses a single crossing test for the clutch integration. */
typedef float f32;
typedef signed short s16;
typedef unsigned char u8;
typedef struct Parameters {u8 prefix[160]; f32 enginvmi; u8 gap164[4]; f32 clutchmaxt;} Parameters;
typedef struct Tuning {u8 prefix[16]; f32 clutch_gain;} Tuning;
typedef struct Model2056 {
    Parameters *parameters; Tuning *tuning;
    u8 to_clutch[964]; f32 clutch;
    u8 to_gear[36]; s16 gear; u8 gap1014[14];
    f32 engtorque,engangvel; u8 gap1036[4];
    f32 dwtorque,dwinvmi,efdwinvmi,dwangvel,clutchtorque,clutchangvel;
    u8 gap1064[4]; f32 totalratio;
    u8 to_dt[516]; f32 dt; u8 tail[464];
} Model2056;
#define rpmtordps (2.0*3.14159/60.0)
void func_800E2AC4(Model2056 *m)
{
    f32 totratsq,curclmaxt,angvel;
    int was_slipping;
    totratsq=m->totalratio*m->totalratio;
    if (m->engangvel<(f32)(1000*rpmtordps)) m->engangvel=(f32)(1000*rpmtordps);
    if ((f32).8<m->clutch) curclmaxt=0;
    else curclmaxt=((f32).8-m->clutch)*(f32)1.25*m->parameters->clutchmaxt;
    if (m->gear==0 || curclmaxt==0) {
        m->clutchtorque=0;
        m->dwtorque=0;
        m->clutchangvel=m->engangvel;
        m->engangvel+=m->engtorque*m->parameters->enginvmi*m->dt;
        m->efdwinvmi=m->dwinvmi;
        return;
    }
    curclmaxt*=m->tuning->clutch_gain;
    m->clutchangvel=m->dwangvel*m->totalratio;
    was_slipping=0;
    if (m->engangvel>m->clutchangvel) was_slipping=1;
    if (was_slipping) m->clutchtorque=curclmaxt;
    else m->clutchtorque=-curclmaxt;
    angvel=(m->engtorque-m->clutchtorque)*m->parameters->enginvmi*m->dt;
    angvel=m->engangvel+angvel;
    if ((angvel>m->clutchangvel)!=was_slipping) {
        m->engangvel=m->clutchangvel;
        m->clutchtorque=m->engtorque;
    } else m->engangvel=angvel;
    m->efdwinvmi=(f32)1.0/((f32)1.0/m->dwinvmi+totratsq/m->parameters->enginvmi);
    m->dwtorque=m->clutchtorque*m->totalratio;
}
