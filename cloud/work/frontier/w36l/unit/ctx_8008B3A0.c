/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* ZOID_FindMatrix (MAT3 zoid.c): ObjInstanceList[objnum].mat, written against the
 * D_8012E700 instance table so that inlined copies share the table base with
 * ZOID_SetColor/ZOID_ObjectFlags (the locked single uses &D_8012E708). */
typedef struct Node Node;
typedef struct ModelSlot { unsigned int flags; char pad4[4]; Node *node; char padC[0x38]; } ModelSlot;
extern ModelSlot D_8012E700[];
Node *func_8008B3A0(short objnum) { return D_8012E700[objnum].node; }
