#include <stddef.h>
#include <string.h>
#include "candidate.c"
QNode *D_80124EEC;
static QNode storage[512];
static QNode before[512];
int host_run(const unsigned char *bytes, int count, int start, int x, int y,
             short initial, short *result_quadrant)
{
    int i, j;
    QNode *result;
    for (i = 0; i < count; i++) {
        const unsigned char *p = bytes + 20 * i;
        storage[i].parent = (short)((p[0] << 8) | p[1]);
        storage[i].unknown02 = p[2];
        storage[i].child_mask = p[3];
        storage[i].min_x = (short)((p[4] << 8) | p[5]);
        storage[i].max_x = (short)((p[6] << 8) | p[7]);
        storage[i].min_y = (short)((p[8] << 8) | p[9]);
        storage[i].max_y = (short)((p[10] << 8) | p[11]);
        for (j = 0; j < 4; j++)
            storage[i].child[j] = (unsigned short)((p[12 + 2*j] << 8) | p[13 + 2*j]);
    }
    memcpy(before, storage, count * sizeof(QNode));
    D_80124EEC = storage;
    *result_quadrant = initial;
    result = func_800AC9BC(&storage[start], (short)x, (short)y, result_quadrant);
    if (D_80124EEC != storage || memcmp(before, storage, count * sizeof(QNode)) != 0)
        return -999999;
    return result == NULL ? -1 : (int)(result - storage);
}
int host_layout(void) { return sizeof(QNode) == 20 && offsetof(QNode, child) == 12; }
