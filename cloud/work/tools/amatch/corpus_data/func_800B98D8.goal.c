s32 func_800B98D8(s32 node, s32 depth)
{
    s32 i;

    if (node >= 0 && D_801407F0.nodes[node].type != 0) {
        if ((D_801407F0.nodes[node].next >= 0 &&
             D_801407F0.nodes[D_801407F0.nodes[node].next].type == 0) ||
            (D_801407F0.nodes[node].prev >= 0 &&
             D_801407F0.nodes[D_801407F0.nodes[node].prev].type == 0)) {
            return 0;
        }
    }
    D_80143A88[depth] = node;
    for (i = 0; i < depth; i++) {
        if (node == D_80143A88[i]) {
            return 0;
        }
    }
    return 1;
}