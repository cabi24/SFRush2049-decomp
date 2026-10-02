/* Exact whole-group flags: -g0 -O3 -mips2 -G 0 -non_shared
 * -Wab,-r4300_mul; cc and all optimization/codegen stages use O3.
 * uld merges this one real entry with the protected keep list.
 * Ordinary O2 also produces exact text and literals.
 */
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
    if (((m->engangvel+angvel)>m->clutchangvel)^was_slipping) {
        m->engangvel=m->clutchangvel;
        m->clutchtorque=m->engtorque;
    } else m->engangvel+=angvel;
    m->efdwinvmi=(f32)1.0/((f32)1.0/m->dwinvmi+totratsq/m->parameters->enginvmi);
    m->dwtorque=m->clutchtorque*m->totalratio;
}

/* func_800E2AC4: integrate clutch and engine rotation, detect a crossing
 * through clutch angular velocity, and update the effective driven inertia.
 * Arcade ancestry: reference/repos/rushtherock/game/drivetra.c:whatslips;
 * rpmtordps is the actual macro from game/drivsym.h:23.
 * Tier: portable drivetrain with N64-specific controller/clutch adaptation.
 * The native N64 branch clamps idle angular velocity to 1000 RPM and uses
 * the donor 0.8 clutch friction point and 1.25 gain. Its tuning gain comes
 * from the actual second model pointer. A single crossing decision replaces
 * the arcade's repeated integration branches.
 * ABI: one ordinary model pointer; complete 428-byte/107-word leaf extent.
 * XOR combines two genuine 0/1 predicates to detect crossing. The compiler
 * shares the predicted engine+increment value with the compound update;
 * this produces one actual addition and the native result carrier.
 * Two source-built float constants occupy the native eight-byte pool at
 * 0x801243D8..0x801243DF. Whole O3 body and pool both match after protected
 * relocation. There is no stand-in callee, hidden argument, extra work or
 * register-pressure storage. Root image/ROM acceptance is a separate gate.
 */
