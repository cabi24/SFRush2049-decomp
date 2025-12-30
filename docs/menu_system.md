# Rush 2049 N64 - Menu and UI System Documentation

This document provides comprehensive documentation of the menu and UI system in the Rush 2049 N64 decompilation project.

## Table of Contents

1. [Overview](#overview)
2. [Menu State Machine](#menu-state-machine)
3. [Screen Types and IDs](#screen-types-and-ids)
4. [Input Handling](#input-handling)
5. [HUD Elements](#hud-elements)
6. [Text and Font Rendering](#text-and-font-rendering)
7. [Arcade Source Mapping](#arcade-source-mapping)
8. [Key Data Structures](#key-data-structures)
9. [Function Reference](#function-reference)

---

## Overview

The Rush 2049 N64 menu and UI system is implemented primarily in `src/game/game.c` with approximately 40+ menu-related functions. The system handles:

- Main menu navigation
- Track and car selection
- Options/settings screens
- In-game HUD (speedometer, timer, position, minimap)
- Pause menu
- High score entry
- Dialog boxes and confirmations

The implementation follows patterns from the arcade source code (`reference/repos/rushtherock/game/`) but is adapted for N64 hardware constraints.

---

## Menu State Machine

### Game States (gstate)

The game uses a global state machine (`gstate` at address `0x801146EC`) that controls the overall game flow:

```c
enum GState {
    GSTATE_ATTRACT   = 0,   // Attract mode / title screen
    GSTATE_TRKSEL    = 1,   // Track selection
    GSTATE_CARSEL    = 2,   // Car selection
    GSTATE_PREPLAY   = 3,   // Pre-race setup (once per game)
    GSTATE_COUNTDOWN = 4,   // Race countdown
    GSTATE_PREPLAY2  = 5,   // Final pre-race setup
    GSTATE_PLAYGAME  = 6,   // Active racing
    GSTATE_ENDGAME   = 7,   // Race finished
    GSTATE_GAMEOVER  = 8,   // Game over screen
    GSTATE_HISCORE   = 9,   // High score entry
    GSTATE_MENU      = 10,  // Options menu (N64-specific)
};
```

### Menu State IDs

Within the menu system, there are additional sub-states for different screens:

| Menu ID | Screen | Item Count | Description |
|---------|--------|------------|-------------|
| 0 | Main Menu | 5 | RACE, TIME TRIAL, STUNT MODE, BATTLE, OPTIONS |
| 1 | Race Track Select | 8 | Track selection for Race mode |
| 2 | Time Trial Track Select | 8 | Track selection for Time Trial |
| 3 | Stunt Track Select | 6 | Track/arena selection for Stunt mode |
| 4 | Battle Arena Select | varies | Arena selection for Battle mode |
| 5 | Save/Load Menu | 4 | Controller Pak operations |
| 6 | Sound Test | varies | Debug sound test |
| 7 | Credits | N/A | Credits scroll |
| 10 | Options Menu | 6 | Audio, Video, Controls, Save, Load, Defaults |
| 11 | Video Settings | 4 | Widescreen, HUD, Minimap, Back |
| 12 | Control Settings | 6 | Vibration, Sensitivity, Remap, etc. |
| 13 | Controller Remap | 8 | Button remapping screen |
| 20 | Car Select | 12 | Car selection screen |

### State Transitions

```
ATTRACT -> TRKSEL (on Start button)
TRKSEL -> CARSEL (track confirmed)
CARSEL -> PREPLAY (car confirmed)
PREPLAY -> COUNTDOWN (initialization complete)
COUNTDOWN -> PREPLAY2 (all players ready)
PREPLAY2 -> PLAYGAME (countdown finished)
PLAYGAME -> ENDGAME (race finished or timeout)
ENDGAME -> GAMEOVER (results shown)
GAMEOVER -> HISCORE (if new high score)
GAMEOVER/HISCORE -> ATTRACT (return to title)
```

### Menu Stack

The menu system uses a stack for navigation history:

```c
extern void *menu_stack[];        // Stack of previous menu IDs
extern s32 menu_stack_depth;      // Current stack depth (max 10)
```

Functions:
- `menu_switch(s32 toMenuId)` - Push current menu and switch to new one
- `menu_back()` - Pop menu from stack and return to previous

---

## Screen Types and IDs

### Menu Item Types

Each menu item has a type that determines its behavior:

| Type | Name | Behavior |
|------|------|----------|
| 0 | Action Button | Triggers an action when selected |
| 1 | Toggle | Flips between ON/OFF |
| 2 | Slider | Adjustable value (0-100) via Left/Right |
| 3 | Submenu | Opens another menu screen |
| 4 | Multi-choice | Cycles through multiple options |
| 5 | Callback | Calls a function pointer |

### Menu Item Structure

```c
struct MenuItem {
    s32 action;      // Action ID or target menu
    s32 value;       // Current value (for toggles/sliders)
    s32 type;        // Item type (0-5)
    s32 flags;       // Bit 0: disabled, upper bits: max value for multi-choice
};
```

### UI Element IDs

The `draw_ui_element()` function uses element IDs for different sprites:

| ID Range | Purpose |
|----------|---------|
| 0-9 | HUD backgrounds (speedometer, tach, etc.) |
| 10-15 | Progress bars and indicators |
| 16-25 | Digit sprites for numbers |
| 26-35 | Additional HUD elements |
| 32 | Car shadow sprite |
| 33-34 | Light sprites (headlights) |
| 50 | Menu cursor highlight |
| 51 | Arrow icon (for submenus) |
| 52-53 | Slider background and fill |
| 54-55 | Left/Right arrows for multi-choice |
| 56 | Selection cursor |
| 57 | Screen darken overlay |
| 58 | Dialog box background |
| 59 | Dialog border |
| 60 | Button highlight |
| 64-127 | Font characters (0-63 mapped to ASCII subset) |
| 128-143 | Speedometer needle sprites (16 angles) |
| 144 | Minimap player dot |

---

## Input Handling

### Button Mapping

N64 controller buttons mapped to menu actions:

| Button | Hex Mask | Menu Action |
|--------|----------|-------------|
| A | 0x8000 | Select / Confirm |
| B | 0x4000 | Back / Cancel |
| Start | 0x1000 | Pause / Confirm |
| D-Up | 0x0800 | Navigate Up |
| D-Down | 0x0400 | Navigate Down |
| D-Left | 0x0200 | Navigate Left / Decrease Value |
| D-Right | 0x0100 | Navigate Right / Increase Value |
| L Trigger | 0x0020 | Page Up |
| R Trigger | 0x0010 | Page Down |

### Input Processing Functions

```c
// Primary menu input handler
s32 menu_process_input(s32 input);
// Returns: 1=Select, 2=Back, 3=Start, 4=Up, 5=Down, 6=Left, 7=Right, 8=L, 9=R

// Cursor navigation (skips disabled items)
void menu_cursor_move(s32 direction);  // 4=Up, 5=Down, 6=Left, 7=Right

// Item selection action
void menu_item_select(s32 itemIndex);
```

### Input Repeat

The system implements auto-repeat for held buttons:

- Initial delay: 15 frames before first repeat
- Repeat rate: Every 10 frames for Left/Right (sliders)
- Repeat rate: Every 15 frames for Up/Down (navigation)

Key variables:
```c
extern s32 repeat_counter;       // Frame counter for Up/Down repeat
extern s32 input_repeat_delay;   // Frame counter for Left/Right repeat
extern s32 buttons_state;        // Previous frame button state
```

---

## HUD Elements

### HUD Components During Gameplay

| Element | Position | Description |
|---------|----------|-------------|
| Speedometer | Bottom-Center | Analog dial with rotating needle |
| Digital Speed | Below speedometer | Numeric MPH/KPH display |
| Tachometer | Bottom-Center (optional) | Engine RPM gauge |
| Lap Counter | Top-Right | "LAP X/Y" display |
| Position | Right side | "1st", "2nd", etc. |
| Timer | Top-Left | MM:SS.CC race time |
| Minimap | Top-Right | Track overview with player dots |
| Nitro Meter | Bottom-Left | Boost gauge fill |
| Wrong Way | Center | Flashing arrow when going backwards |
| Messages | Center | "GO!", "FINAL LAP!", etc. |

### HUD Functions

```c
void hud_render(void);              // Main HUD render (called every frame)
void hud_element_render(void *hud, s32 elementId);  // Render single element
void hud_speed_display(void *hud, f32 speed);       // Speedometer update
void hud_setup(s32 a, s32 b, s32 c, s32 d, s32 e, f32 f, f32 g, s32 h);  // HUD init
void hud_full_update(void);         // Update all HUD elements
void hud_fade_effect(void);         // Fade in/out transitions
```

### HUD Messages

Pre-defined message strings displayed at screen center:

```c
char *hudMessages[] = {
    "GO!",
    "FINAL LAP",
    "WRONG WAY",
    "3",
    "2",
    "1",
    "SET",
    "FINISH!",
    "NEW RECORD!"
};
```

### Speedometer Implementation

The speedometer uses a sprite-based needle system:

1. 16 pre-rendered needle sprites at different angles (elements 128-143)
2. Speed (0-200 MPH) maps to angle (0-180 degrees)
3. Needle sprite selected: `needleElement = 128 + (speedMph * 180 / 200) / 12`
4. Digital readout rendered below using number sprites

---

## Text and Font Rendering

### Font System

The game uses a bitmap font stored in a sprite atlas:

- Characters stored in element IDs 64-127
- Character width stored in `font_char_width` variable
- Supports: A-Z, 0-9, and symbols (: . - / !)

### Character Mapping

```c
Character -> Font Index Mapping:
'0'-'9' -> 0-9
'A'-'Z' -> 10-35
'a'-'z' -> 10-35 (lowercase = uppercase)
':'     -> 36
'.'     -> 37
'-'     -> 38
'/'     -> 39
'!'     -> 40
' '     -> (skip, advance only)
```

### Text Rendering Functions

```c
// Draw text string at position with color
void draw_text(s32 x, s32 y, char *text, u32 color);

// Draw numeric value with leading zeros
void draw_number(s32 value, s32 digits, s32 x, s32 y);

// Draw centered text
void draw_message(s32 messageId);

// Set text color (RGBA)
void text_color_set(u8 r, u8 g, u8 b, u8 a);

// Calculate text width for centering
s32 text_width(char *str);
```

### On-Screen Keyboard

For high score name entry, an on-screen keyboard is displayed:

```c
// Keyboard layout rows
keyboard_row_0 = "ABCDEFG"
keyboard_row_1 = "HIJKLMN"
keyboard_row_2 = "OPQRSTU"
keyboard_row_3 = "VWXYZ01"
keyboard_row_4 = "2345678"
keyboard_row_5 = "9 < OK "  // < = backspace

// Keyboard navigation
extern s32 keyboard_col, keyboard_row;   // Current position
extern s32 keyboard_visible;              // Keyboard active flag
extern s32 text_cursor_pos;               // Text buffer cursor
extern void *text_input_buffer[];         // Input buffer
```

---

## Arcade Source Mapping

### Equivalent Files

| N64 Code | Arcade Source | Notes |
|----------|---------------|-------|
| `src/game/game.c` (menu_*) | `game/menus.c` | Debug menus primarily |
| `src/game/game.c` (hud_*) | `game/hud.c` | HUD rendering |
| `src/game/game.c` (menu_track_select) | `game/select.c`, `game/sselect.c` | Track/car selection |
| `src/game/hiscore.c` | `game/hiscore.c` | High score system |
| `src/game/attract.c` | `game/attract.c` | Attract mode |

### Key Arcade Structures

From `game/hud.c`:
```c
typedef struct BlitList {
    char Name[10];
    TexInfo *ti;
    S32 TexIndex;
} BlitList;

static MULTIBLIT const HudMultiBlit[] = {
    {"MARKERS", 0, 0, MUNUSED, 0x00, AnimateMarker, 0},
    {"NEWTACH15L", SCR_MH-69, SCR_T-7, MUNUSED, 0x00, AnimateTach, 0},
    // ... more HUD elements
};
```

### State Machine Comparison

| Arcade State | Arcade Function | N64 Equivalent |
|--------------|-----------------|----------------|
| ATTRACT | `attract()` | `attract_handler()` |
| TRKSEL | `TrackSel()` | `menu_track_select()` |
| CARSEL | `CarSel()` | `menu_car_select()` |
| PREPLAY | `preplay()` | `state_init_handler()` |
| COUNTDOWN | `CountDown()` | `countdown_handler()` |
| PLAYGAME | `playgame()` | `game_mode_handler()` |
| ENDGAME | `do_endgame()` | `state_finalize()` |
| GAMEOVER | `do_gameover()` | `gameover_handler()` |
| HISCORE | `EnterHighScore()` | `hiscore_handler()` |

---

## Key Data Structures

### Menu State Variables

```c
// Current menu state
extern s32 current_menu_id;       // Active menu screen ID
extern s32 target_menu_id;        // Target menu during transition
extern s32 menu_selection;        // Currently selected item index
extern s32 menu_item_count;       // Number of items in current menu
extern s32 menu_items_ptr;        // Pointer to item array
extern s32 menu_wrap_around;      // 1 = cursor wraps, 0 = stops at edges
extern s32 menu_visible_flag;     // 1 = menu is visible

// Animation/transition state
extern s32 transition_state;      // 0=none, 1=out, 2=in
extern s32 transition_anim_frame; // Current animation frame
extern s32 transition_alpha;      // Fade alpha (0-255)
extern s32 cursor_anim_frame;     // Cursor blink frame
extern void *menu_item_alpha[];   // Per-item alpha for animations
extern s32 menu_target_y;         // Target Y position for highlight
```

### Pause Menu State

```c
extern s32 pause_menu_selection;  // Selected pause menu item
extern s32 pause_submenu_mode;    // 0=main, 1=options, 2=controls
extern s32 game_pause_state;      // 0=not paused, 1=paused
extern s32 pause_start_frame;     // Frame when pause started
```

### Dialog State

```c
extern s32 confirm_dialog_active; // Dialog box visible
extern s32 confirm_dialog_anim;   // Dialog animation frame
extern s32 confirm_dialog_selection; // YES/NO selection
extern s32 save_load_dialog;      // 0=none, 1=save, 2=load, 3=error
```

---

## Function Reference

### Menu System Core

| Function | Size | Description |
|----------|------|-------------|
| `menu_system_update()` | 12544 bytes | Main menu logic processor |
| `menu_render()` | 5748 bytes | Menu UI rendering |
| `menu_input_handle()` | 3040 bytes | Process controller input |
| `menu_switch()` | 340 bytes | Switch to new menu |
| `menu_back()` | 132 bytes | Return to previous menu |
| `menu_cursor_move()` | 516 bytes | Navigate between items |
| `menu_item_select()` | 564 bytes | Handle item selection |
| `menu_animation_update()` | varies | Update menu animations |
| `menu_scroll()` | 1284 bytes | Handle menu scrolling |
| `menu_transition()` | 1824 bytes | Animate menu transitions |

### Selection Screens

| Function | Size | Description |
|----------|------|-------------|
| `menu_track_select()` | 5196 bytes | Track selection screen |
| `menu_car_select()` | 1228 bytes | Car selection screen |
| `menu_options_screen()` | 2532 bytes | Options menu |
| `menu_audio_settings()` | 1364 bytes | Audio options |
| `menu_video_settings()` | 228 bytes | Video options |
| `menu_control_settings()` | 808 bytes | Control options |
| `menu_controller_remap()` | 1976 bytes | Button remapping |

### HUD Rendering

| Function | Size | Description |
|----------|------|-------------|
| `hud_render()` | 2468 bytes | Main HUD render loop |
| `hud_element_render()` | 804 bytes | Render single HUD element |
| `hud_speed_display()` | 868 bytes | Speedometer display |
| `hud_setup()` | 1048 bytes | HUD initialization |
| `hud_full_update()` | 1824 bytes | Update all HUD elements |
| `draw_speedometer()` | 1724 bytes | Speedometer dial |
| `draw_lap_counter()` | varies | Lap counter display |
| `draw_position()` | varies | Race position display |
| `draw_timer()` | varies | Race timer display |
| `draw_minimap()` | varies | Track minimap |

### UI Primitives

| Function | Size | Description |
|----------|------|-------------|
| `draw_ui_element()` | 572 bytes | Draw sprite element |
| `draw_text()` | 700 bytes | Render text string |
| `draw_number()` | 524 bytes | Render numeric value |
| `draw_message()` | varies | Centered message display |
| `text_color_set()` | 460 bytes | Set text color |
| `selection_highlight()` | 2340 bytes | Menu item highlight |
| `button_sprite_render()` | 1100 bytes | Controller button icons |
| `icon_sprite_render()` | 1760 bytes | Icon sprites |

### Dialogs and Popups

| Function | Size | Description |
|----------|------|-------------|
| `menu_dialog_display()` | varies | Show dialog box |
| `menu_confirm_dialog()` | 412 bytes | Yes/No confirmation |
| `menu_dialog_close()` | varies | Close dialog box |
| `menu_text_input()` | 460 bytes | On-screen keyboard input |

### Save/Load

| Function | Size | Description |
|----------|------|-------------|
| `menu_save_options()` | 764 bytes | Save options to Controller Pak |
| `menu_load_options()` | 364 bytes | Load options from Controller Pak |
| `menu_option_toggle()` | 576 bytes | Toggle a game option |
| `menu_vibration_test()` | 404 bytes | Test rumble feature |

---

## Memory Layout

Key addresses for menu/UI data:

| Address | Size | Purpose |
|---------|------|---------|
| 0x801146EC | 1 byte | gstate (game state) |
| 0x80142AFC | 4 bytes | frame_counter |
| 0x80149438 | 4 bytes | Display list pointer |
| 0x80159000 | varies | Unlock flags |
| 0x80168900 | 4 bytes | Text color |
| 0x8016C000 | varies | Button texture atlas |
| 0x8016D000 | varies | Icon texture atlas |

---

## Notes for Decompilation

1. **Menu structure offsets**: Menu data structures use fixed offsets (+0x00 for state, +0x04 for selection, +0x08 for item count, etc.)

2. **Display list generation**: UI rendering directly builds RDP commands (G_SETTILESIZE, G_TEXRECT, G_FILLRECT)

3. **Sprite atlas**: Multiple sprites are packed into texture atlases with calculated U/V offsets

4. **Animation timing**: Many animations use `frame_counter * constant` for timing, assuming 60fps

5. **Arcade differences**: N64 version has additional pause menu, options saving to Controller Pak, and simplified track/car selection compared to arcade

---

*Generated for the Rush 2049 N64 Decompilation Project*
*Last updated: 2025-12-30*
