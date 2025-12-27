/**
 * @file os_cont.h
 * @brief N64 Controller interface definitions
 *
 * Based on Nintendo SDK headers and reference decomp projects.
 */

#ifndef _OS_CONT_H_
#define _OS_CONT_H_

#include "types.h"
#include "PR/os_message.h"

/**
 * Controller Status Structure
 *
 * Returned by osContGetQuery() after osContStartQuery().
 */
typedef struct OSContStatus {
    u16 type;           /* Controller type */
    u8  status;         /* Controller status flags */
    u8  errno;          /* Error number */
} OSContStatus;

/**
 * Controller Pad Data Structure
 *
 * Returned by osContGetReadData() after osContStartReadData().
 */
typedef struct OSContPad {
    u16 button;         /* Button states */
    s8  stick_x;        /* Analog stick X (-80 to 80) */
    s8  stick_y;        /* Analog stick Y (-80 to 80) */
    u8  errno;          /* Error number */
} OSContPad;

/**
 * Controller RAM I/O Structure
 *
 * Used for Controller Pak read/write operations.
 */
typedef struct OSContRamIo {
    void *address;          /* RAM address (11 bits) */
    u8 databuffer[32];      /* Data buffer */
    u8 addressCrc;          /* CRC code for address */
    u8 dataCrc;             /* CRC code for data */
    u8 errno;               /* Error number */
} OSContRamIo;

/* Maximum number of controllers */
#define MAXCONTROLLERS  4

/* Controller error flags */
#define CONT_NO_RESPONSE_ERROR  0x8
#define CONT_OVERRUN_ERROR      0x4

/* Controller type flags */
#define CONT_ABSOLUTE           0x0001
#define CONT_RELATIVE           0x0002
#define CONT_JOYPORT            0x0004
#define CONT_EEPROM             0x8000
#define CONT_EEP16K             0x4000
#define CONT_TYPE_MASK          0x1f07
#define CONT_TYPE_NORMAL        0x0005
#define CONT_TYPE_MOUSE         0x0002
#define CONT_TYPE_VOICE         0x0100

/* Controller status flags */
#define CONT_CARD_ON            0x01
#define CONT_CARD_PULL          0x02
#define CONT_ADDR_CRC_ER        0x04
#define CONT_EEPROM_BUSY        0x80

/* Button masks */
#define CONT_A      0x8000
#define CONT_B      0x4000
#define CONT_G      0x2000  /* Z button */
#define CONT_START  0x1000
#define CONT_UP     0x0800
#define CONT_DOWN   0x0400
#define CONT_LEFT   0x0200
#define CONT_RIGHT  0x0100
#define CONT_L      0x0020
#define CONT_R      0x0010
#define CONT_E      0x0008  /* C-Up */
#define CONT_D      0x0004  /* C-Down */
#define CONT_C      0x0002  /* C-Left */
#define CONT_F      0x0001  /* C-Right */

/* Nintendo's official button names */
#define A_BUTTON        CONT_A
#define B_BUTTON        CONT_B
#define L_TRIG          CONT_L
#define R_TRIG          CONT_R
#define Z_TRIG          CONT_G
#define START_BUTTON    CONT_START
#define U_JPAD          CONT_UP
#define L_JPAD          CONT_LEFT
#define R_JPAD          CONT_RIGHT
#define D_JPAD          CONT_DOWN
#define U_CBUTTONS      CONT_E
#define L_CBUTTONS      CONT_C
#define R_CBUTTONS      CONT_F
#define D_CBUTTONS      CONT_D

/* Controller error numbers */
#define CONT_ERR_NO_CONTROLLER  1   /* No controller connected */
#define CONT_ERR_CONTRFAIL      4   /* Controller overrun error */
#define CONT_ERR_INVALID        5   /* Invalid operation */
#define CONT_ERR_DEVICE         11  /* Wrong device type */
#define CONT_ERR_NOT_READY      12  /* Controller not ready */

/* Function Prototypes */
s32 osContInit(OSMesgQueue *mq, u8 *pattern, OSContStatus *status);
s32 osContReset(OSMesgQueue *mq, OSContStatus *status);
s32 osContStartQuery(OSMesgQueue *mq);
s32 osContStartReadData(OSMesgQueue *mq);
s32 osContSetCh(u8 num);
void osContGetQuery(OSContStatus *status);
void osContGetReadData(OSContPad *pad);

#endif /* _OS_CONT_H_ */
