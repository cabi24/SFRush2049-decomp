typedef struct ModelSlot {
    unsigned int flags;
    char pad4[0x12];
    short child;
    short sibling;
    char pad1A[0x2A];
} ModelSlot; /* 0x44 */
extern ModelSlot D_8012E700[];
