s32 minimap_render(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 *dist, s32 stopAtTyped, s32 depth)
{
    if (!func_800B98D8(node, depth)) {
        return 0;
    }
    if (depth == 0) {
        *dist = 0;
    }
    if (node < 0 || (stopAtTyped && D_801407F0.nodes[node].type != 0)) {
        *outNode = node;
        *outPos = pos;
        return 1;
    }
    *dist += D_801407F0.nodes[node].numPoints - pos;
    return minimap_render(D_801407F0.nodes[node].prev, D_801407F0.nodes[node].prevPos,
                          outNode, outPos, dist, stopAtTyped, depth + 1);
}