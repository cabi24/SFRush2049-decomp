# Collision module compiler audit — frozen nonmatch

No source credit is claimed. All comparisons use the protected scorer and genuine C89 sources. Raw objects, target disassembly and workbench output remain in ignored build storage. Accepted sources and compiler tooling are unchanged.

## Native identities and ABI

| Native symbol | Arcade ancestor | Complete runtime extent | Observed interface |
| --- | --- | --- | --- |
| `menu_options_screen` | `collision.c:setFBCollisionForce` | 589 words / 2356 bytes | Five consumed pointer inputs; 152-byte frame, only return address saved |
| `menu_load_options` | `collision.c:ForceApart` | 91 words / 364 bytes | Four pointer inputs; 80-byte frame, only return address saved |
| `menu_audio_settings` | `collision.c:collision` | 204 words / 816 bytes | Accepted caller; one model pointer, pair iteration |
| `func_800CEC8C` | `collision.c:PointInBody` | 44 words / 176 bytes | Accepted native extension: model, point and float height |

The accepted collision caller supplies a sixth outgoing height argument to `menu_options_screen`; the callee never consumes it. The direct donor has five inputs. ForceApart's donor fifth `pos` input is unused and absent from the native callee. No helper relies on an incoming hidden register.

The large function retains the donor relative-position and velocity transforms, body bounds, overlap dispatch, per-axis contact force and clamps. N64 adds item effects and divides the opposite center forces using both cars' inverse masses. `menu_save_options` is an unrelated menu function despite its neighboring address.

Native used model offsets include body bounds at 244/280, center force 292, world velocity 544, inverse mass 1476, collision threshold 1596, controlled flag 1600, hit target 1732, reckon velocity 1928, position 1940, basis 1952, player 1990 and mode 1996. Struct gaps describe these native fields; no extra local storage was introduced.

The `func_8038d3a4` call consumes two car pointers and one integer amount; its return is unused. Its identity beyond the native address is unproven, so the source provides only the observed external declaration.

## Bounded source controls

ForceApart improved from 75 differing words in the first typed-vector form to 11 using actual float arrays, the donor left-associated dot product, original local ownership and center-force-first addition. Its complete 91-word body and 80-byte frame are preserved. The remaining changes are early floating-point register allocation and the actual three-element loop. Reusing the original donor distance scalar on this best form is a distinct grounded control, but worsens the result to 25 words.

`collision_best_complete_context_group` combines reconstruction's best full collision source, the best ForceApart body and unchanged accepted transform/norm bodies in one genuine translation unit. True whole O3 retains all four entries. Results:

| Member | Differing / target words |
| --- | --- |
| Collision force | 448 / 589 |
| ForceApart | 11 / 91 |
| Accepted transform | 0 / 37 |
| Accepted norm | 0 / 11 |

There are no unresolved references, unverified references, scorer errors or extra words. Collision force has 588 runtime words and its native 152-byte frame. The one-word deficit occurs in the body; the native final delay slot is part of its actual extent. Complete context improves the standalone comparison by one word and does not solve the register and instruction differences. No pool or whole-body equality is claimed.

Protected decoded constants were kept distinct: ForceApart threshold approximately 0.0001 and two 40000 entries; collision cooldowns approximately 0.3333333134651184 and two 20000 entries. The cooldown is one float ULP below the nearest representation of one third. Literal ownership experiments did not solve ForceApart and their local pool references remain unverified.

Full numerical scores, source/object hashes and protected target hashes are in [bounded_audit.json](bounded_audit.json). Both helper and large caller are frozen after these distinct donor and genuine shared-context controls.

## Adjacent rotation context

The accepted positions wrapper `sound_position_set` consumes two pointers, has a 24-byte frame and applies three rotations in Y/X/Z order. Each native axis helper consumes an ordinary float and basis pointer. Accepted `menu_video_settings` normalizes the three actual model basis rows through the accepted one-pointer reciprocal norm helper. Their calling conventions do not establish hidden parameter storage for the frozen positions reconstruction.
