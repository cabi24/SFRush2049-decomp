# Tasks: 009 blob into the cartridge

- [x] T001 Vendor zlib 1.0.4 + `deflate104` CLI; reproduce the ROM stream
- [x] T002 `pipeline/blob_rom.py`: image gate → compress → length + stream pre-flight
- [x] T003 Makefile: compose `data.o` from prefix + deflate + suffix, no fallback
- [x] T004 Builder sync + full ROM build + `make test` (SC-001)
- [x] T005 Drill: altered deflate fails the ROM SHA-1 (SC-002)
- [x] T006 Coverage: spliced game functions counted as cartridge coverage (SC-003)
- [x] T007 Tests (length refusal, missing blob, compose slice maths) (SC-004)
- [x] T008 Docs + close-out (SC-005)
