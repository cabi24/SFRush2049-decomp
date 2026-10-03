# Independent native audit: `wheel_render_full`

Target `0x800A6BE4`, complete scanner extent 518 executable words / 2,072 bytes. Historical name is misleading: the routine configures one, two, or four N64 viewports and their scissor rectangles. Three players use the four viewport layout. Native entry consumes one ordinary signed integer argument. Native return reads a signed byte at `D_8017A63C`.

The only callee is `arb_rate_set` (`0x800A5908`). Its entry reads parameters five through nine from caller stack slots 16, 20, 24, 28, 32; its first four arguments are in the ordinary O32 registers. The fourth argument is a float projection/aspect parameter passed as raw bits in `$a3`, not a pointer. The real prototype is nine parameters: signed index, viewport pointer, rectangle pointer, float projection/aspect, then five floats. Caller values for the latter are zero, width, height, horizontal center, vertical center. No extra entry registers or preserved caller-save requirement was observed.

`D_8002AFC0` and `D_8002AFC4` are signed word dimensions. The target reloads them after each call. Signed divisions by powers of two retain round-toward-zero corrections. The target writes rectangles as four 16-bit fields and viewport records at a 72-byte stride. Each record begins with a viewport pointer and rectangle pointer. The single-player rectangle ends use halfword reads at offsets two within the dimension words, as expected for big endian low halves.

Viewport structures are spaced by 16 bytes from `D_8011EA30`. Rectangle globals range from `D_80149870` to `D_80149B78`. `D_80146204` records positive input player count as one byte. `D_8017A63C` becomes zero for one player, one for two players, two for three/four players. Inputs at or below zero and inputs above four return its existing value without configuring a layout. The three/four player path configures all four rectangles.

Four separate float constants (`D_80123BEC`, `D_80123BF0`, `D_80123BF4`, `D_80123BF8`) each decode to 0.95f in the native image. They must remain separate globals because the retail code loads each address individually. Every four-player call multiplies the live `D_80154188` value by its respective constant.

Arcade search: searched `reference/repos/rushtherock/game` for viewport, split screen, screen width, and clip-window terms; no equivalent game routine was found. The arcade `game.map` lists Glide clipping and screen width routines; these are graphics library entries and do not establish common source for the N64 multiplayer viewport layout. Naming and purpose here are inferred from native memory accesses and the callee.

No switch jump table appears in this target. Frame is 72 bytes with five saved integer registers plus return address and outgoing arguments. Protected target object SHA256: `fd0e6aa8c157c1434fd634496fe15f326b9fde1b11cf6b768f1efd8834627bb8`.

## Fresh compiler controls

The real nine arguments can also be declared with two integer address values rather than dereferenceable pointer parameters, because the callee stores their bits into its record. This authentic prototype interpretation changes IDO allocation: the corrected fresh full C at ordinary O2 changes from an 80-byte frame/496 emitted words to a 72-byte frame/504 emitted words. Generic void/char pointers give the former result. Neither interpretation matches all retail words. Treat address typing as a compiler lead, not a proven original declaration.

Independent full-source controls cover integer zero, rectangle signedness, viewport array declarations, real record cursor increments, native low-half reads, exact native store order, original scalar arithmetic carriers, signed short/byte formal controls, O1/O2/O3, g3, Olimit, and real callee context. Narrow input formals add homing operations absent in retail. Complete actual `arb_rate_set` from archived B83 was included and independently compiled through ordinary O2, ordinary O3, and the protected whole-program uld/uopt group pipeline. Its presence gives the same caller body as standalone O3 and does not resolve the match.

Best cursor/integer-address control emits 512 words including alignment versus 518 retail words, with the correct 72-byte frame and 437 aligned opcode matches. Strict comparison still differs in 450 of 518 words. No variant is accepted; no viewport bytes are claimed. Raw objects, words, and workbench diagnosis are retained only under ignored `build/large_viewport/compiler/`; frozen full C and numerical results are in this packet.
