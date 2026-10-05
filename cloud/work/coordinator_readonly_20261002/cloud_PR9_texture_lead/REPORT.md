# Texture rectangle independent compiler audit

Not accepted; no image or coverage credit.

Canonical head `func_80087110`, 445 instructions / 1,780 bytes. The function
clips an input rectangle and issues three real F3DEX2 SDK commands on each
of eight live horizontal/vertical flip and stretch paths. No direct arcade
counterpart was identified; the command macros are traced to the SDK GBI
header. Six ordinary word arguments are actually consumed.

The protected target object SHA256 is
`34c90c28f83d15e9e010e94d3ed3e6509950e98baaea0c2e053ccccd95f525a2`.
Independent baseline SHA256:
`4df5b2f78e7366029e764bed40de4e152edda3ca7c5859007ce795e0edaac8f7`.
Flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
Toolkit: `796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`.

The frozen prior whole draft replays at four positional word differences with
all 445 instructions, a 104-byte frame, and no extra instructions or unresolved
references. Workbench reports a pure ordering change in the horizontal texture
edge addition for the stretched x-flip-only arm; all pool, temporary, and shared
register lanes agree. IDO `cc -c -K` captures real ugen intermediates. The listing
births the display-pointer address immediately after the branch, before the
consumed edge addition. IDO's `-S` driver and as1 `-R` diagnostic mode abort in
wrapper functions on this toolkit; ordinary compiles are unaffected.

Natural edge expression/carrier spellings, unsigned carrier/global types,
ordinary packet pointer capture/advance, and a pointer to the command holder
all failed to improve the verified baseline. Moving the real edge calculation
into the flag condition coalesces away one target instruction. Combining the
edge into the SDK operand likewise changes the instruction population. A
flag/debug ownership probe confirms -O2 and debug builds do not reproduce the
retail body; the -Olimit 2000 control is identical to the recorded baseline.

Raw listings, diagnostics, objects, and instruction arrays remain only under
ignored build/large_texture_rect/compiler/. Numerical strict comparisons are
in control_results.json. No dummy dependencies, volatile operations, padding,
new formal arguments, or custom instruction emission were introduced.

The next pair target is fresh init_state_begin; this four-word schedule
plateau remains a non-match.
