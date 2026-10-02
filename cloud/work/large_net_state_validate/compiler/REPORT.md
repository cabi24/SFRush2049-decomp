# Native/compiler audit: net_state_validate

Not accepted; no image or coverage credit.

The function is a complete 682-word unlock/achievement table updater. Native
entry consumes no arguments; only calls are four sites of the actual popcount
`func_800B78A4(u32, u8)`. Arcade achievement/unlock searches found no direct
Rush The Rock counterpart. The output table strides and chained offsets were
confirmed from retail native tdis; the mode-C2 first branch clears the third
flag using the player index left one past the final player.

Protected target ELF SHA256:
`8ea6c4ecf649d488e8e691151d36eb784244e52179a58febbf834d009e7ac501`.
Flags: `-g0 -O2 -mips2 -G 0 -non_shared` (canonical scorer applies the recorded
R4300 assembler flag). Toolkit:
`796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`.

Replayed old whole drafts independently. Best aligned baseline reproduces
585/682 exact words in order, but is not a positional match: 661 differing
words and one real excess instruction. Earlier strict baseline retains
289/682 positional differences and only zero alignment beyond the target.
Correcting the actual callee prototype and removing old volatile output-array
qualification leave both scores unchanged.

New controls: signed/plain byte output types; natural 96-byte course and
64-byte car record layouts; independent pointer/counter webs; signed versus
unsigned constant-one carrier; coherent explicit player count initialized in
both first-section branches and refreshed after actual calls; split sum and
pointer arithmetic; pointer assignment inside the real call argument. None
improved the verified baseline. Several controls are byte identical; integer
narrowing or changed pointer lifetimes regress. Workbench diagnosis confirms
the first divergence is a copied count value in the mode-A arm; the saved
pointer and offset colors then exchange. Full raw diagnostic and artifacts
remain under ignored build/large_net_state_validate/compiler/.

The remaining blocker is the natural player-count web and chained pointer
lifetime. No artificial pressure, dummy operations, padding, or argument
changes were introduced. Switching the working pair to the newer four-word
texture rectangle residual offers a stronger next large-function candidate.
