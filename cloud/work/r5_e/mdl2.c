#include "mdl_hdr.h"
#define SLOT(i) (*(ModelSlot *)((char *)D_8012E700 + (unsigned)(i) * 68))
void model_data_load(short idx, int mode, int f) {
    if (mode == 2) {
        SLOT(idx).flags |= f << 8;
        if (SLOT(idx).child != -1) {
            model_data_load(SLOT(idx).child, 3, f);
        }
    } else if (mode == 0) {
        SLOT(idx).flags |= 0x80000000;
    } else if (mode == 1) {
        SLOT(idx).flags |= f << 8;
    } else if (mode == 3) {
        do {
            SLOT(idx).flags |= f << 8;
            if (SLOT(idx).child != -1) {
                model_data_load(SLOT(idx).child, 3, f);
            }
            idx = SLOT(idx).sibling;
        } while (idx != -1);
    }
}
