# Independent review

Lane E independently reviewed this packet on 2026-10-02 UTC and reported PASS
for research publication. Its fresh IDO replay was exactly equal to
`verification.json`; all three focused tests passed.

The reviewer separately inspected the native prologue: `ra`, `s0–s8` and six
double-precision save slots total 88 bytes, in a 472-byte frame; the reset helper
has a 16-byte frame saving `s0/f20`. It reproduced root 1361/1413 differing words,
reset 207/211, and accepted interpolation 0/52, plus the root's 5272-byte ELF body
and 12-byte padding. It agreed that the no-go and semantic limitations were
honest and found no publication blocker.

The author also ran the three focused tests, fresh full group replay, canonical
single-function and three-member compiler controls (all MATCH), and static lock
check (all 161 intact). No semantic gameplay or ROM tests were performed.
