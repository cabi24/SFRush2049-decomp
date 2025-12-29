# Rush 2049 N64 Menu System Mapping

Analysis of the arcade menu and selection system from Rush The Rock source code, with mappings to N64 equivalents.

## Menu Flow Diagram

```
                    +-------------+
                    |   ATTRACT   |
                    | (attract.c) |
                    +------+------+
                           |
                           | Coin insert + Start
                           v
                    +-------------+
                    |   TRKSEL    |
                    | (select.c)  |
                    | TrackSel()  |
                    +------+------+
                           |
                           | Track locked (gas pedal)
                           v
                    +-------------+
                    |   CARSEL    |
                    | (select.c)  |
                    |  CarSel()   |
                    +------+------+
                           |
                           | Car locked (gas pedal)
                           v
                    +-------------+
                    | ClutchSel() |
                    | Trans Select|
                    +------+------+
                           |
                           | Transmission locked
                           v
                    +-------------+
                    |   PREPLAY   |
                    |  preplay()  |
                    +------+------+
                           |
                           v
                    +-------------+
                    |  COUNTDOWN  |
                    | CountDown() |
                    +------+------+
                           |
                           | 3..2..1..RUSH!
                           v
                    +-------------+
                    |  PLAYGAME   |
                    |  playgame() |
                    +------+------+
                           |
                           | Time expires
                           v
                    +-------------+
                    |   ENDGAME   |
                    |  endgame()  |
                    +------+------+
                           |
                           v
                    +-------------+
                    |  GAMEOVER   |
                    |  gameover() |
                    +------+------+
                           |
                           | High score?
                           v
                    +-------------+
                    |   HISCORE   |
                    |  hiscore()  |
                    +------+------+
                           |
                           +-----> back to ATTRACT
```

## Game State Machine (GState)

From `reference/repos/rushtherock/game/game.h`:

```c
typedef enum GState
{
    ATTRACT,     // 0 - Attract mode loop
    TRKSEL,      // 1 - Track selection
    CARSEL,      // 2 - Car selection
    PLAYGAME,    // 3 - Active racing
    ENDGAME,     // 4 - Race ended
    GAMEOVER,    // 5 - Game over screen
    HISCORE,     // 6 - High score entry
    PREPLAY,     // 7 - Pre-race initialization
    PREPLAY2,    // 8 - Final pre-race setup
    COUNTDOWN,   // 9 - 3-2-1-RUSH countdown
    NUM_GAME_STATES
} GState;
```

**Global**: `gstate` - current game state (from `globals.h`)

## Arcade Menu Functions

### Main State Dispatch (game.c:714-880)

```c
switch (gstate) {
    case ATTRACT:   attract();    break;
    case TRKSEL:    TrackSel();   break;
    case CARSEL:    CarSel();     break;
    case PREPLAY:   preplay();    break;
    case COUNTDOWN: CountDown();  break;
    case PREPLAY2:  /* final setup */ break;
    case PLAYGAME:  playgame();   break;
    case ENDGAME:   endgame();    break;
    case GAMEOVER:  gameover();   break;
    case HISCORE:   hiscore();    break;
}
```

### Track Selection Flow (select.c)

| Function | Purpose | Location |
|----------|---------|----------|
| `InitSelect()` | Initialize select mode, load models | select.c:711 |
| `InitTrkSel()` | Reset track selection globals | select.c:773 |
| `ShowTrackSelect()` | Show/hide track select UI | select.c:789 |
| `TrackSelForce()` | Steering wheel force feedback | select.c:831 |
| `TrackSel()` | Main track selection logic | select.c:891 |
| `track_negotiation()` | Network negotiation for linked play | select.c:1171 |

**Track Select UI Elements** (TrackMultiBlit):
- PANEL00-03: Background panels
- SWHEEL01: Steering wheel display
- TRKPIC01: Track map preview
- TRKNUMBER1: Track number display
- CARICON: Car icons for linked players
- LINKLIGHT: Link status indicator
- SOLO: Solo mode indicator

**Track Selection Input**:
- Steering wheel: Select track (divided into sections)
- Gas pedal: Lock in selection
- SW_ABORT: Alternative lock-in
- SW_REVERSE: Toggle solo/linked mode
- SW_VIEW1: Toggle mirror mode

### Car Selection Flow (select.c)

| Function | Purpose | Location |
|----------|---------|----------|
| `init_car_select()` | Initialize car select state | select.c:1355 |
| `create_cars()` | Create 3D car objects on pads | select.c:1383 |
| `ShowCarSelect()` | Show/hide car select UI | select.c:2494 |
| `CarSelForce()` | Steering wheel force for car select | select.c:2532 |
| `CarSel()` | Main car selection logic | select.c:2560 |

**Car Select UI Elements** (CarMultiBlit):
- SPEEDCON00-01: Speed control display
- COLORBAR01-16: Car color selection bars
- AUTOTRANS/MANUAL: Transmission selection
- DRONES: Drone toggle indicator
- FEEDREG: Force feedback level
- DANGER: Danger car warning

**Car Selection Input**:
- Steering wheel: Select car (4 positions)
- Gas pedal: Lock in selection
- SW_VIEW1: Toggle car set (additional cars)
- SW_VIEW2: Toggle drones on/off
- SW_VIEW3: Toggle force feedback level

**Key Constants**:
```c
#define NPADS           4       // Number of car pads
#define WHEEL_SLICE     (WHEEL_RANGE / NPADS)
#define GAS_TRIGGER     (2 * (0x1000) / 3)
#define CAR_DELAY_TIME  (ONE_SEC)   // Delay before car lock allowed
#define CAR_SELECT_TIME (20 * ONE_SEC)  // Timer for car selection
```

### Transmission Selection (ClutchSel)

| Function | Purpose | Location |
|----------|---------|----------|
| `ClutchSel()` | Transmission type selection | select.c:2736 |
| `race_negotiation()` | Network negotiation for race start | select.c:2848 |

**Transmission Options**:
- Auto (autotrans = true)
- Manual (autotrans = false, requires shifter)

**Input**:
- Steering wheel: Left = Auto, Right = Manual
- Gas pedal: Lock in selection
- Timer: Auto-locks after CLUTCH_SEL_TIME (11 seconds)

### Attract Mode (attract.c)

| Function | Purpose | Location |
|----------|---------|----------|
| `InitAttract()` | Initialize attract mode | attract.c:777 |
| `ShowAttract()` | Display attract screen | attract.c:838 |
| `AttractForce()` | Steering wheel force in attract | attract.c:900 |
| `attract()` | Main attract loop | attract.c:933 |

**Attract Mode Screens** (AttractMode enum):
```c
typedef enum AttractMode {
    ATR_HSSETUP=-1,   // High score setup
    ATR_HSENTRY=0,    // High score entry
    ATR_HISCORE1-7,   // High score displays
    ATR_ATARILOGO,    // Atari logo movie
    ATR_MOVIE2-5,     // Various movies
    ATR_TEAM,         // Team credits
    ATR_CREDITS,      // Credits screen
    ATR_3DFXLOGO,     // 3dfx logo
    ATR_ADVISORY,     // Legal advisory
    ATR_DEMO,         // Demo gameplay
    ATR_TOURNEY,      // Tournament setup
    ATR_TRANSIT,      // Transition screen
    ATR_JOIN,         // Join now screen
    ATR_GAMESTAT,     // Game statistics
    NUM_ATTRACTS
} AttractMode;
```

**Attract Mode Flow**:
1. Cycles through screens based on timers
2. Checks for coin insert and game start
3. Displays "JOIN NOW" when linked game in progress
4. Handles tournament mode setup

## Options/Settings

Options are handled through operator menus in GUTS/os, not game code directly.

**In-Game Options** (selected during car select):
- `drones_flag`: 0=off, 1=on, 2=Racer X
- `catchup_flag`: 0=off, 1=on
- `gFeedbackLevel`: Force feedback level (0-2)
- `autotrans`: Automatic transmission toggle
- `solo_flag`: Solo mode (no network opponents)

**Global Variables** (globals.h):
```c
RUSHGLOBAL GState gstate;           // Current game state
RUSHGLOBAL BOOL   gTrackLocked;     // Track selection locked
RUSHGLOBAL BOOL   gCarLocked;       // Car selection locked
RUSHGLOBAL BOOL   gClutchLocked;    // Transmission locked
RUSHGLOBAL S16    trackno;          // Selected track number
RUSHGLOBAL S16    gThisCar;         // Selected car type
RUSHGLOBAL BOOL   autotrans;        // Auto/manual transmission
RUSHGLOBAL U8     solo_flag;        // Solo mode flag
```

## N64 Equivalents (src/game/game.c)

### N64 Menu State Variables

```c
extern s32 menu_selection;      // Current menu item
extern s32 menu_item_count;     // Total menu items
extern s32 menu_items_ptr;      // Pointer to menu items
extern s32 menu_stack_depth;    // Menu navigation depth
extern s32 menu_enabled;        // Menu active flag
extern void *menu_callback_array[];  // Menu callbacks
extern s32 cursor_anim_frame;   // Cursor animation
extern s32 ui_cursor_state;     // UI cursor state
extern s32 ui_cursor_frame;     // UI cursor frame
```

### N64 State Machine

```c
extern u32 gstate_mask;   // Current state bitmask
extern u32 gstate_prev;   // Previous/next state
extern s8  gstate_byte;   // Init flag

// State handlers
extern void state_init_handler(void);   // GSTATE_INIT
extern void state_setup_handler(void);  // GSTATE_SETUP
extern void state_menu_handler(void);   // GSTATE_MENU
extern void process_inputs(void);       // Input processing
extern void state_dispatch(void);       // State dispatcher (game_pause)
```

### N64 Game Loop (game_loop @ 0x800FD464)

The N64 uses a bitmask-based state system rather than simple enum:
- `gstate_mask` contains current state flags
- `gstate_prev` contains pending state
- State transitions happen when these differ

**Key State Bits** (estimated from code analysis):
```c
0x00000001 - Init state
0x00000002 - Setup state
0x00000004 - Menu state
0x00000008 - Game active
0x00000010 - Pause state
...
0x03FC0000 - High score states
0x40000000 - Attract mode bit
0x80000000 - Special flag
```

## Transition Animations

### Arcade Transitions (select.c)

**Track Select -> Car Select**:
```c
void init_car_select(void) {
    gstate = CARSEL;
    SetCountdownTimer(CAR_SELECT_TIME);
    ShowTrackSelect(false);      // Hide track select
    gCameraTransition = true;    // Enable camera transition
    ShowCarSelect(true);         // Show car select
}
```

**Car Select -> Preplay**:
```c
if (gstate != PREPLAY) {
    gstate = PREPLAY;
    ShowCarSelect(false);
    ShowTransit(true, 2);        // Show transition screen
}
```

**Transit Blit System**:
```c
static MULTIBLIT const TransitMultiBlit[] = {
    {"BLACK2", 0, 0, MUNUSED, 0x00, AnimateTransit, 0},
};
```

The `AnimateTransit()` function handles fade effects between screens.

### Camera Transitions

```c
// Car select camera positions
S16 gCurCam, gNextCam, gCamFrames;
BOOL gCameraTransition;

void SelectCam(void);  // Handles camera movement during select screens
```

## Input Handling for Menus

### Arcade Input (controls.c reference)

```c
// Button edge detection
extern U32 edges;           // Rising edges (newly pressed)
extern U32 trailing_edges;  // Falling edges (just released)
extern U32 levels;          // Current button states

// Button defines
#define SW_ABORT    (1<<0)  // Abort/start button
#define SW_RADIO    (1<<1)  // Music button
#define SW_VIEW1    (1<<2)  // View 1 button
#define SW_VIEW2    (1<<3)  // View 2 button
#define SW_VIEW3    (1<<4)  // View 3 button
#define SW_REVERSE  (1<<5)  // Reverse toggle
#define SW_GEAR_1-4 ...     // Shifter positions

// Analog inputs
GasPressed(bool check_release);  // Gas pedal state
WheelSection(int sections, int center);  // Wheel position
RawWheelSection(int sections, int center);  // Raw wheel
```

### N64 Input (from game.c)

```c
extern void process_inputs(void);  // Main input processor
extern s32 text_cursor_pos;        // Text input cursor
```

## N64 Function Mapping

| Arcade Function | N64 Equivalent | Address | Confidence |
|-----------------|----------------|---------|------------|
| game() | game_loop | 0x800FD464 | High |
| attract() | func_800DB81C | - | Medium |
| TrackSel() | state_menu_handler | 0x800FC0EC | Medium |
| CarSel() | state_menu_handler | 0x800FC0EC | Medium |
| preplay() | state_setup_handler | - | Medium |
| playgame() | func_800CA3B4 | - | High |
| CountDown() | func_800FBC30 | - | High |
| hiscore() | func_800FBF88 | - | Medium |
| process_inputs() | func_800C997C | - | High |

## Key Differences: Arcade vs N64

1. **State System**: Arcade uses simple enum, N64 uses bitmask
2. **Controls**: Arcade has steering wheel + pedals, N64 uses controller
3. **Network**: Arcade has 8-way linking, N64 has split-screen multiplayer
4. **UI**: Arcade has 2D blits, N64 has 3D/2D hybrid
5. **Tournament**: Arcade has extensive tournament mode, N64 simplified

## Files Referenced

| Arcade File | Purpose | Lines |
|-------------|---------|-------|
| game/game.c | Main game loop | ~1,700 |
| game/game.h | State enum | ~120 |
| game/select.c | Track/car selection | ~3,000 |
| game/sselect.c | Alternate select (older) | ~2,600 |
| game/attract.c | Attract mode | ~1,900 |
| game/attract.h | Attract enums | ~220 |
| game/globals.h | Global variables | ~200 |

## N64 Files to Compare

| N64 File | Purpose | Arcade Equivalent |
|----------|---------|-------------------|
| src/game/game.c | Game logic | game/game.c |
| src/game/menu.c | Menu system | game/select.c (partial) |
| src/game/carsel.c | Car selection | game/select.c:CarSel |
| src/game/select.c | Track selection | game/select.c:TrackSel |
| src/game/attract.c | Attract mode | game/attract.c |
| src/game/hiscore.c | High scores | game/hiscore.c |
