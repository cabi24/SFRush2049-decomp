26 additional static functions (8,656 bytes) were strictly verified and normally promoted into six actual ROM translation units. Each promotion passed a full-ROM SHA-1 gate using an isolated watchman2 checkout. Static coverage on this branch is 146/230 functions and 37,388/61,440 bytes (60.85%).

All existing 629 game bodies remain source-built and byte-exact, with the original compressed stream and ROM preserved. All static/blob/group locks hold; full pytest passed with exit 0 (737 passed, 564 skipped). The original common SDK context is unchanged. BSD-derived formatter helpers retain their complete notice in the production destination.

The branch is based on 8199580a; newer game matches on master must be retained when integrating. No shared master sources, lock files, builders or Git refs were changed by this isolated acceptance. Further C85 ownership/formatter work remains separate until it passes its own normal gates.
