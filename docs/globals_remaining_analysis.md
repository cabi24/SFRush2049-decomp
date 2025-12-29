# Remaining Global Variables Analysis

## High Priority (6+ references)

None found in the current top-50 list (all entries are 4 references in `src/game/game.c`).

## Medium Priority (3-5 references)

| Address | Refs | Proposed Name | Evidence | Confidence |
|---------|------|---------------|----------|------------|
| D_801601A4 | 4 | audio_listener_velocity | Used in doppler/relative velocity calc for audio pan/pitch | High |
| D_80160198 | 4 | audio_listener_right | Used in right-vector dot product for audio panning | High |
| D_8015A7C0 | 4 | main_menu_selection | Used in `main_menu_input` and `main_menu_render` | High |
| D_8015A7B0 | 4 | title_anim_frame | Used in `title_screen` for logo animation timing | High |
| D_8015A7A8 | 4 | attract_video_frame | Frame counter for `attract_video_play` | High |
| D_8015A790 | 4 | demo_playback_frame | Frame index for attract demo input playback | High |
| D_8015A788 | 4 | demo_cycle_index | Cycles through demo slots (mod 3) | High |
| D_8015A770 | 4 | champ_points | Championship points per player (array) | High |
| D_8015A734 | 4 | results_timer | Timer used in `results_screen` | High |
| D_8015A730 | 4 | results_state | State used in `results_screen` | High |
| D_8015A548 | 4 | achievement_anim_frame | Animation frame for achievement popup | High |
| D_8015A538 | 4 | demo_cycle_count | Incremented to cycle demo slots | Med |
| D_8015A4B4 | 4 | stats_scroll_y | Scroll position in `statistics_display` | High |
| D_8015A4B0 | 4 | stats_state | State/enable for `statistics_display` | Med |
| D_8015A478 | 4 | trophy_anim_frame | Frame counter for `trophy_animate` | High |
| D_8015A440 | 4 | gameover_anim_frame | Animation frame in `game_over_screen` | High |
| D_8015A420 | 4 | race_results_anim_frame | Animation frame in `race_results_display` | High |
| D_8015A314 | 4 | new_best_lap_flag | Set when best lap is updated, shown in results | High |
| D_8015A310 | 4 | new_record_flag | Set when best race time is updated, shown in results | High |
| D_8015A304 | 4 | ghost_recording_flag | Controls ghost recording start/stop | High |
| D_8015A298 | 4 | race_num_laps | Number of laps for current race | High |
| D_8015A088 | 4 | bonus_coins_complete | Flag set when all coins collected | High |
| D_8015A020 | 4 | split_display_active | Split-time display active flags (array) | Med |
| D_80159F58 | 4 | pause_overlay_mode | Pause overlay render mode flag | Med |
| D_80159F48 | 4 | pause_submenu_mode | Pause submenu mode (options/controls) | High |
| D_80159F40 | 4 | pause_menu_selection | Selected item in pause menu | High |
| D_80159E48 | 4 | ghost_selected_index | Ghost slot selection in ghost race menu | High |
| D_80159E44 | 4 | ghost_track_selection | Track selection in ghost race menu | High |
| D_80159E30 | 4 | battle_weapons_enabled | Battle setup toggle for weapons | High |
| D_80159E2C | 4 | battle_time_limit | Battle setup time limit index/value | High |
| D_80159E28 | 4 | battle_frag_limit | Battle setup frag limit index/value | High |
| D_80159E24 | 4 | battle_arena_selection | Battle arena selection | High |
| D_80159E0C | 4 | stunt_time_limit | Stunt setup time limit index/value | High |
| D_80159E04 | 4 | stunt_menu_selection | Stunt setup menu selection; also used as debug collision toggle | Med |
| D_80159DC0 | 4 | credits_scroll_y | Scroll position for credits screen | High |
| D_80159B4C | 4 | sun_angle | Time-of-day sun angle setting | High |
| D_80159B48 | 4 | sun_color | Time-of-day sun color setting | High |
| D_80159A14 | 4 | finish_time_by_car | Finish times recorded per car; also used as ghost mode flag | Low |
| D_80159904 | 4 | keyboard_row | On-screen keyboard row index | High |
| D_80159900 | 4 | keyboard_col | On-screen keyboard column index | High |

## Arcade Cross-References Found

| N64 Address | Arcade Variable | File |
|-------------|-----------------|------|
| D_801146EC | gstate | reference/repos/rushtherock/game/globals.h, game.h |
| D_80142AFC | IRQTIME | reference/repos/rushtherock/game/timint.c |
| D_80159A08 | trackno | reference/repos/rushtherock/game/globals.h |
| D_8015A108 | num_humans | reference/repos/rushtherock/game/globals.h |
| D_80152818 | game_car | reference/repos/rushtherock/game/modeldat.h |
| D_80158000 | gCamPos | reference/repos/rushtherock/game/globals.h |
| D_80159B10 | gMirrorMode | reference/repos/rushtherock/game/globals.h |
| D_801582B4 | number_checkpoints | reference/repos/rushtherock/game/globals.h |
| D_8015A298 | track_len / number_of_laps | reference/repos/rushtherock/game/globals.h, game.h |
| D_80159A54 | gThisCar | reference/repos/rushtherock/game/game.c |

## Recommended Next Batch for Renaming

Top candidates with straightforward semantics:

```sh
sed -i 's/D_801601A4/audio_listener_velocity/g' src/game/game.c
sed -i 's/D_80160198/audio_listener_right/g' src/game/game.c
sed -i 's/D_8015A7C0/main_menu_selection/g' src/game/game.c
sed -i 's/D_8015A7B0/title_anim_frame/g' src/game/game.c
sed -i 's/D_8015A7A8/attract_video_frame/g' src/game/game.c
sed -i 's/D_8015A790/demo_playback_frame/g' src/game/game.c
sed -i 's/D_8015A788/demo_cycle_index/g' src/game/game.c
sed -i 's/D_8015A770/champ_points/g' src/game/game.c
sed -i 's/D_8015A734/results_timer/g' src/game/game.c
sed -i 's/D_8015A730/results_state/g' src/game/game.c
sed -i 's/D_8015A548/achievement_anim_frame/g' src/game/game.c
sed -i 's/D_8015A4B4/stats_scroll_y/g' src/game/game.c
sed -i 's/D_8015A4B0/stats_state/g' src/game/game.c
sed -i 's/D_8015A478/trophy_anim_frame/g' src/game/game.c
sed -i 's/D_8015A440/gameover_anim_frame/g' src/game/game.c
sed -i 's/D_8015A420/race_results_anim_frame/g' src/game/game.c
sed -i 's/D_8015A314/new_best_lap_flag/g' src/game/game.c
sed -i 's/D_8015A310/new_record_flag/g' src/game/game.c
sed -i 's/D_8015A304/ghost_recording_flag/g' src/game/game.c
sed -i 's/D_8015A298/race_num_laps/g' src/game/game.c
```
