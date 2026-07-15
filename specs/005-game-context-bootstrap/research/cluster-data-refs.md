# Cluster data-reference survey

Generated from `build/game_code.bin` with GNU `mips-linux-gnu-objdump`, using
load address `0x80086A50`, big-endian MIPS III (`mips:4300`). Function ends
were taken at the first complete `jr ra` epilogue (the two instructions at
`0x800FBC30` and `0x800FBF88` precede their stack prologues and are part of
their functions). `R`/`W` mean read/write. Addresses below are effective RAM
addresses reconstructed from `lui` plus signed low-half accesses.

## `game_loop` (`0x800FD464..0x800FD723`, 704 bytes)

| Global | Width/access |
|---|---|
| `0x8002AFB8` | float R |
| `0x8002EB64` | word R |
| `0x8002EB70` | half R |
| `0x80111958` | word R |
| `0x801146E8` | word R/W |
| `0x801146EC` | byte R/W |
| `0x801170FC` | word R |
| `0x801174B4` | word R |
| `0x801174B8` | word R |
| `0x80142AFC` | word R/W |
| `0x80149438` | word W |
| `0x801497C8` | word R |
| `0x801497F4` | word W |

No one base register has multiple field offsets in this function; accesses
are direct globals. Calls: `game_mode_handler` (`0x800C9AE0`),
`attract_or_transition` (`0x800EDDC0`), `process_inputs` (`0x800C997C`),
`sound_control` (`0x800B37E8`), `sound_stop` (`0x800B358C`),
`playgame_state_change` (`0x800CA3B4`), `state_dispatch` (`0x800FD238`),
`RaceStateMachine_Update` (`0x800DB81C`), `update_active_objects`
(`0x800F733C`), `PhysicsObjectList_Update` (`0x800B0868`),
`effects_update_emitters` (`0x800B811C`), `render_scene`/the alias
`Input_ProcessGameplayPad` (`0x800A04C4`), `state_transition`/the alias
`countdown` (`0x800FBF88`), and `countdown_handler` (`0x800FBC30`).

## `game_mode_handler` (`0x800C9AE0..0x800C9BDF`, 256 bytes)

| Global | Width/access |
|---|---|
| `0x80035470` | byte W |
| `0x80035471` | byte W |
| `0x80035472` | byte W |
| `0x801497C8` | word R |
| `0x801497F4` | word W |

No multi-offset base pattern. Calls: `osJamMesg` (`0x800075E0`),
`osRecvMesg` (`0x80007270`), `viUpdateTime` (`0x800013F4`),
`process_inputs` (`0x800C997C`), and `input_init_flag_get` (`0x800C9528`).

## `attract_or_transition` (`0x800EDDC0..0x800EE5DB`, 2,076 bytes)

| Global | Width/access |
|---|---|
| `0x8002AFC0` | word R |
| `0x8002AFC4` | word R |
| `0x8002EBB0` | word R |
| `0x80124FC8` | word R |
| `0x801497C8` | word R/W |
| `0x80151AD0` | half R |
| `0x8015B250` | word W |
| `0x8015B260` | word W |
| `0x8015F72D` | byte R |
| `0x8015F738` | word R |
| `0x80161380` | word R |
| `0x80161398` | word R |
| `0x801613A4` | word R |

Struct pattern: the pointer repeatedly loaded from global `0x801497C8` is
read/written at offsets `{0x00, 0x04, 0x0C}`. Calls:
`osVirtualToPhysical` (`0x8000D5C0`), `wheel_render_full` (`0x800A6BE4`),
`osViSetSpecialFeatures` (`0x80007D20`), `sound_init` (`0x800A4934`), and
`world_trigger_check` (`0x800EDCE8`). The apparent store through address
`0xDE000004` is an N64 display-list command assembled in a register, not a
RAM global, and is therefore excluded from the table.

## `process_inputs` (`0x800C997C..0x800C9ADF`, 356 bytes)

| Global | Width/access |
|---|---|
| `0x8014A108` | half R |
| `0x8014A119` | byte R |
| `0x8014A11C` | word W |
| `0x8014A120` | word W |
| `0x8014A124` | word W |
| `0x8014A128` | float W |
| `0x8014A12C` | float W |
| `0x8014A165` | byte R |
| `0x8014A168` | word W |
| `0x8014A16C` | word W |
| `0x8014A170` | word W |
| `0x8014A174` | float W |
| `0x8014A178` | float W |

Struct patterns: global base `0x8014A118` has offsets
`{0x01,0x04,0x08,0x0C,0x10,0x14,0x4D}`; global base `0x8014A164` has
`{0x01,0x04,0x08,0x0C,0x10,0x14}`. These are two similarly shaped input
records. Sole call: `controller_poll` (`0x800C95DC`).

## `sound_control` (`0x800B37E8..0x800B39BB`, 468 bytes)

There are no direct `lui`/low-half global accesses. Argument `a2` is a
clear record pointer with offsets
`{0x00,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x18,0x1C,0x20}`
(word and halfword fields). Calls: unnamed `0x800B3704`, `input_handler`
(`0x80094EC8`), and `sound_stop` (`0x800B358C`).

## `playgame_state_change` (`0x800CA3B4..0x800CADA3`, 2,544 bytes)

| Global | Width/access |
|---|---|
| `0x80114650` | byte R |
| `0x80114691` | byte R |
| `0x80114692` | byte R |
| `0x80114693` | byte R |
| `0x801174B4` | word R/W |
| `0x801174B8` | word R/W |
| `0x801174BC` | word R |
| `0x80123FB4` | float R |
| `0x80123FB8` | float R |
| `0x80123FBC` | float R |
| `0x8012E6E0` | word R |
| `0x80140008` | word R |
| `0x80146114` | byte R |
| `0x80146115` | byte R |
| `0x80146116` | byte R |
| `0x801461F8` | byte W |
| `0x80146205` | byte W |
| `0x80149414` | byte W |
| `0x80149774` | byte W |
| `0x80149794` | byte W |
| `0x801497C4` | byte W |
| `0x8014A108` | half R/W |
| `0x8014A110` | word R |
| `0x8014A160` | word R |
| `0x80150000` | word R |
| `0x80151AD0` | half W |
| `0x80151AD8` | byte W |
| `0x80156994` | byte R |

Struct patterns: base `0x80114658` plus `{0x39,0x3A,0x3B}` and base
`0x80146108` plus `{0x0C,0x0E}`. Calls: `state_change_preprocess`
(`0x800CA300`), `tire_compound_set` (`0x800A5BB8`), `hud_setup`
(`0x800C8B8C`), unnamed `0x800C885C`, `display_enable` (`0x800C8FA4`),
`init_state_begin` (`0x800C9BE0`), `viScheduleTick` (`0x800014F0`),
`game_mode_handler` (`0x800C9AE0`), unnamed `0x800C9480`,
`player_cleanup_slots` (`0x800C90E0`), `speed_set` (`0x800C9210`),
`resource_slots_clear_multiple` (`0x800C937C`), `player_state_set`
(`0x800C9158`), `player_mode_set` (`0x800C84C0`), unnamed `0x800A3424`,
`sync_entry_register` (`0x800C9194`), `object_render_cleanup`
(`0x800C7308`), `scene_cleanup_slots` (`0x800C70BC`), `InitMaxPath`
(`0x800A1244`), `audio_frame_sync` (`0x80097798`), `hud_speed_display`
(`0x800C84FC`), `visual_objects_update` (`0x800B55FC`), `object_create`
(`0x800B42F0`), `osRecvMesg` (`0x80007270`), `slot_state_setup`
(`0x800B4200`), `osJamMesg` (`0x800075E0`), and unnamed `0x800C813C` and
`0x800A7480`.

## `RaceStateMachine_Update` (`0x800DB81C..0x800DC07F`, 2,148 bytes)

| Global | Width/access |
|---|---|
| `0x80114650` | byte W |
| `0x801170FC` | word R |
| `0x80117350` | byte R/W |
| `0x80117354` | byte R/W |
| `0x801174B4` | word R |
| `0x801242A8` | float R |
| `0x80142699` | byte W |
| `0x80143FD8` | word R |
| `0x80149D98` | word R/W |
| `0x8014A11C` | word R |
| `0x80151AD0` | half R |
| `0x801543CC` | float R |
| `0x8015698C` | word R/W |
| `0x80157244` | byte R |

Struct pattern: pointer loaded from global `0x80143FD8`, offsets
`{0x00,0x02,0x60,0x64}`. Calls: unnamed `0x800C813C`, unnamed
`0x800B4FB0`, `viEnableAccum` (`0x800013C0`), `osRecvMesg`
(`0x80007270`), unnamed `0x80091B00`, `osJamMesg` (`0x800075E0`),
`speed_set` (`0x800C9210`), unnamed `0x800D5374`, `players_frame_update`
(`0x800D5798`), `player_mode_set` (`0x800C84C0`),
`visual_objects_update` (`0x800B55FC`), `frame_sync` (`0x80092360`),
unnamed `0x800DB1E0`, `0x800D91A0`, and `0x800D7634`, `physics_sym`
(`0x800B8C14`), `viDisableAccum` (`0x800013DC`), unnamed `0x800D6530`,
`0x800D6160`, and `0x800D60AC`, `audio_distance_atten` (`0x800B5F4C`),
`resource_type_select` (`0x800B5FC4`), `audio_doppler` (`0x800B5F88`),
unnamed `0x800D5A04`, and `voice_stop` (`0x800B438C`).

## `countdown` (`0x800FBF88..0x800FC9F7`, 2,672 bytes)

| Global | Width/access |
|---|---|
| `0x8002AFB4` | float R |
| `0x80114650` | byte R |
| `0x80114654` | byte R/W |
| `0x801146F0` | byte R/W |
| `0x801174B4` | word R |
| `0x801174B8` | word W |
| `0x801174BC` | word W |
| `0x8012E67C` | byte R |
| `0x8013FECB` | byte R/W |
| `0x801407BC` | word R |
| `0x80140804` | word R |
| `0x80140A00` | word R |
| `0x80140AD8` | word R |
| `0x80140B08` | word R |
| `0x80140BD8` | word R |
| `0x80142510` | word R |
| `0x80142690` | byte W |
| `0x80142699` | byte R |
| `0x80142760` | byte R |
| `0x80143F10` | word W |
| `0x8014401C` | float R/W |
| `0x8014A108` | half R |
| `0x8014A110` | word R |
| `0x8014A118` | byte R |
| `0x8014AA16` | half R |
| `0x8014AA38` | byte R |
| `0x8014B240` | byte R |
| `0x80150EFC` | byte R |
| `0x80150F14` | byte W |
| `0x80151AD0` | half R |
| `0x8015204C` | word R |
| `0x801520C4` | word R |
| `0x8015256C` | byte R |
| `0x801525F4` | float W |
| `0x80152734` | half R |
| `0x80152744` | byte R |
| `0x80152B98` | word R |
| `0x80152BBB` | byte R |
| `0x80152F29` | byte W |
| `0x80153308` | word R |
| `0x801543CC` | float R |

Struct/array patterns include base `0x8014A250` plus `{0x7C6,0x7E8}` and,
most importantly, base `0x80152818` plus `{0x380,0x3A3}` after indexed
address formation. Calls: `init_state_begin` (`0x800C9BE0`),
`init_state_continue` (`0x800FAF6C`), `setup_state_main` (`0x800FB2C8`),
`set_race_state` (`0x800B6138`), unnamed `0x800C813C`,
`camera_race_setup` (`0x800F8D9C`), `race_init_helper` (`0x800EC0DC`),
`race_setup_1` (`0x800BD2C8`), `race_setup_2` (`0x800BEAA0`),
`viScheduleTick` (`0x800014F0`), unnamed `0x800E762C`, `0x800FBF2C`,
`0x800FBE30`, `0x800FBED8`, `0x800B61A8`, `0x800D6160`, and `0x800FBE60`,
`render_viewport_init` (`0x800FA9B4`), unnamed `0x800F7F3C`,
`ghost_race_setup` (`0x800D510C`), `records_screen` (`0x800D58CC`), unnamed
`0x800CF604` and `0x800E7DD0`, `viDeadlinePassed` (`0x800015BC`), `memset`
(`0x80002790`), `speed_set` (`0x800C9210`), unnamed `0x800D5828`,
`cpak_read` (`0x800B0180`), `players_frame_update` (`0x800D5798`), unnamed
`0x800D5374`, `finish_state_normal` (`0x800F64D4`), `finish_state_alt`
(`0x800F8B70`), and unnamed `0x800AB18C`.

## `countdown_handler` (`0x800FBC30..0x800FBE1F`, 496 bytes)

| Global | Width/access |
|---|---|
| `0x801146F8` | word R/W |
| `0x801174B4` | word R |
| `0x8014A108` | half R |
| `0x8014A110` | word R |
| `0x80156CF0` | byte R |
| `0x801613B0` | word W |
| `0x8017A4E4` | word R |

Struct pattern: pointer loaded from global `0x8017A4E4`, offsets
`{0x1F0,0x1F4,0x1F8,0x1FC,0x200}`. Calls: `sprintf` (`0x80004990`),
unnamed `0x800A4770`, `osRecvMesg` (`0x80007270`), `slot_state_setup`
(`0x800B4200`), `osJamMesg` (`0x800075E0`), `dispatch_handler`
(`0x800B74A0`), `state_utility` (`0x800B71D4`), and `object_utility`
(`0x800B3FA4`).

## `Input_ProcessGameplayPad` / `render_scene` (`0x800A04C4..0x800A0F63`, 2,720 bytes)

| Global | Width/access |
|---|---|
| `0x8002AFC0` | word R |
| `0x8002AFC4` | word R |
| `0x8011ED0B` | byte W |
| `0x80124F84` | byte W |
| `0x80140618` | half R |
| `0x801406B8` | word R |
| `0x80140BF0` | word R |
| `0x80140BF4` | word R |
| `0x80140BF8` | half R |
| `0x80140BFA` | half R |
| `0x80140BFC` | half R |
| `0x80140BFE` | half R |
| `0x80140C00` | half R |
| `0x80140C02` | half R |
| `0x80140C04` | byte R |
| `0x80140C05` | byte R |
| `0x80140C06` | byte R |
| `0x80140C08` | half R |
| `0x80140C0A` | half R |
| `0x80140C0C` | half R/W |
| `0x80140C0E` | half R |
| `0x80140C26` | byte R |
| `0x80140D70` | word R |
| `0x80141428` | word R |
| `0x80146204` | byte R |
| `0x801497C8` | word R/W |
| `0x8015F72D` | byte R |
| `0x801613AC` | word R |
| `0x80161434` | word W |
| `0x8017A4B0` | word W |
| `0x8017A508` | word W |
| `0x8017A638` | word W |

Struct patterns: global base `0x80140BF0` has
`{0x00,0x04,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x15,0x16,0x18,0x1A,0x1C,0x1E}`;
the pointer loaded from `0x80140BF0` has `{0x14,0x1C}`; the pointer loaded
from `0x80140BF4` has `{0x10,0x12,0x14,0x15,0x16,0x18}`; and the pointer
loaded from `0x801497C8` has `{0x00,0x04}`. Calls: unnamed `0x8009F058`,
`0x800878E0`, `0x8008A644`, `0x8008705C`, `0x8008A38C`, `0x8008A3E4`,
`0x8008A46C`, and `0x8008A148`, plus `object_render` (`0x80087A08`) and
unnamed `0x80087110`.

## Cross-check of proposed globals

| Belief | Result | Evidence |
|---|---|---|
| `0x801146EC` is `gstate` | **Confirmed** | `game_loop` loads and stores it as a byte immediately around state dispatch. The width implies the N64 representation is an 8-bit state, despite the arcade source's C enum type. |
| `0x801461D0` is a main game struct | **Not confirmed by this cluster** | No target directly forms or dereferences `0x801461D0`. Nearby bytes `0x80146114..16`, `0x801461F8`, and `0x80146205` are accessed, so the wider region is game state, but that does not establish this exact base. |
| `0x80152818` is a player/car array | **Confirmed as an array-like base** | `countdown` forms addresses relative to `0x80152818` and accesses fields at large indexed offsets (`0x380`, `0x3A3`); numerous adjacent per-player globals reinforce the array interpretation. Element type/stride still needs a wider data-flow pass. |
| `0x80142AFC` is a frame counter | **Confirmed** | `game_loop` initializes, reads, increments, and writes the word once per invocation. |
