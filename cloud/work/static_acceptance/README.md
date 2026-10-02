27 additional static functions (12,604 bytes) were strictly verified and normally promoted into six actual ROM translation units. Each promotion passed a full-ROM SHA-1 gate using an isolated watchman2 checkout. Static coverage on this branch is 147/230 functions and 41,336/61,440 bytes (67.28%).

The formatter module now contains all nine native C functions (8,528 bytes). Its complete 89-entry, 356-byte source-built jump table occupies the original 0x8002D558 position; every neighboring original byte is retained. The build validates the table relocations and alignment zeros, preserves meaningful symbols and all instructions, and honors actual input alignment in this explicitly owned container. Other ownership rows keep their defaults and all original address and size assertions remain enabled.

All existing 629 game bodies remain source-built and byte-exact, with the original compressed stream and ROM preserved. All 150 static locks and all blob/group locks hold; full pytest passed with exit 0 (775 passed, 564 skipped). The original common SDK context is unchanged. BSD-derived formatter helpers retain their complete notice in the production destination.

The branch is based on 8199580a; newer game matches on master must be retained when integrating. No shared master sources, lock files, builders or Git refs were changed by this isolated acceptance.
