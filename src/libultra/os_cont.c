/**
 * @file os_cont.c
 * @brief N64 controller interface functions
 *
 * Controller query and read functions for the N64 joybus interface.
 * Uses PIF (Peripheral Interface) for communication.
 */

#include "types.h"

/* Controller data structures */
typedef struct {
    u16 type;           /* Controller type */
    u8  status;         /* Status flags */
    u8  errno;          /* Error number */
} OSContStatus;

typedef struct {
    u16 button;         /* Button states */
    s8  stick_x;        /* Analog stick X (-80 to 80) */
    s8  stick_y;        /* Analog stick Y (-80 to 80) */
    u8  errno;          /* Error number */
} OSContPad;

/* PIF command buffer - 64 bytes */
#define PIF_CMD_SIZE 64
#define CONT_CMD_REQUEST_STATUS 0x00
#define CONT_CMD_READ_BUTTON    0x01
#define CONT_CMD_RESET          0xFF
#define CONT_CMD_END            0xFE

/* External data */
extern u8 D_80037AA0[60];   /* PIF command/response buffer (0x3C bytes) */
extern u8 D_80037ADC;       /* End of buffer */
extern u8 D_80037AE0;       /* Controller read in progress flag */
extern u8 D_80037AE1;       /* Number of controllers (max 4) */

/* External functions */
extern void __osSiGetAccess(void);              /* Acquire SI access */
extern void __osSiRelAccess(void);              /* Release SI access */
extern s32 __osSiRawStartDma(s32 write, u8 *buf); /* SI raw DMA to/from PIF */
extern void osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag); /* Receive message */

/**
 * Build controller query command packet
 * Address: 0x80009658
 *
 * Initializes PIF buffer with status query commands for all controllers.
 *
 * @param channel Controller channel (0-3), 0 queries all
 */
void __osContRamReset(u32 channel) {
    u8 *ptr = D_80037AA0;
    u8 *end = &D_80037ADC;
    u8 pattern[8];
    s32 i;

    /* Clear buffer */
    while (ptr < end) {
        *ptr++ = 0;
    }

    /* Set skip flag */
    *(u32 *)(D_80037AA0 + 0x3C) = 1;

    /* Build command packets for each controller */
    pattern[0] = 0xFF;  /* Tx bytes */
    pattern[1] = 0x01;  /* Rx bytes */
    pattern[2] = 0x03;  /* Command: reset */
    pattern[3] = (u8)channel;
    pattern[4] = 0xFF;
    pattern[5] = 0xFF;
    pattern[6] = 0xFF;
    pattern[7] = 0xFF;

    ptr = D_80037AA0;
    for (i = 0; i < D_80037AE1; i++) {
        /* Copy pattern to buffer (unaligned store) */
        *(u32 *)ptr = *(u32 *)pattern;
        *(u32 *)(ptr + 4) = *(u32 *)(pattern + 4);
        ptr += 8;
    }

    /* End marker */
    *ptr = CONT_CMD_END;
}

/**
 * Start controller status query
 * Address: 0x80009730
 *
 * Initiates async read of controller status (type, present, pak status).
 *
 * @param mq Message queue to signal on completion
 * @return 0 on success
 */
s32 osContStartQuery(OSMesgQueue *mq) {
    s32 ret;

    __osSiGetAccess();

    if (D_80037AE0 != 0) {
        /* Controller already in use - re-init */
        __osContRamReset(0);
        __osSiRawStartDma(1, D_80037AA0);  /* Write to PIF */
        osRecvMesg(mq, NULL, 1);     /* Wait for completion */
    }

    /* Read results */
    ret = __osSiRawStartDma(0, D_80037AA0);
    D_80037AE0 = 0;
    __osSiRelAccess();

    return ret;
}

/**
 * Get controller query results
 * Address: 0x800097AC
 *
 * Parses PIF response buffer into OSContStatus array.
 * Wrapper for __osContGetInitData.
 *
 * @param status Array of 4 OSContStatus structures
 */
void osContGetQuery(OSContStatus *status) {
    u8 bitpattern;
    func_800095AC(&bitpattern, status);
}

/**
 * Start controller button read (alternate)
 * Address: 0x800097D0
 *
 * Initiates async read of controller buttons and stick.
 *
 * @param mq Message queue to signal on completion
 * @return 0 on success
 */
s32 osContStartReadData2(OSMesgQueue *mq) {
    s32 ret;

    __osSiGetAccess();

    if (D_80037AE0 != 1) {
        /* Need to send read command first */
        func_800098E0();  /* Build read command */
        __osSiRawStartDma(1, D_80037AA0);  /* Write to PIF */
        osRecvMesg(mq, NULL, 1);     /* Wait for completion */
    }

    /* Read results */
    ret = __osSiRawStartDma(0, D_80037AA0);
    D_80037AE0 = 1;  /* Mark as read mode */
    __osSiRelAccess();

    return ret;
}

/**
 * Get controller read data
 * Address: 0x80009854
 *
 * Parses PIF response into OSContPad array.
 *
 * @param pad Array of 4 OSContPad structures
 */
void osContGetReadData(OSContPad *pad) {
    u8 *buf = D_80037AA0;
    u8 response[8];
    s32 i;

    for (i = 0; i < D_80037AE1; i++) {
        /* Copy response (unaligned load) */
        *(u32 *)response = *(u32 *)buf;
        *(u32 *)(response + 4) = *(u32 *)(buf + 4);

        /* Check for errors (upper 2 bits of status byte) */
        pad->errno = (response[2] & 0xC0) >> 4;

        if (pad->errno == 0) {
            /* Valid data - parse buttons and stick */
            pad->button = (response[4] << 8) | response[5];
            pad->stick_x = (s8)response[6];
            pad->stick_y = (s8)response[7];
        }

        buf += 8;
        pad++;
    }
}

/**
 * Build controller read command packet
 * Address: 0x800098E0
 *
 * Initializes PIF buffer with button read commands for all controllers.
 */
void func_800098E0(void) {
    u8 *ptr = D_80037AA0;
    u8 *end = &D_80037ADC;
    u8 pattern[8];
    s32 i;

    /* Clear buffer */
    while (ptr < end) {
        *ptr++ = 0;
    }

    /* Set skip flag */
    *(u32 *)(D_80037AA0 + 0x3C) = 1;

    /* Build command packets for each controller */
    pattern[0] = 0xFF;  /* Tx bytes (will be overwritten) */
    pattern[1] = 0x01;  /* Rx bytes */
    pattern[2] = 0x04;  /* Rx bytes (extended) */
    pattern[3] = 0x01;  /* Command: read buttons */
    pattern[4] = 0xFF;
    pattern[5] = 0xFF;
    pattern[6] = 0xFF;
    pattern[7] = 0xFF;

    ptr = D_80037AA0;
    for (i = 0; i < D_80037AE1; i++) {
        /* Copy pattern to buffer (unaligned store) */
        *(u32 *)ptr = *(u32 *)pattern;
        *(u32 *)(ptr + 4) = *(u32 *)(pattern + 4);
        ptr += 8;
    }

    /* End marker */
    *ptr = CONT_CMD_END;
}

/**
 * Parse controller init/status data
 * Address: 0x800095AC
 *
 * Parses PIF response into OSContStatus structures.
 *
 * @param bitpattern Output: bitmask of connected controllers
 * @param status Array of OSContStatus structures
 * @return Bitpattern of connected controllers
 */
u8 func_800095AC(u8 *bitpattern, OSContStatus *status) {
    u8 *buf = D_80037AA0;
    u8 response[8];
    u8 pattern = 0;
    s32 i;

    for (i = 0; i < D_80037AE1; i++) {
        /* Copy response (unaligned load) */
        *(u32 *)response = *(u32 *)buf;
        *(u32 *)(response + 4) = *(u32 *)(buf + 4);

        /* Parse status - error in upper bits */
        status->errno = (response[2] & 0xC0) >> 4;

        if (status->errno == 0) {
            /* Valid - parse type */
            status->type = (response[4] << 8) | response[5];
            status->status = response[6];
            pattern |= (1 << i);
        }

        buf += 8;
        status++;
    }

    *bitpattern = pattern;
    return pattern;
}
