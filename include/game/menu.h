/**
 * menu.h - Menu and UI system for Rush 2049 N64
 *
 * Handles main menus, pause menu, options, and UI elements.
 */

#ifndef MENU_H
#define MENU_H

#include "types.h"

/* Maximum values */
#define MAX_MENU_ITEMS      16
#define MAX_MENU_DEPTH      8
#define MAX_MENU_TEXT       32

/* Menu item types */
#define MENU_TYPE_NONE      0
#define MENU_TYPE_LABEL     1   /* Static text label */
#define MENU_TYPE_ACTION    2   /* Execute callback when selected */
#define MENU_TYPE_TOGGLE    3   /* On/Off toggle */
#define MENU_TYPE_SLIDER    4   /* Value slider */
#define MENU_TYPE_SELECT    5   /* Value selection from list */
#define MENU_TYPE_SUBMENU   6   /* Opens another menu */
#define MENU_TYPE_BACK      7   /* Return to parent menu */

/* Menu states */
#define MENU_STATE_CLOSED   0
#define MENU_STATE_OPENING  1
#define MENU_STATE_OPEN     2
#define MENU_STATE_CLOSING  3

/* Menu flags */
#define MENU_FLAG_VISIBLE   0x01
#define MENU_FLAG_ENABLED   0x02
#define MENU_FLAG_SELECTED  0x04
#define MENU_FLAG_LOCKED    0x08    /* Cannot be changed */

/* UI element types */
#define UI_ELEM_NONE        0
#define UI_ELEM_TEXT        1
#define UI_ELEM_NUMBER      2
#define UI_ELEM_BAR         3
#define UI_ELEM_ICON        4
#define UI_ELEM_IMAGE       5

/* Text alignment */
#define ALIGN_LEFT          0
#define ALIGN_CENTER        1
#define ALIGN_RIGHT         2

/* Colors (RGBA8888) */
#define COLOR_WHITE         0xFFFFFFFF
#define COLOR_BLACK         0x000000FF
#define COLOR_RED           0xFF0000FF
#define COLOR_GREEN         0x00FF00FF
#define COLOR_BLUE          0x0000FFFF
#define COLOR_YELLOW        0xFFFF00FF
#define COLOR_GRAY          0x808080FF
#define COLOR_DARK_GRAY     0x404040FF

/* Screen dimensions (N64 low-res) */
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240

/* Menu callback function pointer */
typedef void (*MenuCallback)(s32 value);

/* Menu item structure */
typedef struct MenuItem {
    char        text[MAX_MENU_TEXT];    /* Display text */
    u8          type;                   /* Item type */
    u8          flags;                  /* Item flags */
    s16         value;                  /* Current value (for toggle/slider) */
    s16         min_value;              /* Minimum value (for slider) */
    s16         max_value;              /* Maximum value (for slider) */
    s16         step;                   /* Step size (for slider) */
    MenuCallback callback;              /* Action callback */
    void        *submenu;               /* Submenu pointer (for MENU_TYPE_SUBMENU) */
    char        **options;              /* Options list (for MENU_TYPE_SELECT) */
    s16         num_options;            /* Number of options */
} MenuItem;

/* Menu structure */
typedef struct Menu {
    char        title[MAX_MENU_TEXT];   /* Menu title */
    MenuItem    items[MAX_MENU_ITEMS];  /* Menu items */
    s32         num_items;              /* Number of items */
    s32         selected;               /* Currently selected item */
    s32         scroll_offset;          /* Scroll offset for long menus */
    s32         visible_items;          /* Number of visible items */
    u8          state;                  /* Menu state */
    u8          flags;                  /* Menu flags */
    u8          pad[2];
    struct Menu *parent;                /* Parent menu */
} Menu;

/* UI Text element */
typedef struct UIText {
    char        text[64];
    s16         x, y;
    u8          align;
    u8          visible;
    u32         color;
    f32         scale;
} UIText;

/* UI Bar element (health bar, progress bar, etc.) */
typedef struct UIBar {
    s16         x, y;
    s16         width, height;
    f32         value;          /* 0.0 - 1.0 */
    f32         max_value;
    u32         fill_color;
    u32         bg_color;
    u8          visible;
    u8          pad[3];
} UIBar;

/* UI State */
typedef struct UIState {
    /* Menu stack */
    Menu        *menu_stack[MAX_MENU_DEPTH];
    s32         menu_depth;
    Menu        *current_menu;

    /* Input state */
    u32         input_timer;
    u8          input_repeat;

    /* Animation */
    f32         anim_progress;
    u8          animating;
    u8          pad[3];

    /* Screen fade */
    f32         fade_alpha;
    u8          fading;
    u8          fade_in;
    u8          pad2[2];

} UIState;

/* Global UI state */
extern UIState gUI;

/* Pre-defined menus */
extern Menu gMainMenu;
extern Menu gPauseMenu;
extern Menu gOptionsMenu;
extern Menu gAudioMenu;
extern Menu gVideoMenu;
extern Menu gControlsMenu;

/* Initialization */
void menu_init(void);
void menu_reset(void);

/* Menu management */
void menu_open(Menu *menu);
void menu_close(void);
void menu_back(void);
void menu_push(Menu *menu);
void menu_pop(void);

/* Menu update and draw */
void menu_update(void);
void menu_draw(void);

/* Menu navigation */
void menu_select_next(void);
void menu_select_prev(void);
void menu_select_item(s32 index);
void menu_confirm(void);
void menu_cancel(void);

/* Item manipulation */
void menu_item_increase(s32 index);
void menu_item_decrease(s32 index);
void menu_item_toggle(s32 index);
void menu_item_set_value(s32 index, s32 value);
s32 menu_item_get_value(s32 index);

/* Menu queries */
s32 menu_is_open(void);
s32 menu_get_selected(void);
Menu* menu_get_current(void);

/* UI helpers */
void ui_init(void);
void ui_update(void);
void ui_draw(void);

/* Text drawing */
void ui_draw_text(s16 x, s16 y, const char *text, u32 color, u8 align);
void ui_draw_text_scaled(s16 x, s16 y, const char *text, u32 color, u8 align, f32 scale);
void ui_draw_number(s16 x, s16 y, s32 value, u32 color);
void ui_draw_time(s16 x, s16 y, u32 frames, u32 color);

/* Bar drawing */
void ui_draw_bar(s16 x, s16 y, s16 width, s16 height, f32 value, u32 fill, u32 bg);
void ui_draw_bar_segmented(s16 x, s16 y, s16 width, s16 height, f32 value, s32 segments, u32 fill, u32 bg);

/* Box/Panel drawing */
void ui_draw_box(s16 x, s16 y, s16 width, s16 height, u32 color);
void ui_draw_box_filled(s16 x, s16 y, s16 width, s16 height, u32 fill, u32 border);
void ui_draw_panel(s16 x, s16 y, s16 width, s16 height);

/* Screen effects */
void ui_fade_in(f32 speed);
void ui_fade_out(f32 speed);
void ui_set_fade(f32 alpha);
f32 ui_get_fade(void);
s32 ui_is_fading(void);

/* Predefined menu callbacks */
void menu_cb_start_game(s32 value);
void menu_cb_options(s32 value);
void menu_cb_quit(s32 value);
void menu_cb_resume(s32 value);
void menu_cb_restart(s32 value);
void menu_cb_audio(s32 value);
void menu_cb_video(s32 value);
void menu_cb_controls(s32 value);

/* ========================================================================
 * ARCADE-COMPATIBLE DEFINITIONS (menus.h)
 * Based on arcade: game/menus.h
 * ======================================================================== */

/* Command function modes (from arcade game.h:CmdFunc enum) */
#define MENU_CMD_INITIALIZE     0   /* Initialize = init important menu data */
#define MENU_CMD_ALLOCATE       1   /* Allocate resources */
#define MENU_CMD_DO_IT          2   /* Do_it = check keystroke and act on key */
#define MENU_CMD_UNDO_IT        3   /* Undo_it = clear menu area */
#define MENU_CMD_DEALLOCATE     4   /* Deallocate resources */
#define MENU_CMD_CLEANUP        5   /* Cleanup */

/* Arcade menu palette colors (from arcade OS/config.h) */
#define MENU_PAL_GREEN      0x0800  /* GRN_PAL - palette 2 */
#define MENU_PAL_CYAN       0x0C00  /* CYN_PAL - palette 3 */
#define MENU_PAL_RED        0x1000  /* RED_PAL - palette 4 */
#define MENU_PAL_YELLOW     0x1800  /* YEL_PAL - palette 6 */
#define MENU_PAL_WHITE      0x1C00  /* WHT_PAL - palette 7 */

/* Arcade menu structure (from arcade menus.h) */
typedef struct ArcadeMenu {
    char *label_txt;        /* Display label text */
    char *help_txt;         /* Help text shown at bottom */
    void (*rout)(s16);      /* Menu item routine */
    void *child_menu;       /* Child submenu pointer */
} ArcadeMenu;

/* Arcade menu globals */
extern s16 cp_edit;                 /* Menu editing enabled flag */
extern s16 cur_selection;           /* Current selected item */
extern s16 num_selections;          /* Number of menu items */
extern s16 menu_depth;              /* Current menu depth */
extern s16 menu_txt_index[10];      /* Text position indices */
extern s16 display_pos_data;        /* Display position data flag */
extern s16 clear_pos_data;          /* Clear position data flag */
extern s16 disp_sw_bar;             /* Display switch bar flag */
extern ArcadeMenu *cur_arcade_menu; /* Current menu pointer */
extern ArcadeMenu *menu_tab[10];    /* Menu stack (10 levels max) */
extern char menu_buf[66];           /* Menu text buffer */

/* Arcade menu functions (from arcade menus.c) */
void check_menu(s16 mode, ArcadeMenu *menu_pntr);
void display_menu_arcade(s16 mode, ArcadeMenu *menu);
void clear_display(void);
void clear_menu(void);

/* External arcade dependencies (stubs on N64) */
extern void txt_str(s16 x, s16 y, const char *str, s16 palette);
extern u32 IRQTIME;                 /* IRQ counter for animation */
extern u32 levels;                  /* Switch levels */
extern u32 last_levels;             /* Previous switch levels */
extern u32 edges;                   /* Rising edge switch detections */
extern u32 trailing_edges;          /* Falling edge switch detections */
extern u32 dlevels;                 /* Debug switch levels */
extern u32 dedges;                  /* Debug rising edges */
extern u32 trailing_dedges;         /* Debug falling edges */

#endif /* MENU_H */
