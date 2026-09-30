#include "mdl_hdr.h"
#define T D_8012E700
void model_transform_setup(int idx, int mode, int f) {
    if (mode == 1) {
        { unsigned int t = T[(short)idx].flags; T[idx].flags = t & ~((f << 8) | 0x80000000); }
        if (T[idx].child != -1) model_transform_setup(T[idx].child, 2, f);
    } else if (mode == 0) {
        { unsigned int t = T[(short)idx].flags; T[idx].flags = t & ~((f << 8) | 0x80000000); }
    } else if (mode == 0) {
        { unsigned int t = T[(short)idx].flags; T[idx].flags = (t & 0x7fffffff) | (f << 8); }
    } else if (mode == 2) {
        do {
            { unsigned int t = T[(short)idx].flags; T[idx].flags = t & ~((f << 8) | 0x80000000); }
            if (T[idx].child != -1) model_transform_setup(T[idx].child, 2, f);
            idx = T[idx].sibling;
        } while (idx != -1);
    }
}
