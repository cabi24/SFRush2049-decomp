# ShowHUD: conditional native equality and genuine helper context

Research-only packet for historical N64 label `skid_mark_render`, address
0x800F0100, 1,396 bytes. Its actual behavior is ShowHUD, including a timed
credits pane. It does not render skid marks or tutorial tips.

The root strictly reproduces all 349 native words and the native 64-byte
frame in this graph. The original function-local static floats, credit-name
for-loop and x-before-y corner assignment order explain the prior residual.
No added automatic storage, fake caller, forced register or instruction/line
annotation was used.

**This exact result remains conditional.** The source retains an inherited
volatile declaration for the N64 seconds clock. The later-platform donor says
plain float; that does not establish the original N64 qualifier. Removing
volatile produces 190/349 differing words and unresolved data-reference
mapping. The packet records that as a source-evidence control, not as proof
that an ordinary clock is wrong. Matching instructions do not authenticate
historical volatility. The native symbol map identifies this same address as
boot-library __osScElapsedTime; the promoted lib_1050 source declares that
ordinary float. An existing overlay reconstruction separately discloses its
volatile clock qualification as unproven. Thus no consistent authenticated
native qualification contract is established here. No installable source
boundary is claimed.

## Source evidence and platform distinctions

The available MAT3 ShowHUD is 0x8001C980, 904 bytes. Original DWARF confirms
void return, no parameters, named locals int init/x/y, and function-local
static float delay/nextSwitch. The saved reconstruction additionally uses a
block-local initialization index absent from that original local list. This
packet retains a used block-local index and labels it reconstruction-only.
A separate diagnostic reusing x preserves every native receipt and every
emitted code/data/relocation/symbol section; only debug metadata differs.
That equivalence does not recover the historical spelling.

The older MAT3 report records ShowHUD at 100%, but current hud.c is newer
and has a disclosed partial-source warning. This is saved source/type
evidence, not a freshly verified matching donor. Source SHA256:
93fc1f0f95d96be48706e68acf92a76c76e354197e6255fbed1227b603fe9924.

Native platform behavior is preserved independently: the race/HUD blit counts
are 89 and 8, credits divide the timer by 13, the bottom position uses 145,
and multiplayer overlays retain their N64 availability guard. The later
platform's corresponding values are 90/5, 14 and 149; they are not imported.

The two original static floats each occupy four bytes. The strict scorer
places their references independently at 0x8015462C and 0x80154644. This is
**BSS address-only verification**. It does not compare ROM content or prove
a complete BSS layout. The original type evidence independently supplies the
float type and extent; the packet does not claim neighboring storage.

## Actual helper graph and all failures

The 32 known native bodies are explicit members. Both emitted eight-byte
queue inlining stubs are inventoried separately. There are 28 strict bodies
and four partial bodies:

- object_bytes23_sum: 4/18 words differ
- object_bytes_sum_global: 4/21 words differ
- world_velocity_integrate: 158/178 differ, three excess words and two
  unverified own-table references
- sound_handles_clear / RemovePanes: 43/44 differ, five excess words;
  frame56 versus native48

The RemovePanes body is the genuine two-int-index reconstruction from a
complete Matching later pane TU. It removes the old compiled-out loop device
and temporary pane/handle locals, but remains partial in N64. Its presence
does not disturb the exact HUD root. Original pane.c source SHA256:
c90f3e4564bc39ac50a4e4c2219c3d7eba98b85534229247a13ca4ca7ea4b2d1.

Real InitPane, ChangePaneColor, ChangePaneSize and NewMultiBlit are retained
and strict. The Blit record is 64 bytes; pane is 32 bytes with five pointers
at +12; MultiBlit is 36 bytes. The table loader and HUD share one complete
40-byte native resource-table view. All queue declarations use one opaque
tagged type. Spacing/leading globals are signed bytes, as original font type
evidence records. Font and resource-slot definitions also share their complete
16/20-byte native views; the character-width return is unsigned and the
two-getter height wrapper has no named local, following original evidence.
Native target-compiler checks verify 18 record sizes and
offsets. Opaque gaps describe native record layout, not automatic frame filler.

The exact Random/rand decomposition is reused from the existing independent
music research: native func_8008B2E4(float) calls the genuine global
func_8008B2B4(void). Both bodies stay strict. No private rand duplicate is
introduced. Original Random has one float parameter and no named locals;
source SHA256 dc2b38176f2cde208697603c632cdffcc62d60ccc1d5da0f8b990fc9af7edbc5.

The single HAL_SetFont uses its actual queue helpers, lower setter and real
reset/replay/resource context. Existing font/resource context still contains
disclosed empty or unreachable inline-blocker syntax. Those inherited devices
are neither authenticated by the new donor nor granted new credit. The two
font-height partials and replay failure are preserved. Width, countdown,
ShowHUDExtra and low-level rendering remain real external call boundaries.
This is not a complete transitive implementation of every external dependency.

## Reproduction and controls

With the repository's authorized IDO/MIPS tools configured, run:

    python cloud/work/frontier/dot_showhud_research_20261017/verify.py
    python -O cloud/work/frontier/dot_showhud_research_20261017/verify.py
    python -m unittest tests/cloud/test_dot_showhud_research.py

The verifier pins all source files, all 32 target hashes, comparison tools,
proof implementation and complete receipts. All checks use explicit exceptions
and remain active under Python -O. Stock, retained-stage stock and
observation-only traced objects are identical; hashes are in fidelity.json.
No object, trace, ROM bytes or native instruction dump is distributed.

Nine causal negatives reject: externalized static storage, old while-loop,
reversed corner assignments, wrong credit divisor, wrong font, wrong native
HUD count, wrong coin-record stride, wrong bottom position and wrong pane
permanence. The two source-evidence controls separately retain the ordinary
clock limitation and three-local diagnostic equivalence. Compared byte values
and spurious native address values are scrubbed from error receipts.

A private source-shadow diagnostic also preserves the exact root. It deletes
HAL_SetFont's standalone body and changes the already-partial replay receipt;
those failures prevent unit acceptance. It performs no canonical installation,
ROM build or complete-image proof. Claims remain empty and new accepted credit
is zero. This packet is for independent review; merging is left to the checker.
