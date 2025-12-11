/**
 * menu.c - Menu and UI system for Rush 2049 N64
 *
 * Handles main menus, pause menu, options, and UI elements.
 */

#include "game/menu.h"
#include "game/game.h"

/* Input timing */
#define INPUT_REPEAT_DELAY      15
#define INPUT_REPEAT_RATE       5
#define FADE_SPEED_DEFAULT      0.05f

/* Animation timing */
#define MENU_ANIM_SPEED         0.1f

/* Global UI state */
UIState gUI;

/* Option labels */
static char *sOnOffLabels[] = {"Off", "On"};
static char *sDifficultyLabels[] = {"Easy", "Normal", "Hard", "Expert"};
static char *sControlLabels[] = {"Standard", "Alternate", "Pro"};

/* Predefined menus */
Menu gMainMenu;
Menu gPauseMenu;
Menu gOptionsMenu;
Menu gAudioMenu;
Menu gVideoMenu;
Menu gControlsMenu;

/*
 * Helper function to copy string
 */
static void strcopy(char *dst, const char *src, s32 max)
{
    s32 i;
    for (i = 0; i < max - 1 && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

/*
 * menu_init - Initialize menu system
 */
void menu_init(void)
{
    s32 i;

    /* Clear UI state */
    gUI.menu_depth = 0;
    gUI.current_menu = NULL;
    gUI.input_timer = 0;
    gUI.input_repeat = 0;
    gUI.anim_progress = 0.0f;
    gUI.animating = 0;
    gUI.fade_alpha = 0.0f;
    gUI.fading = 0;
    gUI.fade_in = 0;

    for (i = 0; i < MAX_MENU_DEPTH; i++) {
        gUI.menu_stack[i] = NULL;
    }

    /* Initialize main menu */
    strcopy(gMainMenu.title, "RUSH 2049", MAX_MENU_TEXT);
    gMainMenu.num_items = 4;
    gMainMenu.selected = 0;
    gMainMenu.scroll_offset = 0;
    gMainMenu.visible_items = 8;
    gMainMenu.state = MENU_STATE_CLOSED;
    gMainMenu.flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gMainMenu.parent = NULL;

    /* Main menu items */
    strcopy(gMainMenu.items[0].text, "Start Race", MAX_MENU_TEXT);
    gMainMenu.items[0].type = MENU_TYPE_ACTION;
    gMainMenu.items[0].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gMainMenu.items[0].callback = menu_cb_start_game;

    strcopy(gMainMenu.items[1].text, "Options", MAX_MENU_TEXT);
    gMainMenu.items[1].type = MENU_TYPE_SUBMENU;
    gMainMenu.items[1].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gMainMenu.items[1].submenu = &gOptionsMenu;

    strcopy(gMainMenu.items[2].text, "Records", MAX_MENU_TEXT);
    gMainMenu.items[2].type = MENU_TYPE_ACTION;
    gMainMenu.items[2].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;

    strcopy(gMainMenu.items[3].text, "Quit", MAX_MENU_TEXT);
    gMainMenu.items[3].type = MENU_TYPE_ACTION;
    gMainMenu.items[3].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gMainMenu.items[3].callback = menu_cb_quit;

    /* Initialize pause menu */
    strcopy(gPauseMenu.title, "PAUSED", MAX_MENU_TEXT);
    gPauseMenu.num_items = 4;
    gPauseMenu.selected = 0;
    gPauseMenu.scroll_offset = 0;
    gPauseMenu.visible_items = 8;
    gPauseMenu.state = MENU_STATE_CLOSED;
    gPauseMenu.flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gPauseMenu.parent = NULL;

    strcopy(gPauseMenu.items[0].text, "Resume", MAX_MENU_TEXT);
    gPauseMenu.items[0].type = MENU_TYPE_ACTION;
    gPauseMenu.items[0].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gPauseMenu.items[0].callback = menu_cb_resume;

    strcopy(gPauseMenu.items[1].text, "Restart Race", MAX_MENU_TEXT);
    gPauseMenu.items[1].type = MENU_TYPE_ACTION;
    gPauseMenu.items[1].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gPauseMenu.items[1].callback = menu_cb_restart;

    strcopy(gPauseMenu.items[2].text, "Options", MAX_MENU_TEXT);
    gPauseMenu.items[2].type = MENU_TYPE_SUBMENU;
    gPauseMenu.items[2].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gPauseMenu.items[2].submenu = &gOptionsMenu;

    strcopy(gPauseMenu.items[3].text, "Quit Race", MAX_MENU_TEXT);
    gPauseMenu.items[3].type = MENU_TYPE_ACTION;
    gPauseMenu.items[3].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gPauseMenu.items[3].callback = menu_cb_quit;

    /* Initialize options menu */
    strcopy(gOptionsMenu.title, "OPTIONS", MAX_MENU_TEXT);
    gOptionsMenu.num_items = 4;
    gOptionsMenu.selected = 0;
    gOptionsMenu.scroll_offset = 0;
    gOptionsMenu.visible_items = 8;
    gOptionsMenu.state = MENU_STATE_CLOSED;
    gOptionsMenu.flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gOptionsMenu.parent = NULL;

    strcopy(gOptionsMenu.items[0].text, "Audio", MAX_MENU_TEXT);
    gOptionsMenu.items[0].type = MENU_TYPE_SUBMENU;
    gOptionsMenu.items[0].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gOptionsMenu.items[0].submenu = &gAudioMenu;

    strcopy(gOptionsMenu.items[1].text, "Video", MAX_MENU_TEXT);
    gOptionsMenu.items[1].type = MENU_TYPE_SUBMENU;
    gOptionsMenu.items[1].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gOptionsMenu.items[1].submenu = &gVideoMenu;

    strcopy(gOptionsMenu.items[2].text, "Controls", MAX_MENU_TEXT);
    gOptionsMenu.items[2].type = MENU_TYPE_SUBMENU;
    gOptionsMenu.items[2].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gOptionsMenu.items[2].submenu = &gControlsMenu;

    strcopy(gOptionsMenu.items[3].text, "Back", MAX_MENU_TEXT);
    gOptionsMenu.items[3].type = MENU_TYPE_BACK;
    gOptionsMenu.items[3].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;

    /* Initialize audio menu */
    strcopy(gAudioMenu.title, "AUDIO", MAX_MENU_TEXT);
    gAudioMenu.num_items = 4;
    gAudioMenu.selected = 0;
    gAudioMenu.scroll_offset = 0;
    gAudioMenu.visible_items = 8;
    gAudioMenu.state = MENU_STATE_CLOSED;
    gAudioMenu.flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gAudioMenu.parent = &gOptionsMenu;

    strcopy(gAudioMenu.items[0].text, "Music Volume", MAX_MENU_TEXT);
    gAudioMenu.items[0].type = MENU_TYPE_SLIDER;
    gAudioMenu.items[0].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gAudioMenu.items[0].value = 100;
    gAudioMenu.items[0].min_value = 0;
    gAudioMenu.items[0].max_value = 100;
    gAudioMenu.items[0].step = 10;

    strcopy(gAudioMenu.items[1].text, "SFX Volume", MAX_MENU_TEXT);
    gAudioMenu.items[1].type = MENU_TYPE_SLIDER;
    gAudioMenu.items[1].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gAudioMenu.items[1].value = 100;
    gAudioMenu.items[1].min_value = 0;
    gAudioMenu.items[1].max_value = 100;
    gAudioMenu.items[1].step = 10;

    strcopy(gAudioMenu.items[2].text, "Stereo", MAX_MENU_TEXT);
    gAudioMenu.items[2].type = MENU_TYPE_TOGGLE;
    gAudioMenu.items[2].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gAudioMenu.items[2].value = 1;
    gAudioMenu.items[2].options = sOnOffLabels;
    gAudioMenu.items[2].num_options = 2;

    strcopy(gAudioMenu.items[3].text, "Back", MAX_MENU_TEXT);
    gAudioMenu.items[3].type = MENU_TYPE_BACK;
    gAudioMenu.items[3].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;

    /* Initialize video menu */
    strcopy(gVideoMenu.title, "VIDEO", MAX_MENU_TEXT);
    gVideoMenu.num_items = 3;
    gVideoMenu.selected = 0;
    gVideoMenu.scroll_offset = 0;
    gVideoMenu.visible_items = 8;
    gVideoMenu.state = MENU_STATE_CLOSED;
    gVideoMenu.flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gVideoMenu.parent = &gOptionsMenu;

    strcopy(gVideoMenu.items[0].text, "Brightness", MAX_MENU_TEXT);
    gVideoMenu.items[0].type = MENU_TYPE_SLIDER;
    gVideoMenu.items[0].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gVideoMenu.items[0].value = 50;
    gVideoMenu.items[0].min_value = 0;
    gVideoMenu.items[0].max_value = 100;
    gVideoMenu.items[0].step = 5;

    strcopy(gVideoMenu.items[1].text, "Widescreen", MAX_MENU_TEXT);
    gVideoMenu.items[1].type = MENU_TYPE_TOGGLE;
    gVideoMenu.items[1].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gVideoMenu.items[1].value = 0;
    gVideoMenu.items[1].options = sOnOffLabels;
    gVideoMenu.items[1].num_options = 2;

    strcopy(gVideoMenu.items[2].text, "Back", MAX_MENU_TEXT);
    gVideoMenu.items[2].type = MENU_TYPE_BACK;
    gVideoMenu.items[2].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;

    /* Initialize controls menu */
    strcopy(gControlsMenu.title, "CONTROLS", MAX_MENU_TEXT);
    gControlsMenu.num_items = 4;
    gControlsMenu.selected = 0;
    gControlsMenu.scroll_offset = 0;
    gControlsMenu.visible_items = 8;
    gControlsMenu.state = MENU_STATE_CLOSED;
    gControlsMenu.flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gControlsMenu.parent = &gOptionsMenu;

    strcopy(gControlsMenu.items[0].text, "Layout", MAX_MENU_TEXT);
    gControlsMenu.items[0].type = MENU_TYPE_SELECT;
    gControlsMenu.items[0].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gControlsMenu.items[0].value = 0;
    gControlsMenu.items[0].options = sControlLabels;
    gControlsMenu.items[0].num_options = 3;

    strcopy(gControlsMenu.items[1].text, "Vibration", MAX_MENU_TEXT);
    gControlsMenu.items[1].type = MENU_TYPE_TOGGLE;
    gControlsMenu.items[1].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gControlsMenu.items[1].value = 1;
    gControlsMenu.items[1].options = sOnOffLabels;
    gControlsMenu.items[1].num_options = 2;

    strcopy(gControlsMenu.items[2].text, "Sensitivity", MAX_MENU_TEXT);
    gControlsMenu.items[2].type = MENU_TYPE_SLIDER;
    gControlsMenu.items[2].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
    gControlsMenu.items[2].value = 50;
    gControlsMenu.items[2].min_value = 0;
    gControlsMenu.items[2].max_value = 100;
    gControlsMenu.items[2].step = 10;

    strcopy(gControlsMenu.items[3].text, "Back", MAX_MENU_TEXT);
    gControlsMenu.items[3].type = MENU_TYPE_BACK;
    gControlsMenu.items[3].flags = MENU_FLAG_VISIBLE | MENU_FLAG_ENABLED;
}

/*
 * menu_reset - Reset menu system
 */
void menu_reset(void)
{
    menu_init();
}

/*
 * menu_open - Open a menu
 */
void menu_open(Menu *menu)
{
    if (menu == NULL) {
        return;
    }

    gUI.current_menu = menu;
    gUI.menu_depth = 0;
    gUI.menu_stack[0] = menu;
    menu->state = MENU_STATE_OPENING;
    menu->selected = 0;
    gUI.anim_progress = 0.0f;
    gUI.animating = 1;
}

/*
 * menu_close - Close current menu
 */
void menu_close(void)
{
    if (gUI.current_menu == NULL) {
        return;
    }

    gUI.current_menu->state = MENU_STATE_CLOSING;
    gUI.anim_progress = 1.0f;
    gUI.animating = 1;
}

/*
 * menu_back - Go back to parent menu
 */
void menu_back(void)
{
    if (gUI.menu_depth > 0) {
        menu_pop();
    } else {
        menu_close();
    }
}

/*
 * menu_push - Push a submenu onto the stack
 */
void menu_push(Menu *menu)
{
    if (menu == NULL || gUI.menu_depth >= MAX_MENU_DEPTH - 1) {
        return;
    }

    gUI.menu_depth++;
    gUI.menu_stack[gUI.menu_depth] = menu;
    gUI.current_menu = menu;
    menu->state = MENU_STATE_OPEN;
    menu->selected = 0;
}

/*
 * menu_pop - Pop current menu from stack
 */
void menu_pop(void)
{
    if (gUI.menu_depth <= 0) {
        return;
    }

    gUI.current_menu->state = MENU_STATE_CLOSED;
    gUI.menu_depth--;
    gUI.current_menu = gUI.menu_stack[gUI.menu_depth];
}

/*
 * menu_update - Update menu system
 */
void menu_update(void)
{
    Menu *menu;

    if (gUI.current_menu == NULL) {
        return;
    }

    menu = gUI.current_menu;

    /* Update animation */
    if (gUI.animating) {
        if (menu->state == MENU_STATE_OPENING) {
            gUI.anim_progress += MENU_ANIM_SPEED;
            if (gUI.anim_progress >= 1.0f) {
                gUI.anim_progress = 1.0f;
                gUI.animating = 0;
                menu->state = MENU_STATE_OPEN;
            }
        } else if (menu->state == MENU_STATE_CLOSING) {
            gUI.anim_progress -= MENU_ANIM_SPEED;
            if (gUI.anim_progress <= 0.0f) {
                gUI.anim_progress = 0.0f;
                gUI.animating = 0;
                menu->state = MENU_STATE_CLOSED;
                gUI.current_menu = NULL;
            }
        }
    }

    /* Update input timer */
    if (gUI.input_timer > 0) {
        gUI.input_timer--;
    }

    /* Update fade */
    if (gUI.fading) {
        if (gUI.fade_in) {
            gUI.fade_alpha -= FADE_SPEED_DEFAULT;
            if (gUI.fade_alpha <= 0.0f) {
                gUI.fade_alpha = 0.0f;
                gUI.fading = 0;
            }
        } else {
            gUI.fade_alpha += FADE_SPEED_DEFAULT;
            if (gUI.fade_alpha >= 1.0f) {
                gUI.fade_alpha = 1.0f;
                gUI.fading = 0;
            }
        }
    }
}

/*
 * menu_draw - Draw current menu
 */
void menu_draw(void)
{
    Menu *menu;
    MenuItem *item;
    s32 i, y;
    s32 x_center;
    u32 color;

    menu = gUI.current_menu;
    if (menu == NULL || menu->state == MENU_STATE_CLOSED) {
        return;
    }

    x_center = SCREEN_WIDTH / 2;

    /* Draw semi-transparent background */
    ui_draw_box_filled(40, 40, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 80,
                       0x00000080, COLOR_WHITE);

    /* Draw title */
    ui_draw_text(x_center, 60, menu->title, COLOR_YELLOW, ALIGN_CENTER);

    /* Draw items */
    y = 100;
    for (i = 0; i < menu->num_items && i < menu->visible_items; i++) {
        item = &menu->items[i + menu->scroll_offset];

        if (!(item->flags & MENU_FLAG_VISIBLE)) {
            continue;
        }

        /* Determine color */
        if (!(item->flags & MENU_FLAG_ENABLED)) {
            color = COLOR_GRAY;
        } else if (i + menu->scroll_offset == menu->selected) {
            color = COLOR_YELLOW;
        } else {
            color = COLOR_WHITE;
        }

        /* Draw item based on type */
        switch (item->type) {
            case MENU_TYPE_LABEL:
            case MENU_TYPE_ACTION:
            case MENU_TYPE_SUBMENU:
            case MENU_TYPE_BACK:
                ui_draw_text(x_center, y, item->text, color, ALIGN_CENTER);
                break;

            case MENU_TYPE_TOGGLE:
            case MENU_TYPE_SELECT:
                ui_draw_text(100, y, item->text, color, ALIGN_LEFT);
                if (item->options != NULL && item->value < item->num_options) {
                    ui_draw_text(220, y, item->options[item->value], color, ALIGN_LEFT);
                }
                break;

            case MENU_TYPE_SLIDER:
                ui_draw_text(100, y, item->text, color, ALIGN_LEFT);
                ui_draw_bar(200, y, 80, 10,
                            (f32)(item->value - item->min_value) /
                            (f32)(item->max_value - item->min_value),
                            color, COLOR_DARK_GRAY);
                break;
        }

        y += 20;
    }

    /* Draw scroll indicators if needed */
    if (menu->scroll_offset > 0) {
        ui_draw_text(x_center, 85, "^", COLOR_WHITE, ALIGN_CENTER);
    }
    if (menu->scroll_offset + menu->visible_items < menu->num_items) {
        ui_draw_text(x_center, y + 5, "v", COLOR_WHITE, ALIGN_CENTER);
    }
}

/*
 * Menu navigation functions
 */

void menu_select_next(void)
{
    Menu *menu;

    menu = gUI.current_menu;
    if (menu == NULL || menu->state != MENU_STATE_OPEN) {
        return;
    }

    if (gUI.input_timer > 0) {
        return;
    }

    menu->selected++;
    if (menu->selected >= menu->num_items) {
        menu->selected = 0;
        menu->scroll_offset = 0;
    } else if (menu->selected >= menu->scroll_offset + menu->visible_items) {
        menu->scroll_offset++;
    }

    gUI.input_timer = gUI.input_repeat ? INPUT_REPEAT_RATE : INPUT_REPEAT_DELAY;
    gUI.input_repeat = 1;
}

void menu_select_prev(void)
{
    Menu *menu;

    menu = gUI.current_menu;
    if (menu == NULL || menu->state != MENU_STATE_OPEN) {
        return;
    }

    if (gUI.input_timer > 0) {
        return;
    }

    menu->selected--;
    if (menu->selected < 0) {
        menu->selected = menu->num_items - 1;
        if (menu->num_items > menu->visible_items) {
            menu->scroll_offset = menu->num_items - menu->visible_items;
        }
    } else if (menu->selected < menu->scroll_offset) {
        menu->scroll_offset--;
    }

    gUI.input_timer = gUI.input_repeat ? INPUT_REPEAT_RATE : INPUT_REPEAT_DELAY;
    gUI.input_repeat = 1;
}

void menu_select_item(s32 index)
{
    Menu *menu;

    menu = gUI.current_menu;
    if (menu == NULL || index < 0 || index >= menu->num_items) {
        return;
    }

    menu->selected = index;
}

void menu_confirm(void)
{
    Menu *menu;
    MenuItem *item;

    menu = gUI.current_menu;
    if (menu == NULL || menu->state != MENU_STATE_OPEN) {
        return;
    }

    item = &menu->items[menu->selected];

    if (!(item->flags & MENU_FLAG_ENABLED)) {
        return;
    }

    switch (item->type) {
        case MENU_TYPE_ACTION:
            if (item->callback != NULL) {
                item->callback(item->value);
            }
            break;

        case MENU_TYPE_TOGGLE:
            item->value = !item->value;
            if (item->callback != NULL) {
                item->callback(item->value);
            }
            break;

        case MENU_TYPE_SELECT:
            item->value++;
            if (item->value >= item->num_options) {
                item->value = 0;
            }
            if (item->callback != NULL) {
                item->callback(item->value);
            }
            break;

        case MENU_TYPE_SUBMENU:
            if (item->submenu != NULL) {
                menu_push((Menu *)item->submenu);
            }
            break;

        case MENU_TYPE_BACK:
            menu_back();
            break;
    }
}

void menu_cancel(void)
{
    menu_back();
}

/*
 * Item manipulation
 */

void menu_item_increase(s32 index)
{
    Menu *menu;
    MenuItem *item;

    menu = gUI.current_menu;
    if (menu == NULL || index < 0 || index >= menu->num_items) {
        return;
    }

    item = &menu->items[index];

    if (item->type == MENU_TYPE_SLIDER) {
        item->value += item->step;
        if (item->value > item->max_value) {
            item->value = item->max_value;
        }
        if (item->callback != NULL) {
            item->callback(item->value);
        }
    } else if (item->type == MENU_TYPE_SELECT) {
        item->value++;
        if (item->value >= item->num_options) {
            item->value = 0;
        }
        if (item->callback != NULL) {
            item->callback(item->value);
        }
    }
}

void menu_item_decrease(s32 index)
{
    Menu *menu;
    MenuItem *item;

    menu = gUI.current_menu;
    if (menu == NULL || index < 0 || index >= menu->num_items) {
        return;
    }

    item = &menu->items[index];

    if (item->type == MENU_TYPE_SLIDER) {
        item->value -= item->step;
        if (item->value < item->min_value) {
            item->value = item->min_value;
        }
        if (item->callback != NULL) {
            item->callback(item->value);
        }
    } else if (item->type == MENU_TYPE_SELECT) {
        item->value--;
        if (item->value < 0) {
            item->value = item->num_options - 1;
        }
        if (item->callback != NULL) {
            item->callback(item->value);
        }
    }
}

void menu_item_toggle(s32 index)
{
    Menu *menu;
    MenuItem *item;

    menu = gUI.current_menu;
    if (menu == NULL || index < 0 || index >= menu->num_items) {
        return;
    }

    item = &menu->items[index];

    if (item->type == MENU_TYPE_TOGGLE) {
        item->value = !item->value;
        if (item->callback != NULL) {
            item->callback(item->value);
        }
    }
}

void menu_item_set_value(s32 index, s32 value)
{
    Menu *menu;
    MenuItem *item;

    menu = gUI.current_menu;
    if (menu == NULL || index < 0 || index >= menu->num_items) {
        return;
    }

    item = &menu->items[index];
    item->value = value;
}

s32 menu_item_get_value(s32 index)
{
    Menu *menu;

    menu = gUI.current_menu;
    if (menu == NULL || index < 0 || index >= menu->num_items) {
        return 0;
    }

    return menu->items[index].value;
}

/*
 * Menu queries
 */

s32 menu_is_open(void)
{
    return (gUI.current_menu != NULL &&
            gUI.current_menu->state == MENU_STATE_OPEN);
}

s32 menu_get_selected(void)
{
    if (gUI.current_menu == NULL) {
        return -1;
    }
    return gUI.current_menu->selected;
}

Menu* menu_get_current(void)
{
    return gUI.current_menu;
}

/*
 * UI helper functions
 */

void ui_init(void)
{
    menu_init();
}

void ui_update(void)
{
    menu_update();
}

void ui_draw(void)
{
    menu_draw();

    /* Draw fade overlay if fading */
    if (gUI.fade_alpha > 0.0f) {
        u32 fade_color = ((u32)(gUI.fade_alpha * 255.0f)) & 0xFF;
        fade_color = (fade_color << 24) | (fade_color << 16) |
                     (fade_color << 8) | fade_color;
        ui_draw_box_filled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fade_color, 0);
    }
}

/*
 * Text drawing stubs - actual implementation needs display list generation
 */

void ui_draw_text(s16 x, s16 y, const char *text, u32 color, u8 align)
{
    /* TODO: Implement with display list */
}

void ui_draw_text_scaled(s16 x, s16 y, const char *text, u32 color, u8 align, f32 scale)
{
    /* TODO: Implement with display list */
}

void ui_draw_number(s16 x, s16 y, s32 value, u32 color)
{
    /* TODO: Implement with display list */
}

void ui_draw_time(s16 x, s16 y, u32 frames, u32 color)
{
    /* TODO: Implement with display list */
}

/*
 * Bar drawing stubs
 */

void ui_draw_bar(s16 x, s16 y, s16 width, s16 height, f32 value, u32 fill, u32 bg)
{
    /* TODO: Implement with display list */
}

void ui_draw_bar_segmented(s16 x, s16 y, s16 width, s16 height, f32 value, s32 segments, u32 fill, u32 bg)
{
    /* TODO: Implement with display list */
}

/*
 * Box/Panel drawing stubs
 */

void ui_draw_box(s16 x, s16 y, s16 width, s16 height, u32 color)
{
    /* TODO: Implement with display list */
}

void ui_draw_box_filled(s16 x, s16 y, s16 width, s16 height, u32 fill, u32 border)
{
    /* TODO: Implement with display list */
}

void ui_draw_panel(s16 x, s16 y, s16 width, s16 height)
{
    /* TODO: Implement with display list */
}

/*
 * Screen effects
 */

void ui_fade_in(f32 speed)
{
    gUI.fade_alpha = 1.0f;
    gUI.fading = 1;
    gUI.fade_in = 1;
}

void ui_fade_out(f32 speed)
{
    gUI.fade_alpha = 0.0f;
    gUI.fading = 1;
    gUI.fade_in = 0;
}

void ui_set_fade(f32 alpha)
{
    gUI.fade_alpha = alpha;
    if (alpha < 0.0f) gUI.fade_alpha = 0.0f;
    if (alpha > 1.0f) gUI.fade_alpha = 1.0f;
}

f32 ui_get_fade(void)
{
    return gUI.fade_alpha;
}

s32 ui_is_fading(void)
{
    return gUI.fading;
}

/*
 * Predefined menu callbacks
 */

void menu_cb_start_game(s32 value)
{
    menu_close();
    /* Transition to track/car selection */
}

void menu_cb_options(s32 value)
{
    menu_push(&gOptionsMenu);
}

void menu_cb_quit(s32 value)
{
    menu_close();
    /* Return to title/attract mode */
}

void menu_cb_resume(s32 value)
{
    menu_close();
    /* Unpause game */
}

void menu_cb_restart(s32 value)
{
    menu_close();
    /* Restart current race */
}

void menu_cb_audio(s32 value)
{
    menu_push(&gAudioMenu);
}

void menu_cb_video(s32 value)
{
    menu_push(&gVideoMenu);
}

void menu_cb_controls(s32 value)
{
    menu_push(&gControlsMenu);
}
