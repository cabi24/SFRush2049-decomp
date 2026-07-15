# Arcade struct harvest

Source: `reference/repos/rushtherock/` (Rush The Rock arcade source). Text in
code blocks is copied verbatim; paths and line numbers refer to that tree.

## Main game state

Provenance: `game/game.h:104-117`.

```c
typedef enum GState
{
		ATTRACT,
		TRKSEL,
		CARSEL,
		PLAYGAME,
		ENDGAME,
		GAMEOVER,
		HISCORE,
		PREPLAY,
		PREPLAY2,
		COUNTDOWN,
		NUM_GAME_STATES
} GState;
```

Declaration provenance: `game/globals.h:142`.

```c
RUSHGLOBAL GState 	gstate;
```

`game/game.c:714-884` switches on this value in `game()` and calls
`attract`, `TrackSel`, `CarSel`, `preplay`, `CountDown`, `playgame`,
`do_endgame`, `do_gameover`, and `EnterHighScore`. All enum members are
portable game concepts; none is intrinsically arcade-hardware-specific.

## Per-car game-task state

Provenance: `game/modeldat.h:403-467`.

```c
typedef struct CAR_DATA
{
	/* dead reckoned, display coordinates*/
	F32 dr_pos[3];		/*dead reckoned position*/
	F32 dr_vel[3];		/*dead reckoned velocity*/
	F32 dr_uvs[3][3];	/*dead reckoned orientation*/
	F32 dr_tirepos[4][3];	/*dead reckoned tire position*/

	F32 dr_acc[3];		/*reckon base acceleration*/
#if 0
	F32		pos[3];			/* Position */
	F32		vel[3];			/* Velocity */
	F32		acc[3];			/* Acceleration */
	F32		angv[3];		/* Angular velocity */
	F32		uvs[3][3];		/* UV orientation array */
	uvect 	UV;
#endif
	F32 	RWV[3];
	F32     RWR[3];
#if 1
	F32		V[3];			/* Velocity in world coordinates. */
#endif
	F32	TIRERWR[4][3];	/* tire positions, real world coordinates */
	F32	TIRER[4][3];	/* tire positions, body coordinates */
	F32	tireW[4];	/* Rolling Velocity of each tire	(rad/sec)*/
	F32		mph;		/* Spedometer reading */
	S16		crashflag;	/* flag we've crashed */
	S16	 	rpm;
	S16		engine_type;	/* Engine type for sound model */
	S16		body_type;		/* Car body style. */
	U32		appearance;		/* Appearance of car (smoke, damage, etc) */
	BOOL	data_valid;		/* Set when data has been updated at least once */
	S8		place;			/* place/position in race (first, second etc.) */
	S8		place_locked;	/* has end-of-race place been locked? */
	U32		gameover_time;	/* flag to resend gameover time until net recieve verified. */
	U32		score;			/* player score when passes finish line */
	S32		objnum;			/* ZOID object number for this object. */
	F32		distance;		/* distance from start of race */
	S8		we_died;		/* if set, then this car should be resurrected */
	S8		just_crashed;	/* indicates that car is in its death throes */
	U32		crashtime;		/* time stamp for crash timer */
	Visual	visuals[NUM_VISUALS];	/* Visual effects to overlay on car. */
	S16		collidable;		/* true when drone/car is collidable */
	U32		collide_time;	/* time-stamp used when car in not collidable */
	S8		collide_count;	/* count down to next state */
	S8		collide_state;	/* state of trasparency */
	MPCTL	mpath;			/* game version of maxpath data */
	S32		fwd_progress_t; /* time stamp to determine when car needs help arrows */
	F32		last_distance;	/* previous forward progress marker */
	U16		sound_flags, shortcut_flags;
	S32		sound_flag_time;
	S32		scut_flag_time;
	S16		old_shortcut;	/* value of last shortcut section (-1 = none) */
	F32		shortcut_pos[3];/* entry point into shortcut section */
	S16		last_mpath;		/* last maxpath index before short cut */
	S16		weight_index;	/* slot to use for path weighting */
	S8		difficulty;		/* dynamicly assigned drone difficulty setting */
	F32		road_norm[4][3];/* road normals for shadow */
	S8		in_tunnel;		/* car is in a tunnel */
	S8		laps;			/* game version of current lap */
	S8		mpath_laps;		/* game maxpath version of lap */
	S8		syncmpath_laps;	/* maxpath laps should match this value eventually */
	S8		checkpoint;		/* game version of current checkpoint */
	S32		lap_sync_time;	/* duration of lap and maxpath lap num not matching */
} CAR_DATA;
```

Array declaration provenance: `game/globals.h:159`.

```c
RUSHGLOBAL CAR_DATA game_car[NCARS];/* array with car data that game task can use */
```

`game()` uses `body_type` and `score`; `preplay()` uses `objnum`; and
`playgame()` uses `we_died`, `place_locked`, and `body_type`
(`game/game.c:779,873-874,921,1041,1070,1171`). Physics, race-placement,
crash, sound, and maxpath fields remain useful to N64. `objnum` and the
`Visual` entries are coupled to the arcade ZOID/MBOX renderer and need N64
render-object equivalents. `gameover_time` and lap synchronization comments
reflect the linked-cabinet network, though an N64 multiplayer port may retain
analogous state.

The nested maxpath record is required to lay out `CAR_DATA`. Provenance:
`game/modeldat.h:387-400`.

```c
typedef struct mpctltyp
{
	S32 mpi;			/* index of current maxpath point */
	F32 xrel;			/* left/right offset to next point (in car reference) */
	F32 yrel;			/* forward/backward offset */
	F32 cyrel;			/* rotation to next point */
	F32 len;			/* distance to next point */
	F32 tgtspd;			/* interpolated speed required for current spot */
	F32 tgtpos[3];		/* direction to steer to get to target point */
	S32 interval_time;	/* time that car has been at current maxpath point */
	S16 new_mpi;		/* maxpath point to update to on next model slice */
	S16 mpath_index;	/* current maxpath table being used */
	S16 default_path;	/* default path that drone tries to stay on */
} MPCTL;
```

This is platform-neutral AI/path state used indirectly by all three loop
functions through `CAR_DATA`.

## Controls and input records

The arcade loop does not consume one aggregate switch/pot structure. It reads
global switch bitfields and scaled pot globals. Provenance:
`game/globals.h:143-147,213-215`.

```c
RUSHGLOBAL U32 		edges; 				/*	Bits set for 1 cycle when a switch changes to on. */
RUSHGLOBAL U32 		m_edges; 			/*	model's version of edges */
RUSHGLOBAL U32 		trailing_edges;		/*	Bits set for 1 cycle when a switch changes to off. */
RUSHGLOBAL U32 		levels;				/*	Current values of the switches. */
RUSHGLOBAL U32		m_levels;			/*	model's version of switches */
```

```c
RUSHGLOBAL S16	gGasRaw, gBrakeRaw, gClutchRaw, gWheelRaw;	/* Raw pot value. */
RUSHGLOBAL S16	gGasInt, gBrakeInt, gClutchInt, gWheelInt;	/* Integer scaled 0-0x1000 (wheel -0x1000-0x1000) */
RUSHGLOBAL F32	gGasVal, gBrakeVal, gClutchVal, gWheelVal;	/* Float Scaled 0.0-1.0, (wheel -1.0 - 1.0) */
```

`game()` calls `get_switches(Do_it)` and reads these values; `playgame()` and
button helpers consume `edges`; `preplay()` has abort/button paths. The switch
edge/level abstraction is portable. Potentiometer steering, gas, brake,
clutch, force-wheel output, coin/start/abort cabinet switches, and shifter
presence are arcade-hardware-specific and should map to N64 pad buttons/stick.

The physics-facing input records are in `game/modeldat.h:472-493`:

```c
typedef struct fastintyp {
	S32	lasttime;		/* time of last model iteration (microsec / 10) */
	F32	modeltime;		/* Model Iteration time (seconds) */
	S16	wheel;			/* Wheel position -1.0 to 1.0 */
	S16	swvel;			/* Steering wheel velocity */
	S16	modelrun;		/* Stall model if zero */
} fastintyp;
```

```c
typedef struct mainintyp {
	S16 	clutch;			/* Clutch position 0 to 1.0 */
	S16 	brake;			/* Brake position 0 to 1.0 */
#if !KILLMAININ
	S16 	throttle;		/* Throttle position 0 to 1.0 */
#endif
	S16 	gear;			/* Current gear 0=neutral */
	S16 	autotrans;		/* 1 if automatic trans, 0 manual */
	S16 	ignition;		/* 1 if ignition on, 0 ignition off, 2 end-game */

	S16 	startermotor;	/* Non zero if starter engaged */
	S16 	crashthreshold;	/* Crash threshold, tens of pounds */
} mainintyp;
```

They are used by the model/controls path reached from `game()` and
`playgame()`, rather than accessed directly by the state switch. Wheel
velocity/force-wheel semantics, ignition, starter motor, and physical clutch
are cabinet-specific; normalized steering, brake/throttle, gear, automatic
transmission, and model timing are portable.

## Countdown and race timing

There is no single countdown structure. The state is split among scalars,
network state, and per-track timing. `game/game.c:471` declares:

```c
extern U8			countdown_state;			/* 0=ready to send 1st msg, 1=sending 1st msg, 2=sending countdown msgs */
```

`game/game.c:610` also has the function-static `all_in_cdown`, while
`game/globals.h:210-211` supplies the clocks:

```c
RUSHGLOBAL VS32		IRQTIME;			/* Game millisecond counter. Reset in attract. */
RUSHGLOBAL VS32		gUpSeconds;		/* Elapsed seconds. Reset only at powerup. */
```

`game()` writes `countdown_state` and `all_in_cdown`; `CountDown()` consumes
the timer; `preplay()` initializes race state. `IRQTIME` is an arcade
interrupt clock and must be replaced with the N64 scheduler/frame clock.
`all_in_cdown`, `countdown_state`, and their network handshake are specifically
for linked cabinets; a single-console port needs simpler readiness state.

Per-track timing provenance: `game/checkpoint.h:101-109`.

```c
typedef struct			/* structure to hold data for each track */
{
	F32 start_time[8];	/* amount to give at start of race (8 difficulty levels) */
	F32 end_time[8];	/* target amount of time at end of race (8 levels) */
	S16 loop_chkpnt;	/* after passing last checkpoint in list, loop to this one */
	S16 finish_line;	/* checkpoint index of finish line */
	S16 before_finish;	/* checkpoint prior to finish line */
	S16 number_of_laps;	/* actual number of laps for this race */
} Track_Data;
```

`game()` reads `track_data[trackno].start_time[...]` in `PREPLAY2`; race and
checkpoint code used by `playgame()` consumes the remaining fields. This type
is platform-neutral.

## Small state-machine records

The `COUNTDOWN` case examines linked-cabinet readiness via these records.
Provenance: `game/gamenet.h:111-134`.

```c
typedef struct gnData
{
    GState state;            /* state of x-mitting stack                  */
    U8     track;            /* Which track is selected                   */
    U8     flags;            /* Flags from gLink[i].flags                 */
    S32    start_time;       /* set from start_time in select.c           */
    S32    play_time;        /* set from play_time  in select.c           */
    U32    seed;             /* When u need net-uniform pseudo-random #'s */
}
gnData;

typedef struct gnState
{
    ckSum cksum;             /* cksum for remote copy of gn_state[gThisNode] */
    U32  alive;              /* Counts down to 0 .. 0==dead, else alive      */
    U32  rcvtime;            /* Time last packet for this state was rcv'ed   */
    U32  rcvcnt;             /* Count of number of packets received          */
    S32  irqdiff;            /* How much bigger is IRQTIME than remote?      */
    U32  seqid;              /* Sequence id of last packet from this node    */
    U32  pktloss;            /* Count of how many packets lost from this node*/

    gnData d;                /* The part that is actually x-mitted           */
}
gnState;
```

`game()` uses `gn_state[i].d.state` and `.alive`; selection and countdown
network code maintains the rest. The entire transport/checksum/liveness layer
is arcade linked-cabinet-specific. The nested `GState`, track, start/play time,
and random seed remain useful concepts if N64 multiplayer synchronization is
implemented.

The `HISCORE` case passes scalar fields from `CAR_DATA`; the stored score
record is in `game/attract.h:191-199`:

```c
typedef struct HiScore
{
    U32     score;
    char    name[NLENGTH];
    S8      deaths;
    U8      mirror;      /* TRUE means race was done in mirror mode */
    S8      car;         /* High nibble contains node where score came from. */
}
HiScore;
```

It is used by `EnterHighScore()` reached from `game()`; score/name/mirror/car
are portable, while the node encoded in the high nibble is a linked-cabinet
artifact.

Tournament settings influence `ATTRACT`, `TRKSEL`, and `CARSEL`. Provenance:
`game/attract.h:201-216`.

```c
typedef struct Tourney
{
    BOOL free;       /* 0 == pay each time, 1 == free play.                 */
    S8   ext_start;  /* 0 == each player select,                            */
                     /* 1 == Use external switch to activate join-in,       */
			     /* 2 == Same as remote but remove most game delays.    */
    U32  cabinets;   /* 1 << node is set if 'node' is in tourney.           */
    S16  laps;       /* > 0, Number of laps; 0 == No limit; < 0, Normal.    */
    S8   track;      /* >= 0, Use given track; < 0, use external switch.    */
    S8   car;        /* >= 0, Use given car; < 0, any car can play.         */
    S8   tranny;     /* >= 0, Use given tranny; < 0, any tranny.            */
    S8   drones;     /* 1 == drones; 0 == No drones; < 0, Normal drones.    */
    S8   catchup;    /* 1 == catchup; 0 == No catchup; < 0, Normal catchup. */
    U32  sanity;     /* If != DEADBEEF, settings are invalid. */
}
Tourney;
```

`game()` references `gTourney` indirectly through `TourneyOn()` and the
selection/game-over paths. `free`, external start, cabinet mask, coin/pay
semantics, and remote tournament configuration are arcade-specific. Track,
car, transmission, laps, drones, catchup, and a validation marker are portable
configuration concepts.

No additional aggregate type is directly used by the `game()` cases for
`attract`, `TrackSel`, or `CarSel`: those functions maintain their own globals
and renderer objects. Pulling their full UI/MBOX structures into a minimal
game-loop context would add arcade rendering detail without improving the
state-machine type model.
