/**
 * pdu.h - Protocol Data Unit definitions for Rush 2049 N64
 *
 * Based on arcade game/pdu.c and game/dis.h
 * Original DIS (Distributed Interactive Simulation) protocol handling.
 *
 * NOTE: N64 version does not have networking - these are stub
 * implementations for code compatibility with arcade source.
 *
 * Original arcade code by Time Warner Interactive (1996).
 * See: reference/repos/rushtherock/game/pdu.c
 *      reference/repos/rushtherock/game/dis.h
 */

#ifndef PDU_H
#define PDU_H

#include "types.h"

/* ========================== DEFINES ========================== */

/* PDU Protocol version */
#define DIS_VERSION         0x18    /* miniDIS v1, DIS v7 (little endian) */

/* Entity ID special values */
#define PDU_ALL_ENTITIES    0xFFFF  /* Refers to all entities */

/* PDU Error codes */
#define PDU_NO_ERROR        0       /* Everything's peachy */
#define PDU_TIMEOUT         (-1)    /* Driver timed out */
#define PDU_PARAM_ERROR     (-2)    /* Problem with a PDU parameter */
#define PDU_MEM_ERROR       (-3)    /* Couldn't allocate memory */
#define PDU_TRUNC_ERROR     (-4)    /* Buffer too small */
#define PDU_NO_PDU          (-5)    /* No more PDUs in the queue */
#define PDU_CANT_INIT       (-6)    /* Can't initialize network */
#define PDU_SEND_ERROR      (-7)    /* Error sending a packet */
#define PDU_NUCLEUS_ERROR   (-8)    /* Problem involving Nucleus */
#define PDU_ASSERT_FAILURE  (-9)    /* Assert failed */

/* PDU Appearance flags */
#define PDU_VISUAL_BIT      0x20    /* Visual appearance included */
#define PDU_AUDIO_BIT       0x02    /* Audio appearance included */

/* Link flags (for linked cabinet networking) */
#define LINK_ACTIVE         0x80    /* Car is active and used */
#define LINK_BLINK          0x40    /* Car should blink */
#define LINK_TLOCK          0x20    /* Track is locked */
#define LINK_CLOCK          0x10    /* Car is locked */
#define LINK_SOLO           0x08    /* Node is playing solo */
#define LINK_RACERX         0x04    /* Drone is racer-X */
#define LINK_DRONES         0x02    /* Drones active */
#define LINK_CATCHUP        0x01    /* Catchup mode desired/active */
#define LINK_COM_FLAGS      0x7F    /* Mask of flags to send across link */
#define LINK_ON_FLAGS       (LINK_DRONES | LINK_CATCHUP)
#define DEFAULT_LINK_FLAGS  (LINK_DRONES | LINK_CATCHUP)

/* Entity appearance visual field masks */
#define App_M_PAINT         0x00000001  /* Team paint (unused) */
#define App_M_DAMAGE        0x00000018  /* Generic damage (hulk/translucency) */
#define App_M_SMOKE         0x00000060  /* Engine smoke */
#define App_M_TRAILS        0x00000180  /* Trailing effects (L&R sparks) */
#define App_M_HATCH         0x00000C00  /* Hatch (bottom sparks) */
#define App_M_LIGHTS        0x00007000  /* Any lights */
#define App_M_HEAD_LIGHTS   0x00002000  /* Head lights (unused) */
#define App_M_BRAKE_LIGHTS  0x00001000  /* Brake lights */
#define App_M_FLAMES        0x00008000  /* Flames (unused) */
#define App_M_FROZEN        0x00200000  /* Entity frozen (unused) */
#define App_M_DEACTIVATED   0x00400000  /* Entity deactivated */

/* Car-specific appearance fields */
#define App_M_ANY_DAMAGE    0xFFC00000  /* Any damage to car */
#define App_M_LF_DAMAGE     0xC0000000  /* 4 levels, left-front */
#define App_M_RF_DAMAGE     0x30000000  /* 4 levels, right-front */
#define App_M_LR_DAMAGE     0x0C000000  /* 4 levels, left-rear */
#define App_M_RR_DAMAGE     0x03000000  /* 4 levels, right-rear */
#define App_M_TOP_DAMAGE    0x00C00000  /* 4 levels, top */
#define App_M_TIRE_SMOKE    0x000F0000  /* Any tire smoke */
#define App_M_LF_SMOKE      0x00080000  /* Left front smoke */
#define App_M_RF_SMOKE      0x00040000  /* Right front smoke */
#define App_M_LR_SMOKE      0x00020000  /* Left rear smoke */
#define App_M_RR_SMOKE      0x00010000  /* Right rear smoke */
#define App_M_SKID_MARK     0x00000606  /* Any tire skid marks */
#define App_M_LF_SKID       0x00000002  /* Left front skid */
#define App_M_RF_SKID       0x00000004  /* Right front skid */
#define App_M_LR_SKID       0x00000200  /* Left rear skid */
#define App_M_RR_SKID       0x00000400  /* Right rear skid */
#define App_M_SPARKS        0x00000980  /* Any spark */
#define App_M_LSPARK        0x00000100  /* Left side sparks */
#define App_M_RSPARK        0x00000080  /* Right side sparks */
#define App_M_BSPARK        0x00000800  /* Bottom sparks */
#define App_M_HULK          0x00000010  /* Car is a hulk */
#define App_M_TRANSLUCENT   0x00000008  /* Car is translucent */
#define App_M_UNUSED        0x0030E001  /* Unused bits */

/* Mirroring appearance bits (for left/right swap) */
#define App_SR2BITS (App_M_LF_DAMAGE | App_M_LR_DAMAGE)
#define App_SL2BITS (App_M_RF_DAMAGE | App_M_RR_DAMAGE)
#define App_SH2BITS (App_SR2BITS | App_SL2BITS)
#define App_SR1BITS (App_M_LF_SMOKE | App_M_LR_SMOKE | App_M_RF_SKID | App_M_RR_SKID | App_M_LSPARK)
#define App_SL1BITS (App_M_RF_SMOKE | App_M_RR_SMOKE | App_M_LF_SKID | App_M_LR_SKID | App_M_RSPARK)
#define App_SH1BITS (App_SR1BITS | App_SL1BITS)
#define App_SHBITS  (App_SH2BITS | App_SH1BITS)

/* Look field masks */
#define CAR_TYPE_MASK       0x0F    /* Car type */
#define CAR_TYPE_OFFSET     0       /* Shift for car type */
#define ENG_TYPE_MASK       0x07    /* Engine type */
#define ENG_TYPE_OFFSET     4       /* Shift for engine type */
#define CAR_DEAD_MASK       0x01    /* Car is dead */
#define CAR_DEAD_OFFSET     7       /* Shift for car dead */
#define COL_TYPE_MASK       0x01    /* Collidable bit */
#define COL_TYPE_OFFSET     8       /* Shift for collidable */
#define HIDE_CAR_MASK       0x01    /* Hide car bit */
#define HIDE_CAR_OFFSET     9       /* Shift for hide car */
#define ROADCODE_MASK       0xFFF   /* Road code data */
#define ROADCODE_OFFSET     10      /* Shift for road code */

/* Road code mirroring bits */
#define Look_LF_ROADCODE    (0xE00 << ROADCODE_OFFSET)
#define Look_RF_ROADCODE    (0x1C0 << ROADCODE_OFFSET)
#define Look_LR_ROADCODE    (0x038 << ROADCODE_OFFSET)
#define Look_RR_ROADCODE    (0x007 << ROADCODE_OFFSET)

/* Communication channels */
#define BROADCAST_COM_CHAN  ((u8)0xFF)
#define MULTICAST_COM_CHAN  ((u8)0xED)

/* Timing constants */
#define PDU_ONE_SEC         1000    /* One second in ms (arcade uses IRQTIME) */
#define NET_SEC             100000  /* Net timer ticks per second */
#define TRK_SELECT_TIME     (16 * PDU_ONE_SEC)  /* Track select timeout */
#define QCK_SELECT_TIME     (5 * PDU_ONE_SEC)   /* Quick select timeout */
#define CAR_SELECT_TIME     (16 * PDU_ONE_SEC)  /* Car select timeout */
#define COUNTDOWN_TIME      3       /* Countdown seconds */

/* Join modes */
#define JOIN_ANY            0       /* Normal tourney join */
#define JOIN_REMOTE         1       /* Tourney join on remote button */
#define JOIN_QUICK          2       /* Remote join with no pauses */

/* Maximum values */
#define MAX_NAME_LEN        9       /* Max chars in high score name */
#define MAX_ENTITIES        300     /* Maximum entities in game */
#define MAX_DATA_ITEMS      20      /* Max items in data PDU */
#define MAX_TOT_COLLISIONS  32      /* Max simultaneous collisions */
#define MAX_COLLISIONS      1       /* Max collisions per entity */
#define MAX_LINKS           12      /* Maximum linked cabinets (12 cars) */
#define PDU_NCARS           8       /* Number of car types */
#define PDU_NTRACKS         4       /* Number of tracks */
#define NULL_DATA           1       /* Placeholder for variable arrays */

/* ========================== ENUMS ========================== */

/**
 * PDU Types - Types of PDUs in the DIS protocol
 */
typedef enum PDUType {
    kOtherPDU = 0,
    kEntityStatePDU = 1,
    kFirePDU = 2,
    kDetonationPDU = 3,
    kCollisionPDU = 4,
    kServiceRequestPDU = 5,
    kResupplyOfferPDU = 6,
    kResupplyReceivedPDU = 7,
    kResupplyCanceled = 8,
    kRepairCompletePDU = 9,
    kRepairResponsePDU = 10,
    kCreateEntityPDU = 11,
    kRemoveEntityPDU = 12,
    kStartPDU = 13,
    kStopPDU = 14,
    kAcknowledgePDU = 15,
    kActionRequestPDU = 16,
    kActionResponsePDU = 17,
    kDataQueryPDU = 18,
    kSetDataPDU = 19,
    kDataPDU = 20,
    kEventReportPDU = 21,
    kMessagePDU = 22,
    kEMEmmisionPDU = 23,
    kDesignatorPDU = 24,
    kTransmitterPDU = 25,
    kSignalPDU = 26,
    kReceiverPDU = 27,
    kNewtonianCollisionPDU = 28,
    kEntityAppearancePDU = 29,
    kEnvironmentPDU = 30,
    NUM_PDUS
} PDUType;

/**
 * Acknowledge Flag - Values for Acknowledge PDU
 */
typedef enum AckFlag {
    AckCreate = 1,
    AckRemove = 2,
    AckStart = 3,
    AckStop = 4
} AckFlag;

/**
 * Acknowledge Response - Response codes for Acknowledge PDU
 */
typedef enum AckResponse {
    AckOther = 0,
    AckOK = 1,
    AckFail = 2
} AckResponse;

/**
 * Event Type ID - Event types for Event Report PDU
 */
typedef enum EventTypeID {
    /* Game startup messages */
    Event_CanPlay = 0x100,          /* (256) Node available to link */
    Event_CanJoin = 0x101,          /* (257) Node available with params */
    Event_TrackLocked = 0x102,      /* (258) Track locked */
    Event_CarLocked = 0x103,        /* (259) Initial race config */
    Event_Countdown = 0x104,        /* (260) Countdown seconds */
    Event_Timer = 0x105,            /* (261) Select timer value */
    Event_Negotiate_Track = 0x106,  /* (262) Track negotiation */
    Event_Negotiate_Race = 0x107,   /* (263) Race negotiation */

    /* In-game messages */
    Event_Highlight = 0x108,        /* (264) Highlight recording on/off */
    Event_CheckPoint = 0x109,       /* (265) Checkpoint passed */
    Event_GameOver = 0x10A,         /* (266) Player out of time */

    /* Any-time messages */
    Event_Heartbeat = 0x10B,        /* (267) Node alive */
    Event_Coin = 0x10C,             /* (268) Coin received */
    Event_Tourney = 0x10D,          /* (269) Tourney setup */
    Event_TourneyStart = 0x10E,     /* (270) Tourney started */

    /* Data messages */
    Event_EntityData = 0x10F,       /* (271) Entity data follows */
    Event_EntityIDReq = 0x110,      /* (272) Request entity ID allocation */
    Event_Stats = 0x111,            /* (273) Game statistics */

    /* Special messages using different PDU type */
    Message_MCast_Channel = 0x112,  /* (274) Multicast channel snifter */
    Message_BCast_Countdown = 0x113 /* (275) Initial countdown message */
} EventTypeID;

/**
 * Event Report Item - Data item types for Event Report PDU
 */
typedef enum EventReportItem {
    Report_Other = 0,           /* Unknown report */
    Report_Node = 1,            /* Node number (0-7) */
    Report_Track = 2,           /* Track number (0-2) */
    Report_Channel = 3,         /* Multicast address (0-63) */
    Report_CarType = 4,         /* Car type (0-7) */
    Report_RaceFlags = 5,       /* Race flag settings */
    Report_Count = 6,           /* Countdown seconds (0-10?) */
    Report_Highlight = 7,       /* Highlight start/end (Boolean) */
    Report_Score = 8,           /* Score value (0-?) */
    Report_ScoreFlags = 9,      /* Score flags (0=new, 1=synch) */
    Report_CheckPoint = 10,     /* Checkpoint number (0-?) */
    Report_Flags = 11,          /* Checkpoint flags */
    Report_SimID = 12,          /* Simulation ID (u32) */
    Report_Drones = 13,         /* Drone responsibilities */
    Report_PolePos = 14,        /* Pole position (0-7) */
    Report_Time = 15,           /* Time value */
    Report_Slot = 16,           /* Slot number (0-8) */
    Report_Difficulty = 17,     /* Difficulty factor (0-3) */
    Report_TourneyFlags = 18,   /* Tourney mode flags */
    Report_TourneyLaps = 19,    /* Tourney laps (0-99) */
    Report_State = 20,          /* Node state info (0-4) */
    Report_MarkerFlag = 21,     /* Marker flag (0-1) */
    Report_NumberLaps = 22,     /* Number of laps (2-3 or 0) */
    HIGHEST_REPORT_NUM          /* Keep last - error checking */
} EventReportItem;

/**
 * Node State - Network node states
 */
typedef enum NodeState {
    NODE_INACTIVE = 0,
    NODE_IDLE = 1,
    NODE_JOINING = 2,
    NODE_LOCKED = 3,
    NODE_BUSY = 4,
    NODE_NOCHANGE = 5,
    NUM_NODESTATES
} NodeState;

/* ========================== TYPEDEFS ========================== */

/* Float array types */
typedef f32 F32x3[3];
typedef f32 F32x6[6];
typedef f32 F32x3x3[3][3];
typedef f64 F64x3[3];

/* Network address type (6-byte MAC address) */
typedef u8 netadd[6];

/* Forward declarations */
struct MODELDAT;

/**
 * Entity ID - Unique identifier for network entities
 */
typedef struct EntityID {
    u16 site;           /* Site identifier */
    u16 entity;         /* Entity identifier */
} EntityID;

/**
 * Entity Type - Describes an entity type
 */
typedef struct EntityType {
    u16 kind;           /* Entity kind */
    u16 specific;       /* Specific type */
} EntityType;

/**
 * PDU Header - Common header for all PDUs
 */
typedef struct PDUHeader {
    u8  version;        /* Protocol version */
    u8  type;           /* PDU type */
    u16 length;         /* PDU length */
    u32 simID;          /* Simulation ID (0 for any) */
    u32 id;             /* Message ID number */
    u32 timeStamp;      /* Timestamp */
} PDUHeader;

/**
 * PDU - Base PDU structure
 */
typedef struct PDU {
    netadd dst;         /* Destination address */
    netadd src;         /* Source address */
    u8  junk[4];        /* Driver-used bytes */
    u8  data[NULL_DATA]; /* Start of PDU data */
} PDU;

/**
 * PDU Data Item - Fixed-length (32-bit) data item
 */
typedef struct PDUDataItem {
    u32 datumID;        /* Data ID (EventReportItem) */
    u32 datum;          /* Data value */
} PDUDataItem;

/**
 * PDU Variable Data Item - Variable-length data item
 */
typedef struct PDUVarDataItem {
    u32 datumID;        /* Data ID */
    u32 length;         /* Data length */
    u32 data[NULL_DATA]; /* Variable data */
} PDUVarDataItem;

/**
 * Entity State Data - Data for EntityState PDUs
 */
typedef struct EntityStateData {
    F32x3 pos;          /* Position */
    F32x3 vel;          /* Velocity */
    F32x3 acc;          /* Acceleration */
    F32x3 angv;         /* Angular velocity */
    f32   quat[4];      /* Quaternion orientation */
    s16   torque;       /* Engine torque */
    u16   rpm;          /* Engine RPM */
    f32   steerangle;   /* Tire angle */
    f32   suscomp[4];   /* Suspension compression */
    f32   tireV[4];     /* Tire angular velocity */
    u32   look;         /* Look field (engine type, etc.) */
    u32   appearance;   /* Visual appearance flags */
    s32   timeStamp;    /* Timestamp when created */
    f32   timeFudge;    /* Time correction factor */
    f32   shadow_h[4];  /* Shadow vertical offset */
    f32   shadow_v[4];  /* Shadow vertical velocity */
    s8    checkpoint;   /* Current checkpoint */
    s8    lap;          /* Current lap */
    s8    mpath_lap;    /* Maxpath version of lap */
    BOOL  fresh;        /* True if fresh from network */
} EntityStateData;

/**
 * Link Info - Information about a linked cabinet
 */
typedef struct LinkInfo {
    u8  track;          /* Current track selection */
    u8  car;            /* Current car selection */
    u8  slot;           /* Current pole position */
    u8  flags;          /* Drones, catchup, active, blink */
    u8  owner;          /* Owner node */
    s8  difficulty;     /* Difficulty setting */
    s8  marker_flag;    /* Marker flag (1=enabled) */
    s8  num_laps;       /* Number of laps for track */
    u32 join_time;      /* Local time of last CanJoin/CanPlay */
    u32 heartbeat;      /* Local time of last message */
    f32 version;        /* Version number of stack */
    NodeState state;    /* Node state */
    struct MODELDAT *model; /* Points to model */
} LinkInfo;

/**
 * Acknowledge PDU - Acknowledgement packet
 */
typedef struct AcknowledgePDU {
    PDUHeader header;
    EntityID receivingID;
    EntityID originID;
    AckFlag ackFlag;
    AckResponse responseFlag;
    u32 requestID;
} AcknowledgePDU;

/**
 * Entity Appearance PDU - Entity appearance update
 */
typedef struct EntityAppearancePDU {
    PDUHeader header;
    EntityID originID;
    u8  flag1;
    u8  flag2;
    u32 appearVis;
    u32 appearAudio;
} EntityAppearancePDU;

/**
 * Entity State PDU - Entity state update
 */
typedef struct EntityStatePDU {
    PDUHeader header;
    EntityID entityID;
    EntityType entityType;
    F32x3 velocity;
    F32x3 location;
    f32 quat[4];
    s16 torque;
    u16 rpm;
    u32 appearance;
    u32 look;
    f32 steerangle;
    f32 tireV[4];
    f32 suscomp[4];
    s32 timeStamp;
    f32 timeFudge;
    f32 shadow_h[4];
    f32 shadow_v[4];
    F32x3 accel;
    F32x3 angularV;
    s8 checkpoint;
    s8 lap;
    s8 mpath_lap;
    s8 dummy;
} EntityStatePDU;

/**
 * Event Report PDU - Event report packet
 */
typedef struct EventReportPDU {
    PDUHeader header;
    EntityID receivingID;
    EntityID originID;
    u32 eventType;
    u32 numFixedRecs;
    u32 numVarRecs;
    u8  data[NULL_DATA];
} EventReportPDU;

/**
 * Message PDU - General message packet (same as Event Report)
 */
typedef struct MessagePDU {
    PDUHeader header;
    EntityID receivingID;
    EntityID originID;
    u32 eventType;
    u32 numFixedRecs;
    u32 numVarRecs;
    u8  data[NULL_DATA];
} MessagePDU;

/* PDU handler function pointer type */
typedef s16 (*PDUHandler)(PDU *);

/* ========================== GLOBALS ========================== */

/* Damage field masks and shifts */
extern const u32 gDamageMask[5];
extern const u8 gDamageShift[5];

/* Communication addresses */
extern netadd gComAddress;      /* Multicast address to talk/listen */
extern netadd gBroadcast;       /* Broadcast channel */

/* Entity state */
extern EntityID gThisEntity;    /* Entity ID of this machine */
extern s32 gCountDown;          /* Time until game starts */
extern EntityStateData carz[MAX_LINKS]; /* Current positions */

/* Request tracking */
extern u32 gNextRequest;        /* Next request ID to use */
extern u8 gSimManager;          /* True if simulation manager */
extern u32 gSimID;              /* Current simulation ID */

/* Link state for all cabinets */
extern LinkInfo gLink[MAX_LINKS];
extern BOOL gTrackLocked;       /* True if game locked down */
extern BOOL gAllTracksLocked;   /* True if all players locked */
extern BOOL gUseCatchup;        /* True if using catchup mode */
extern BOOL gErrOverride;       /* Message suppression override */
extern BOOL gNetDelay;          /* Delay Ack timeouts */
extern BOOL solo_flag;          /* Only one human playing */
extern BOOL gJoinInPossible;    /* Game starting on net */
extern BOOL gTourneyStart;      /* External tourney start received */
extern BOOL gTourneyTrack;      /* External track selection */
extern u8 gFeedbackLevel;       /* Feedback level */

/* Game state tracking */
extern BOOL gIgnoreScore;       /* Don't enter score in HS table */
extern u32 gInThisGame;         /* Bits for nodes in this game */
extern u32 gLiveNodes;          /* Bits for nodes sending msgs */

/* Statistics */
extern u32 gPacketsSent;        /* Total packets sent */
extern u32 gPacketsReject;      /* Total packets rejected */
extern u32 gPacketsRcvd;        /* Total packets received */

/* ========================== FUNCTIONS ========================== */

/* Acknowledge PDU */
s16 PutAcknowledgePDU(netadd dest, u16 entID, u32 reqID, u32 type,
                       AckFlag ack_flag, AckResponse response);
s16 ParseAcknowledgePDU(PDU *pdu, u16 *entID, u32 *reqID, u16 *origin,
                        AckFlag *ack_flag, AckResponse *response);

/* Entity State PDU */
s16 PutEntityStatePDU(u16 entID);
s16 ParseEntityStatePDU(PDU *pdu, u16 *entID, EntityStateData *d);

/* Entity Appearance PDU */
s16 PutEntityAppearancePDU(u16 entID, u8 flag, u32 appearV, u32 appearA);
s16 ParseEntityAppearancePDU(PDU *pdu, u16 *entID, u8 *flag,
                              u32 *appearV, u32 *appearA);

/* Event Report PDU (variable args not supported on N64 - simplified) */
s16 PutEventReportPDU_Simple(EventTypeID event_type, u32 num_items,
                              PDUDataItem *items);
s16 ParseEventReportPDU(PDU *pdu, u16 *entID, EventTypeID *event_type,
                        u32 *num_items, PDUDataItem *item_list);

/* Utility functions */
void ReportPDUError(s16 PDUerr);
u32 GetTimeStamp(void);
u32 GetElapsedTime(void);

/* Network send (stub for N64) */
s16 SendPDU(PDU *pdu, u16 length, PDUType type, netadd dest, BOOL reliable);

/* Initialization */
void PDU_Init(void);

#endif /* PDU_H */
