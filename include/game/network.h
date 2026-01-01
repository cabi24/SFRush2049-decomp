/**
 * network.h - Network definitions for Rush 2049 N64
 *
 * Based on arcade game/gamenet.h and OS/net_link.h
 * N64 is single-player so these are stub definitions for compatibility.
 *
 * Arcade network supported 8-player linked cabinets over Ethernet.
 * This header provides the types and constants for code compatibility.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include "types.h"
#include "game/game.h"

/*=============================== DEFINES ================================*/

/* Network enable - set to 0 for N64 (single-player only) */
#define GAMENET             0

/* Network protocol version (arcade used version 3) */
#define NET_VERSION         3

/* Maximum linked cabinets (arcade supported 8) */
#define MAX_LINKS           8

/* Timing constants (in milliseconds, based on arcade IRQTIME) */
#define GN_1SEC             1000
#define GN_VERIFY_TIME      50
#define GN_REFRESH_TIME     (GN_1SEC * 4)
#define GN_IS_ALIVE         (GN_REFRESH_TIME * 2)
#define GN_GRACE_PERIOD     (GN_1SEC / 10)

/* Network result codes */
#define NETOK                   0x001
#define NETOK_CLOSE_COMPLETE    0x002
#define NETOK_SEND_COMPLETE     0x003
#define NETOK_SEND_PENDING      0x004

#define NETERR_BADPARAMETER     0x100
#define NETERR_SEND_NORESOURCE  0x200
#define NETERR_SEND_BADPARAMETER 0x300
#define NETERR_SEND_TIMEOUT     0x400
#define NETERR_NOMEMORY         0x600
#define NETERR_FREE_ERROR       0x700
#define NETERR_BAD_BUFFER       0x800

/* Network option flags */
#define NETOPT_SEND_IMMEDIATE   1
#define NETOPT_NONBLOCK         2
#define NETOPT_BLOCKING         4
#define NETOPT_PRIORITY         8

/* Link flags (per-cabinet status) */
#define LINK_ACTIVE     0x80    /* Car is active and in use */
#define LINK_BLINK      0x40    /* Car should blink (joining) */
#define LINK_TLOCK      0x20    /* Track selection is locked */
#define LINK_CLOCK      0x10    /* Car selection is locked */
#define LINK_SOLO       0x08    /* Playing solo (no opponents) */
#define LINK_RACERX     0x04    /* Drone is Racer-X (special) */
#define LINK_DRONES     0x02    /* AI drones are active */
#define LINK_CATCHUP    0x01    /* Catchup mode enabled */

#define LINK_COM_FLAGS  0x7F    /* Flags transmitted over network */
#define LINK_ON_FLAGS   (LINK_DRONES | LINK_CATCHUP)
#define DEFAULT_LINK_FLAGS (LINK_DRONES | LINK_CATCHUP)

/* Network packet header size */
#define NET_PKT_HEADER_SIZE 14

/*================================ ENUMS =================================*/

/**
 * Network error codes
 */
typedef enum GNerr {
    GNerr_NetInitialize = 0,
    GNerr_OpenInterface,
    GNerr_GetBuffer,
    GNerr_FreeBuffer,
    GNerr_SendPacket,
    HIGHEST_GNERR
} GNerr;

/**
 * Network message types
 */
typedef enum MSGtype {
    MSG_LinkState = 0,      /* Link state update */
    MSG_PositionUpdate,     /* Position/velocity update */
    MSG_AppearanceUpdate    /* Car appearance change */
} MSGtype;

/**
 * Callback function types
 */
typedef enum NET_CB_FUNC {
    CB_Filter,      /* Filter incoming packet */
    CB_Receive,     /* Receive packet */
    CB_TXResult,    /* Transmit result */
    CB_Relinquish,  /* Busy wait callback */
    CB_BfrRlse,     /* Buffer release (unused) */
    CB_BfrAquire    /* Buffer acquire (unused) */
} Net_CB_Func;

/**
 * Callback result codes
 */
typedef enum NET_CB_RESULT {
    CB_OK = 1,          /* Success */
    CB_FAILED,          /* Error */
    CB_Drop_Packet,     /* Ignore packet */
    CB_Copy_Packet,     /* Accept packet */
    CB_Copy_Part,       /* Accept partial */
    CB_Extract_Part,    /* Extract portion */
    CB_Take_Ownership   /* Take buffer ownership */
} Net_CB_Result;

/*============================== TYPEDEFS ================================*/

/* Network types (arcade used 32-bit handles) */
typedef u32 network_descriptor;
typedef u32 packet_number;
typedef u8 netadd[6];   /* Ethernet MAC address */

/**
 * Checksum structure (64-bit for packet verification)
 */
typedef struct ckSum {
    union {
        u8  u8[8];
        u16 u16[4];
        u32 u32[2];
    } u;
} ckSum;

/**
 * Network buffer for packet data
 */
typedef struct Net_Buffer {
    struct Net_Buffer *next;    /* Next buffer in chain */
    struct Net_Buffer *self;    /* Self-reference for validation */
    u16 size;                   /* Buffer size */
    u16 len;                    /* Data length */
    u16 offset;                 /* Data offset */
    u16 flags;                  /* Allocation flags */
    u8 data[256];               /* Packet data */
} Net_Buffer;

/**
 * Network state data (transmitted between cabinets)
 */
typedef struct gnData {
    GState state;       /* Current game state */
    u8 track;           /* Selected track */
    u8 flags;           /* Link flags */
    s32 start_time;     /* Race start time */
    s32 play_time;      /* Remaining play time */
    u32 seed;           /* Random seed for synchronization */
} gnData;

/**
 * Full network state (per cabinet)
 */
typedef struct gnState {
    ckSum cksum;        /* Checksum of remote state copy */
    u32 alive;          /* Countdown to 0 = dead */
    u32 rcvtime;        /* Time of last packet received */
    u32 rcvcnt;         /* Packet receive count */
    s32 irqdiff;        /* IRQ time difference from remote */
    u32 seqid;          /* Sequence ID of last packet */
    u32 pktloss;        /* Lost packet count */
    gnData d;           /* Transmitted data */
} gnState;

/**
 * Link info structure (per connected cabinet)
 * Based on arcade dis.h LinkInfo
 */
typedef struct LinkInfo {
    u8 track;           /* Current track selection */
    u8 car;             /* Current car selection */
    u8 slot;            /* Pole position slot */
    u8 flags;           /* Link status flags */
    u8 owner;           /* Owner ID */
    s8 difficulty;      /* Difficulty setting */
    s8 marker_flag;     /* Markers enabled */
    s8 num_laps;        /* Number of laps */
    u32 join_time;      /* Time of last join message */
    u32 heartbeat;      /* Time of last any message */
    f32 version;        /* Protocol version */
} LinkInfo;

/*============================= PROTOTYPES ===============================*/

/* Network initialization and update */
void gnInit(void);              /* Initialize network system */
void gnUpdate(void);            /* Per-frame network update */

/* Network callbacks (arcade used for packet handling) */
s32 gnNetCallback(Net_CB_Func func, network_descriptor nd,
                  packet_number pkt_no, s32 plen, u32 p1, u32 p2, u32 p3);

/* Timer synchronization */
s32 gnGetCountdownTime(s32 node);   /* Get node's countdown time */

/* Checksum utilities */
ckSum ckSumGet(const void *ptr, u32 size);
ckSum ckSumProduct(ckSum a, ckSum b);

/* Inline checksum comparison */
#define ckSumEqual(a, b) \
    (((a).u.u32[0] == (b).u.u32[0]) && ((a).u.u32[1] == (b).u.u32[1]))

/* Network buffer management (stubs for N64) */
u32 Net_Initialize(u8 *pool, u32 size, void (*handler)(void), void *func);
network_descriptor Net_Open_Interface(char *device, void *cb, u32 caps,
                                      s32 sig_len, s32 sig_offset);
u32 Net_Close_Interface(network_descriptor nd);
u32 Net_Send_Packet(network_descriptor nd, const netadd dest,
                    Net_Buffer *pkt, s32 opts);
u32 Net_Get_Buffer(Net_Buffer **buf, s32 size, s32 opts);
u32 Net_Free_Buffer(Net_Buffer *buf, s32 opts);

/*============================== GLOBALS =================================*/

/* Network state for all cabinets */
extern gnState gn_state[MAX_LINKS];

/* Link info for all cabinets */
extern LinkInfo gLink[MAX_LINKS];

/* Error counters */
extern s32 GNerrcnt[HIGHEST_GNERR];

/* This cabinet's node ID (0-7) */
extern s32 gThisNode;

/* Network descriptor (handle to network interface) */
extern network_descriptor gNetDesc;

#endif /* NETWORK_H */
