/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float F32;
typedef struct {
    char unknown0[944]; F32 brake_gain; char unknown948[32]; F32 brake;
    char unknown984[24]; F32 speed; char unknown1012[460]; F32 mass;
    char unknown1476[112]; F32 dt;
} MODELDAT;
struct tiredes {
    F32 tradius, springK, rubdamp, Cstiff, Cfmax, invmi, Zforce, Afmax;
    F32 k1,k2,k3,l2,l3,m1,m2,m3,m4,patchy,angvel,sliptorque,sideforce,traction;
    signed char slipflag;
};
extern F32 D_80123E0C;
extern F32 sqrtf(F32);
extern F32 fabsf(F32);
#pragma intrinsic(fabsf)
#pragma intrinsic(sqrtf)
extern F32 func_800BC21C(F32 *);
void camera_dolly(MODELDAT *m, F32 tirev[3], F32 normalforce, F32 torque,
                    struct tiredes *tire, F32 *sfp, F32 *trp)
{
    F32 maxtraction;
    F32 maxtorque,maxf,Cfmax,Afmax,temp,roadangvel;
    F32 ydot,p,k2,k3,l2,l3,patchvel,patchspeed,realtorque;
        F32 alpha;


    realtorque = torque;
    temp = m->mass*tire->tradius;

    tire->slipflag=0;

    torque = temp*torque/(temp + (F32)1.0/tire->invmi);

    if(normalforce <= 0){
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
            patchspeed = sqrtf(tirev[0]*tirev[0] + patchvel*patchvel);
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
        tire->sliptorque = -(F32)maxtorque;
        if(m->brake > D_80123E0C && roadangvel > 5.0f)
            tire->angvel = roadangvel - fabsf(m->brake_gain) * (roadangvel - tire->angvel);
        else
            tire->angvel += (realtorque-tire->sliptorque)*tire->invmi*m->dt;

        if(tire->angvel >= roadangvel){
            tire->angvel = roadangvel;
            tire->slipflag = 30;
        }
        else{
            patchvel = tirev[2] - tire->angvel*tire->tradius;
            patchspeed = sqrtf(tirev[0]*tirev[0] + patchvel*patchvel);

            if(patchspeed == 0){
                *trp = -maxtraction;
                *sfp = 0;
            }
            else{
                *trp = -maxtraction * patchvel / patchspeed;
                if(m->speed > 20.0f)
                    *sfp = -maxtraction * tirev[0] / (2.0f + sqrtf(tirev[0]*tirev[0] + tirev[2]*tirev[2]));
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
            *trp = -(F32)maxtraction;
            *sfp = 0;
            tire->slipflag += 5;
            tire->sliptorque = -(F32)maxtorque;
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



    alpha = func_800BC21C(tirev);

    tire->sliptorque=0;

    if(alpha >= 0){
        if(alpha >= Afmax){
            ydot = -(F32)tirev[0];
            tire->patchy += ydot*m->dt;

            *sfp = tire->springK*tire->patchy +
                tire->rubdamp * ydot;

            maxf = -(Cfmax * normalforce);
            if(*sfp < maxf){
                *sfp = maxf;
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
        if(p <  0)p = -(F32)p;

        if(p * m->dt < (F32).5){
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
            ydot = -(F32)tirev[0];
            tire->patchy += ydot*m->dt;

            *sfp = tire->springK*tire->patchy +
                tire->rubdamp * ydot;
            maxf = Cfmax * normalforce;
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

        if(p <  0)p = -(F32)p;

        if(p * m->dt < (F32).5){
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
