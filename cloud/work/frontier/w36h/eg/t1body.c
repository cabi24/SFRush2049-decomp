/* 0x8038F568 ElevateGun */
void func_8038F568(Car *car)
{
/*@{*/    Car *hit;
    f32 vec[3];
    f32 pos[3];
    f32 best = 2000.0f;
    f32 pitch = /*@{z1*/0/*@| 0.0f @| 0.f @}*/;
    f32 yaw = /*@{z1*/0/*@| 0.0f @| 0.f @}*/;
    f32 h;
/*@|    Car *hit;
    f32 vec[3];
    f32 pos[3];
    f32 best = 2000.0f;
    f32 yaw = 0;
    f32 pitch = 0;
    f32 h;
@|    Car *hit;
    f32 vec[3];
    f32 pos[3];
    f32 h;
    f32 best = 2000.0f;
    f32 pitch = 0;
    f32 yaw = 0;
@|    Car *hit;
    f32 pos[3];
    f32 vec[3];
    f32 best = 2000.0f;
    f32 pitch = 0;
    f32 yaw = 0;
    f32 h;
@}*/

    for (hit = D_80152818; hit < /*@{*/&D_80152818[D_801543CA]/*@| D_80152818 + D_801543CA @}*/; hit++) {
        if (car->slot == hit->slot || D_8012E67C[car->slot] == D_8012E67C[hit->slot] ||
            D_8014A250[hit->slot].crashflag || !hit->collidable || hit->dead)
            continue;
        pos[0] = hit->pos[0] - car->pos[0];
        pos[1] = hit->pos[1] - car->pos[1];
        pos[2] = hit->pos[2] - car->pos[2];
        if (car->battle.type >= 8)
            h = /*@{m*/-1.0f * ShotOffsets[car->battle.type][1]/*@| ShotOffsets[car->battle.type][1] * -1.0f @}*/;
        else
            h = /*@{m*/-1.0f * (ShotOffsets[car->battle.type][1] +
                  D_803943A4[car->battle.type][D_8014A250[car->slot].body_type][1])/*@| (ShotOffsets[car->battle.type][1] +
                  D_803943A4[car->battle.type][D_8014A250[car->slot].body_type][1]) * -1.0f @}*/;
        pos[0] = pos[0] + car->uvs[1][0] * h;
        pos[1] = pos[1] + car->uvs[1][1] * h;
        pos[2] = pos[2] + car->uvs[1][2] * h;
        func_800A61B0(pos, vec, &car->uvs[0][0]);
        if (vec[2] < /*@{*/0.0f/*@| 0 @| 0.f @}*/ || vec[0] < /*@{*/-0.5f * vec[2]/*@| vec[2] * -0.5f @}*/ || vec[0] > /*@{*/0.5f * vec[2]/*@| vec[2] * 0.5f @}*/ ||
            vec[1] < -vec[2] || vec[1] > vec[2] || vec[2] > best)
            continue;
        best = vec[2];
        yaw = func_8008C768(vec[0], vec[2]);
        pitch = func_8008C768(vec[1], vec[2]);
    }
    if (yaw < car->battle.yaw)
        car->battle.yaw -= 0.01f;
    else if (yaw > car->battle.yaw)
        car->battle.yaw += 0.01f;
    if (pitch < car->battle.pitch)
        car->battle.pitch -= 0.01f;
    else if (pitch > car->battle.pitch)
        car->battle.pitch += 0.01f;
}
