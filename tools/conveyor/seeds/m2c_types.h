/* LLM-grown type context for auto-decompilation (autodecomp #1).
 *
 * m2c produces `void *` for globals it can't type and then dereferences them
 * (`g->unk8`), which won't compile. Each struct/typedef/extern added here
 * gives m2c real layouts, so every function that touches that symbol compiles
 * and can be searched. One definition typically unlocks a cluster.
 *
 * Rules: types/typedefs/struct defs and `extern` global declarations only, no
 * function bodies. Field names may be `unkN` (offset N) until known. Keep it
 * self-consistent; `autodecomp clusters` re-measures after every edit.
 */
#ifndef M2C_TYPES_H
#define M2C_TYPES_H
#endif
