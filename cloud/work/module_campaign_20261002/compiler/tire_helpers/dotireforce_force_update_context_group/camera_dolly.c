/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Tire friction-circle force update. The historical camera_dolly label is
 * misleading: the direct arcade equivalent is frictioncircle in
 * reference/repos/rushtherock/game/tires.c. Its companion func_800BC21C is
 * the arcade calcalpha helper.
 *
 * Computes longitudinal traction and lateral tire force from wheel torque,
 * tire loading, contact-patch slip, and a cubic lateral friction curve.
 * Updates wheel angular velocity, contact-patch lateral displacement,
 * slip torque, and the original signed-byte slip diagnostic.
 *
 * N64 differences observed in native instructions:
 * - Longitudinal velocity is tirev[2], lateral velocity is tirev[0].
 * - The nonpositive-loading branch runs before friction/slip calculations;
 *   positive brake adds angular drag and clamps negative wheel speed to zero.
 * - The under-rotating-wheel branch can blend toward road angular velocity
 *   using the brake threshold and the magnitude of model value944.
 * - Above model speed 20, that branch softens lateral force with a denominator
 *   of 2 plus full horizontal contact speed.
 * - The tire record is compacted; pad fields preserve only native offsets.
 *
 * Retains the original arcade scalar locals and coefficient calculations.
 * sqrtf/fabsf are genuine IDO intrinsics matching native sqrt.s/abs.s.
 */
typedef float f32;
typedef struct {
    char pad0[944]; f32 value944; char pad948[32]; f32 brake;
    char pad984[24]; f32 speed; char pad1012[460]; f32 mass;
    char pad1476[112]; f32 dt;
} Model;
typedef struct {
    f32 tradius,springK,rubdamp,unk12,Cfmax,invmi,unk24,Afmax,k1,k2,k3;
    char pad44[24];
    f32 patchy,angvel,sliptorque,sideforce,traction;
    signed char slipflag;
} Tire;
extern f32 D_80123E0C;
extern f32 func_800BC21C(f32 *);
extern f32 sqrtf(f32);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
#pragma intrinsic(sqrtf)

void camera_dolly(Model *m, f32 tirev[3], f32 normalforce, f32 torque,
                    Tire *tire, f32 *sfp, f32 *trp)
{
    f32 maxtraction;
    f32 maxtorque,maxf,Cfmax,Afmax,temp,roadangvel;
    f32 ydot,p,k2,k3,l2,l3,patchvel,patchspeed,realtorque;
        f32 alpha;


    realtorque = torque;
    temp = m->mass*tire->tradius;

    tire->slipflag=0;

    torque = temp*torque/(temp + (f32)1.0/tire->invmi);

    if(normalforce <= 0) {
        *trp = *sfp = 0;
        tire->slipflag += 3;
        tire->angvel += realtorque * tire->invmi * m->dt;
        if(m->brake > 0) {
            tire->angvel -= m->brake * 1000.0f * tire->invmi * m->dt;
            if(tire->angvel < 0) tire->angvel = 0;
        }
        return;
    }

    maxtraction = tire->Cfmax*normalforce;
    maxtorque = maxtraction*tire->tradius;
    roadangvel = tirev[2]/tire->tradius;
    if(tire->angvel > roadangvel){
        tire->sliptorque = maxtorque;
        tire->angvel += (realtorque - tire->sliptorque)*tire->invmi*m->dt;
        if(tire->angvel <= roadangvel){
            tire->slipflag = 10;
            tire->angvel = roadangvel;
        }
        else{
            patchvel = tirev[2] - tire->angvel*tire->tradius;
            patchspeed = sqrtf(patchvel*patchvel +
                 tirev[0]*tirev[0]);
            if(patchspeed == 0){
                *trp = maxtraction;
                *sfp = 0;
            }
            else{
                *trp = -maxtraction * patchvel / patchspeed;
                *sfp = -maxtraction * tirev[0] / patchspeed;
            }
            tire->slipflag=20;
            return;
        }
    }
    else if(tire->angvel < roadangvel){
        tire->sliptorque = -(f32)maxtorque;
        if(m->brake > D_80123E0C && roadangvel > 5.0f)
            tire->angvel = roadangvel - fabsf(m->value944) * (roadangvel - tire->angvel);
        else
            tire->angvel += (realtorque-tire->sliptorque)*tire->invmi*m->dt;

        if(tire->angvel >= roadangvel){
            tire->angvel = roadangvel;
            tire->slipflag = 30;
        }
        else{
            patchvel = tirev[2] - tire->angvel*tire->tradius;
            patchspeed = sqrtf(patchvel*patchvel +
                 tirev[0]*tirev[0]);

            if(patchspeed == 0){
                *trp = -maxtraction;
                *sfp = 0;
            }
            else{
                *trp = -maxtraction * patchvel / patchspeed;
                if(m->speed > 20.0f) {
                    patchspeed = sqrtf(tirev[2]*tirev[2] + tirev[0]*tirev[0]);
                    *sfp = -maxtraction * tirev[0] / (2.0f + patchspeed);
                }
                else
                    *sfp = -maxtraction * tirev[0] / patchspeed;
            }

            tire->slipflag=40;
            return;
        }
    }

    tire->sliptorque=0;

    if(torque != 0){

        if(torque >= maxtorque){
            *trp = maxtraction;
            *sfp = 0;
            tire->slipflag += 4;
            tire->sliptorque = maxtorque;
            tire->angvel += (realtorque - tire->sliptorque)*
                tire->invmi*m->dt;

            return;
        }

        if(torque <= -maxtorque){
            *trp = -(f32)maxtraction;
            *sfp = 0;
            tire->slipflag += 5;
            tire->sliptorque = -(f32)maxtorque;
            tire->angvel += (realtorque - tire->sliptorque)*
                tire->invmi*m->dt;
            return;
        }

        *trp = torque/tire->tradius;
        temp = *trp/normalforce;

        Cfmax = sqrtf(tire->Cfmax * tire->Cfmax - temp * temp);

        k2 = tire->k1*tire->k1/(3*Cfmax);
        k3 = tire->k1*tire->k1*tire->k1/(27*Cfmax*Cfmax);
        Afmax = 3*Cfmax/tire->k1;
    }
    else{
        *trp = 0;
        k2 = tire->k2;
        k3 = tire->k3;
        Cfmax = tire->Cfmax;
        Afmax = tire->Afmax;
    }

    l2 = k2;
    l3 = k3;

    maxf = Cfmax * normalforce;

    alpha = func_800BC21C(tirev);

    tire->sliptorque=0;

    if(alpha >= 0){
        if(alpha >= Afmax){
            ydot = -(f32)tirev[0];
            tire->patchy += ydot*m->dt;

            *sfp = tire->springK*tire->patchy +
                tire->rubdamp * ydot;

            if(*sfp < -maxf){
                *sfp = -(f32)maxf;
                tire->patchy = *sfp/tire->springK;
            }
            tire->slipflag += 6;
            tire->sliptorque = *trp * tire->tradius;
            tire->angvel += (realtorque - tire->sliptorque)*
                tire->invmi*m->dt;
            return;
        }
        tire->slipflag += 7;
        p = tirev[2]*tire->springK/(normalforce *
            (tire->k1 - l2*alpha + l3*alpha*alpha));
        if(p <  0)p = -(f32)p;

        if(p * m->dt < (f32).5){
            ydot = -p * tire->patchy - tirev[0];
            tire->patchy += ydot*m->dt;

            *sfp = tire->springK*tire->patchy +
                tire->rubdamp * ydot;
            tire->slipflag += 100;
        }
        else{
            *sfp = -(tire->k1*alpha - k2*alpha*alpha +
                k3*alpha*alpha*alpha)*normalforce;

            tire->patchy = *sfp/tire->springK;
            tire->slipflag += 200;
        }

    }
    else{
        if(alpha <= -Afmax){
            ydot = -(f32)tirev[0];
            tire->patchy += ydot*m->dt;

            *sfp = tire->springK*tire->patchy +
                tire->rubdamp * ydot;
            if(*sfp > maxf){
                *sfp = maxf;
                tire->patchy = *sfp/tire->springK;
            }
            tire->slipflag += 8;
            tire->sliptorque = *trp * tire->tradius;
            tire->angvel += (realtorque - tire->sliptorque)*
                tire->invmi*m->dt;
            return;
        }

        tire->slipflag += 9;
        p = tirev[2]*tire->springK/(normalforce *
            (tire->k1 + l2*alpha + l3*alpha*alpha));

        if(p <  0)p = -(f32)p;

        if(p * m->dt < (f32).5){
            ydot = -p * tire->patchy - tirev[0];
            tire->patchy += ydot*m->dt;

            *sfp = tire->springK*tire->patchy +
                tire->rubdamp * ydot;
            tire->slipflag += 100;
        }
        else{
            *sfp = -(tire->k1*alpha + k2*alpha*alpha +
                k3*alpha*alpha*alpha)*normalforce;

            tire->patchy = *sfp / tire->springK;
            tire->slipflag += 200;
        }

    }
}
