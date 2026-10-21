/* w36e final body (cloud/work/frontier/w36e/groups/best_ts) */
typedef struct UVMatrix { f32 uvs[3][3]; f32 pos[3]; } UVMatrix;   /* tUVMatrix */
void func_8038C5EC(void)
{
    static f32 angle = 0.0f;
    static f32 fogPos[3];
    UVMatrix fogMat;
    f32 uv[3][3];
    f32 speed;
    f32 frames;
    s32 i;
    s32 slot;
    f32 fogY, fogDelta;
    func_8038BBB4();
    for (D_803BA898 = 0, i = 0; i < 21; i++) if (func_8038ACB0(i)) D_803BA8B0[D_803BA898++] = i;
    frames = D_8002EB94 * 30.0f;
    if ((D_80152570 && !D_80140A04) || (!D_80152570 && D_80140A04)) {
        angle -= 0.01745329238474369f * frames;
        if (angle < 0.0f) angle += 6.2831854820251465f;
    } else {
        angle += 0.01745329238474369f * frames;
        if (angle >= 6.2831854820251465f) angle -= 6.2831854820251465f;
    }
    speed = frames * ((D_803BA918[D_803BA856].targetPos - D_803BA918[D_803BA856].pos[0]) / 10.0f);
    for (i = 0; i < D_803BA852; i++) {
        slot = i - D_803BA856;
        if (slot < 0) slot += D_803BA852;
        if (slot != D_803BA918[i].slot) {
            if (D_803BA918[i].slot == (slot-1 < 0 ? D_803BA852-1 : slot-1)) D_803BA918[i].targetPos -= 100.0f;
            else D_803BA918[i].targetPos += 100.0f;
            D_803BA918[i].slot = slot;
        }
        if (speed != 0.0f) {
            D_803BA918[i].pos[0] += speed;
            if (D_803BA918[i].pos[0] < -(D_803BA852*100/2)) {
                D_803BA918[i].pos[0] += D_803BA852*100;
                D_803BA918[i].targetPos += D_803BA852*100;
            } else if (D_803BA918[i].pos[0] > D_803BA852*100/2) {
                D_803BA918[i].pos[0] -= D_803BA852*100;
                D_803BA918[i].targetPos -= D_803BA852*100;
            }
        }
    }
    fogPos[1] += ((D_803B7A20[D_803BA918[D_803BA856].gTrackNum] - 5.0f - fogPos[1]) / 10.0f) * frames;
    math_utility(D_8011418C, uv);
    func_80090E9C(angle, uv);
    func_800FD754(uv, uv, 2.0f, 1.0f, 2.5f);
}
