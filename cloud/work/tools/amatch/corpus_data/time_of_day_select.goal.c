s32 time_of_day_select(s32 node, s32 pos, s32 *outNode, s32 *dist, s32 depth)
{
    if (!func_800B98D8(node, depth)) {
        return 0;
    }
    if (depth == 0) {
        *dist = 0;
    }
    if (node < 0) {
        *outNode = pos;
        return 1;
    }
    *dist += pos;
    return time_of_day_select(D_801407F0.nodes[node].next, D_801407F0.nodes[node].nextPos,
                              outNode, dist, depth + 1);
}