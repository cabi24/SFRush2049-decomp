/**
 * @file os_pi.h
 * @brief N64 Peripheral Interface (PI) definitions
 *
 * Based on Nintendo SDK headers and reference decomp projects.
 */

#ifndef _OS_PI_H_
#define _OS_PI_H_

#include "types.h"
#include "PR/os_message.h"

/**
 * I/O Message Header
 *
 * Common header for all I/O messages.
 */
typedef struct OSIoMesgHdr {
    u16 type;               /* Message type */
    u8  pri;                /* Message priority (high or normal) */
    u8  status;             /* Return status */
    OSMesgQueue *retQueue;  /* Return message queue for I/O completion */
} OSIoMesgHdr;

/**
 * I/O Message Structure
 *
 * Used for PI DMA requests.
 */
typedef struct OSIoMesg {
    OSIoMesgHdr hdr;        /* Message header */
    void *dramAddr;         /* RDRAM buffer address */
    u32 devAddr;            /* Device (ROM) buffer address */
    u32 size;               /* DMA transfer size in bytes */
    void *piHandle;         /* PI device handle (OSPiHandle*) */
} OSIoMesg;

/**
 * PI Handle Structure
 *
 * Represents a PI device (cartridge ROM, etc.)
 */
typedef struct OSPiHandle {
    struct OSPiHandle *next;    /* Next handle in chain */
    u8 type;                    /* Device type */
    u8 latency;                 /* Domain latency */
    u8 pageSize;                /* Domain page size */
    u8 relDuration;             /* Domain release duration */
    u8 pulse;                   /* Domain pulse width */
    u8 domain;                  /* Which PI domain */
    u32 baseAddress;            /* Domain base address */
    u32 speed;                  /* ROM speed */
} OSPiHandle;

/* Data transfer directions */
#define OS_READ         0       /* Device -> RDRAM */
#define OS_WRITE        1       /* Device <- RDRAM */

/* I/O message types */
#define OS_MESG_TYPE_BASE       10
#define OS_MESG_TYPE_LOOPBACK   (OS_MESG_TYPE_BASE + 0)
#define OS_MESG_TYPE_DMAREAD    (OS_MESG_TYPE_BASE + 1)
#define OS_MESG_TYPE_DMAWRITE   (OS_MESG_TYPE_BASE + 2)
#define OS_MESG_TYPE_VRETRACE   (OS_MESG_TYPE_BASE + 3)
#define OS_MESG_TYPE_COUNTER    (OS_MESG_TYPE_BASE + 4)

/* I/O message priority */
#define OS_MESG_PRI_NORMAL      0
#define OS_MESG_PRI_HIGH        1

/* PI domains */
#define PI_DOMAIN1      0
#define PI_DOMAIN2      1

/* Function prototypes */
u32 osPiGetStatus(void);
s32 osPiWriteIo(u32 devAddr, u32 data);
s32 osPiReadIo(u32 devAddr, u32 *data);
s32 osPiStartDma(OSIoMesg *mb, s32 priority, s32 direction,
                 u32 devAddr, void *dramAddr, u32 size, OSMesgQueue *mq);
void osCreatePiManager(s32 pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt);
OSPiHandle *osCartRomInit(void);

#endif /* _OS_PI_H_ */
