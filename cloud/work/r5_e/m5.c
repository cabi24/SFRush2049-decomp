#include "mdl_hdr.h"
#define T D_8012E700
void model_data_load(int idx, unsigned mode, int f) {
    if (mode == 2) { { unsigned int t = T[(short)idx].flags; T[idx].flags = t | (f << 8); }
        if (T[idx].child != -1) model_data_load(T[idx].child, 3, f);
    } else if (mode == 0) { { unsigned int t = T[(short)idx].flags; T[idx].flags = t | 0x80000000; } }
    else if (mode == 1) { { unsigned int t = T[(short)idx].flags; T[idx].flags = t | (f << 8); } }
    else if (mode == 3) {
        do { { unsigned int t = T[(short)idx].flags; T[idx].flags = t | (f << 8); }
            if (T[idx].child != -1) model_data_load(T[idx].child, 3, f);
            idx = T[idx].sibling;
        } while (idx != -1);
    }
}
