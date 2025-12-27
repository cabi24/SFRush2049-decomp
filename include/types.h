/**
 * @file types.h
 * @brief Common type definitions for Rush 2049 N64
 *
 * Mirrors arcade source conventions where possible.
 * See reference/repos/rushtherock/game/typedefs.h
 */

#ifndef TYPES_H
#define TYPES_H

/* Basic integer types */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

/* Volatile types (for hardware registers) */
typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

/* N64 Graphics types (from libultra) */
typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
    u64 force_structure_alignment;
} Gfx;
typedef u32 Mtx[4][4];

/* Arcade-style type definitions */
typedef f32 F32;          /* 32-bit float */
typedef s32 S32;          /* Signed 32-bit */
typedef s16 S16;          /* Signed 16-bit */
typedef s8  S08;          /* Signed 8-bit */
typedef u32 U32;          /* Unsigned 32-bit */
typedef u16 U16;          /* Unsigned 16-bit */
typedef u8  U08;          /* Unsigned 8-bit */

/* Boolean */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef s32 BOOL;

/* Vector types */
typedef f32 Vec3f[3];
typedef s16 Vec3s[3];
typedef s32 Vec3i[3];

/* Matrix types */
typedef f32 Mat3f[3][3];
typedef f32 Mat4f[4][4];

/* Fixed-point math (arcade uses 0x4000 = 1.0 scale) */
#define FRAC_BITS 14
#define FRAC_ONE (1 << FRAC_BITS)  /* 0x4000 = 16384 */

/* Common null pointer */
#ifndef NULL
#define NULL ((void*)0)
#endif

/* N64 OS constants */
#define OS_READ  0
#define OS_WRITE 1
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK   1

/* Controller Pak (PFS) constants */
#define PFS_READ  0
#define PFS_WRITE 1
#define PFS_ERR_NOPACK    1
#define PFS_ERR_NEW_PACK  2
#define PFS_ERR_CONTRFAIL 4
#define PFS_ERR_EXIST     5
#define PFS_ERR_ID_FATAL  8
#define PFS_ERR_DEVICE    11

/* N64 Matrix constants */
#define G_MTX_MODELVIEW  0x00
#define G_MTX_PROJECTION 0x04
#define G_MTX_MUL        0x00
#define G_MTX_LOAD       0x02
#define G_MTX_NOPUSH     0x00
#define G_MTX_PUSH       0x01

/* N64 AA render mode constants */
#define G_RM_AA_ZB_OPA_SURF  0x0C184000
#define G_RM_AA_ZB_OPA_SURF2 0x03124000
#define G_RM_AA_ZB_XLU_SURF  0x00504A00
#define G_RM_AA_ZB_XLU_SURF2 0x00104A40

/* Combine mode macros */
#define G_CC_MODULATEIA_PRIM 0

/* Float matrix type alias */
typedef f32 MtxF[4][4];

/* N64 Controller Pak (PFS) structure */
typedef struct {
    s32 status;
    s32 channel;
    void *queue;
    u8 activeBank;
    u8 banks[4];
    u8 dir_size;
    u8 inode_table;
    u32 version;
    s32 bank;
} OSPfs;

/* N64 OS timer structure */
typedef struct {
    void *next;
    void *prev;
    u64 interval;
    u64 value;
    void *mq;
    void *msg;
} OSTimer;

/* N64 RSP task type constants */
#define M_GFXTASK   1
#define M_AUDTASK   2
#define M_VIDTASK   3
#define M_HVQTASK   4

#define SP_UCODE_DATA_SIZE 0x800

/* N64 RDP Image Format constants */
#define G_IM_FMT_RGBA    0
#define G_IM_FMT_YUV     1
#define G_IM_FMT_CI      2
#define G_IM_FMT_IA      3
#define G_IM_FMT_I       4

#define G_IM_SIZ_4b      0
#define G_IM_SIZ_8b      1
#define G_IM_SIZ_16b     2
#define G_IM_SIZ_32b     3

/* N64 RDP Render Mode constants */
#define G_RM_OPA_SURF    0x0C084000
#define G_RM_OPA_SURF2   0x03024000
#define G_RM_XLU_SURF    0x00404200
#define G_RM_XLU_SURF2   0x00104240
#define G_RM_ZB_OPA_SURF 0x0C184000
#define G_RM_ZB_OPA_SURF2 0x03124000
#define G_RM_ZB_XLU_SURF 0x00504A00
#define G_RM_ZB_XLU_SURF2 0x00104A40

/* N64 OS Message Priority */
#define OS_MESG_PRI_NORMAL 0
#define OS_MESG_PRI_HIGH   1

/* N64 I/O Message structure for DMA */
typedef struct {
    s16 type;
    s16 pri;
    s32 status;
    void *mq;
    void *devAddr;
    void *dramAddr;
    u32 size;
} OSIoMesg;

/* External RSP boot/ucode symbols */
extern u8 rspbootTextStart[], rspbootTextEnd[];
extern u8 gspF3DEX2_fifoTextStart[], gspF3DEX2_fifoTextEnd[];
extern u8 gspF3DEX2_fifoDataStart[], gspF3DEX2_fifoDataEnd[];

/* N64 OSTask structure for RSP tasks */
typedef struct {
    u32 type;                /* Task type (M_GFXTASK, M_AUDTASK, etc.) */
    u32 flags;               /* Task flags */
    u64 *ucode_boot;         /* RSP boot microcode */
    u32 ucode_boot_size;     /* Boot microcode size */
    u64 *ucode;              /* RSP microcode */
    u32 ucode_size;          /* Microcode size */
    u64 *ucode_data;         /* Microcode data */
    u32 ucode_data_size;     /* Data size */
    u64 *dram_stack;         /* DRAM stack */
    u32 dram_stack_size;     /* Stack size */
    u64 *output_buff;        /* Output buffer */
    u64 *output_buff_size;   /* Output buffer size */
    u64 *data_ptr;           /* Data pointer */
    u32 data_size;           /* Data size */
    u64 *yield_data_ptr;     /* Yield data pointer */
    u32 yield_data_size;     /* Yield data size */
} OSTask_t;

typedef struct {
    OSTask_t t;
} OSTask;

#endif /* TYPES_H */
