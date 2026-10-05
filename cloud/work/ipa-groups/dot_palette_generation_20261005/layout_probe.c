/* Numeric native-layout test; not part of the matching compilation unit. */
#include "palette.c"
const u32 palette_layout[] = {
    sizeof(Resource24), sizeof(Handles64), sizeof(Model68),
    (u32)&((Resource24 *)0)->data,
    (u32)&((Model68 *)0)->palette,
    (u32)&((Handles64 *)0)->root.halves.id,
    (u32)&((Handles64 *)0)->secondary.halves.id
};
