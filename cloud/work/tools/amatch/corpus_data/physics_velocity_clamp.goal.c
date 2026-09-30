s32 physics_velocity_clamp(s32 node, s32 pos, s32 *out, s32 depth)
{
    s32 i;
    s16 best;

    if (!func_800B98D8(node, depth)) {
        return 0;
    }
    if (node < 0) {
        for (i = 1; i < D_80151CE8[0].count; i++) {
            if (pos < D_80151CE8[i].start) {
                break;
            }
        }
        *out = i - 1;
        return 1;
    }
    best = -1;
    for (i = 0; i < D_80151CE8[0].count; i++) {
        if (D_80151CE8[i].nodeStart[node] >= 0 &&
            (best < 0 || best < D_80151CE8[i].nodeStart[node]) &&
            pos >= D_80151CE8[i].nodeStart[node]) {
            *out = i;
            best = D_80151CE8[i].nodeStart[node];
        }
    }
    if (best >= 0) {
        return 1;
    }
    return physics_velocity_clamp(D_801407F0.nodes[node].next, D_801407F0.nodes[node].nextPos,
                                  out, depth + 1);
}