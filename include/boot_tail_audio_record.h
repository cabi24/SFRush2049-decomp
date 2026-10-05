#ifndef BOOT_TAIL_AUDIO_RECORD_H
#define BOOT_TAIL_AUDIO_RECORD_H

/* Native audio record, allocated and traversed in 0x68-byte units.
 * See cloud/work/boot_tail_promotion/audio_record_contracts/README.md.
 * Anonymous union is supported by the pinned IDO compiler: the two names at
 * 0x20 preserve the existing envelope and parameter-writer source contracts.
 * Unknown ranges describe observed gaps, not added object-code padding. */
typedef struct AudioState {
    unsigned char active;                 /* 0x00 */
    unsigned char pending;                /* 0x01 */
    unsigned short flags;                 /* 0x02 */
    unsigned short rate;                  /* 0x04 */
    unsigned char unknown06[2];
    double sample_position;               /* 0x08 */
    unsigned int current;                 /* 0x10 */
    unsigned int position;                /* 0x14: previous integer position */
    unsigned short unknown18[4];
    union {
        unsigned short initial_count;     /* 0x20: envelope view */
        unsigned short value20;           /* 0x20: parameter-writer view */
    };
    unsigned short value22;               /* 0x22 */
    float value24;                        /* 0x24 */
    unsigned short release_count;         /* 0x28 */
    unsigned char unknown2A[2];
    void *data;                           /* 0x2C */
    unsigned int length;                  /* 0x30 */
    unsigned int loop_start;              /* 0x34 */
    unsigned int loop_length;             /* 0x38 */
    unsigned int tail;                    /* 0x3C */
    unsigned short value40;               /* 0x40 */
    unsigned short value42;               /* 0x42 */
    unsigned short value44;               /* 0x44 */
    unsigned short value46;               /* 0x46 */
    unsigned short count;                 /* 0x48 */
    unsigned short unknown4A;
    float value;                          /* 0x4C */
    unsigned int step;                    /* 0x50 */
    float scale;                          /* 0x54 */
    float saved_value;                    /* 0x58 */
    unsigned char state;                  /* 0x5C */
    unsigned char format;                 /* 0x5D */
    unsigned char unknown5E[2];
    unsigned char unknown60;              /* 0x60 */
    unsigned char release_pending;        /* 0x61 */
    unsigned char unknown62[6];
} AudioState;
/* Existing accepted parameter writer uses this name for the same record. */
typedef AudioState AudioSlot;
extern AudioState *D_80038294;
extern void func_80011A10(AudioState *);
extern void func_80011A3C(AudioState *);

#endif
