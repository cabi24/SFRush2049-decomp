# Complete mode configuration initializer

Target `init_state_begin`, 0x800C9BE0, 1816 bytes/454 retail words. Ordinary `void(void)`, frame 48, saves ra and s0..s4. The only external callee takes an actual player pointer, unsigned byte setting index and signed byte setting value. The native body and callee were read from the protected retail words; a derived m2c assembly file was rejected as authority because it rewrites the player field into a spurious global alias.

The first hand-reconstructed whole C (`native.c`) differs at four scheduled words, with two additional section-relative jump-table references explicitly unverified by the single-function scorer. O2 and O3 emit the same full body. The single real source fix is to reset `D_8015F72C` before assigning the word option from configuration[37] in mode 6. That produces zero differing code words (`native_store_order.c`), still unaccepted until both original table references and all seven table entries are verified through the group pipeline. No score masking or instructions were changed.

`group.c` is the publication copy: its first line uses requested O3 flags and the only other changes are comments. It describes the genuine record chain, configuration and mode stores, and original switch provenance. The compiler lane independently proved the publication copy: all 454 linked words match, and all seven original jump-table entries match the native 28 bytes (plus four zero alignment bytes). Its receipt is `../compiler/final_independent.json`; root image and ROM integration remain separate gates. The two emitted zero alignment words are not code coverage.

Arcade search: `reference/repos/rushtherock/game/game.c:game_init` reads EEPROM options and sets runtime flags. That is an analogous subsystem, but no direct counterpart for the N64 player-profile/configuration switch was found. Generic names retain the native symbol naming.

All public receipts contain numeric scores/hashes only. Raw instructions, relocation arrays, objects, disassembly and generated inspection tools are in ignored `build/large_init_state/reconstruction`.
