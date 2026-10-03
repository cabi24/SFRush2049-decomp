# A130 frozen native resource patch loop

listener_position_set@800B2D20,216 bytes/54 words, strict MATCH. Source SHA-256 `6fedb766211657784fad4b0e5d1b6c7db816de3f9c169f69ef473b631df93b22`; literal flags `-g0 -O3 -mips2 -G 0 -non_shared`. Complete original words independently verified with no masks, unresolved/unverified references, errors or extras. Publication claims empty pending coordinator replay/gates.

Complete single consumed ID input and original dynamic global record count. Each32-byte patch record has unsigned ID/index at0/2,18-byte payload4,companion index22,8-byte payload24. Matching IDs copy18 bytes to payload4 of actual24-byte records and8 bytes to the original companion byte storage at index*8. Count reload and cursor increment preserve original mutation semantics across memcpy. Layout gaps describe existing objects only; no stack arrays or synthetic work.

First source used an array-of8 view of companion storage and was4/54. The original second address explicitly computes the consumed byte offset before adding storage base, whereas first address adds the base before payload adjustment. Replacing the companion declaration with its actual byte pointer and expressing the original consumed byte-offset-plus-pointer address recovered all54 words. The initial complete source and proof are archived separately. No pressure, declaration-order, context or padding controls were applied.

All Markdown name/address screening and accepted locked-interval check found no existing packet/accepted body covering this function. No shared accepted sources, locks, layout, gates or commits were altered.
