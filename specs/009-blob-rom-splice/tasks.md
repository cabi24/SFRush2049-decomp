# Tasks: 009 blob into the cartridge

- [ ] T001 Vendor zlib 1.0.4 + `deflate104` CLI; reproduce the ROM stream
- [ ] T002 `pipeline/blob_rom.py`: image gate → compress → length + stream pre-flight
- [ ] T003 Makefile: compose `data.o` from prefix + deflate + suffix, no fallback
- [ ] T004 Builder sync + full ROM build + `make test` (SC-001)
- [ ] T005 Drill: altered deflate fails the ROM SHA-1 (SC-002)
- [ ] T006 Coverage: spliced game functions counted as cartridge coverage (SC-003)
- [ ] T007 Tests (length refusal, missing blob, compose slice maths) (SC-004)
- [ ] T008 Docs + close-out (SC-005)
