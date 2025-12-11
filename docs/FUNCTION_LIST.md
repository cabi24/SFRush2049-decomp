# Rush 2049 Function List

## Summary
| Category | Count | Address Range |
|----------|-------|---------------|
| Static ROM (named) | 720 | 0x80000400 - 0x8000E800 |
| Game Code (dynamic) | 752 | 0x80086A50 - 0x80124AF0 |
| **Total** | **~1472** | |

## Static ROM Functions (symbol_addrs.us.txt)

These are in the uncompressed ROM and loaded at boot:

### Startup (4)
- entrypoint @ 0x80000400
- main @ 0x800020F0  
- idle_thread_entry @ 0x800021A4
- audio_thread_entry @ 0x800024FC

### libc (8)
- memchr @ 0x80002730
- memset @ 0x80002790
- strchr @ 0x80007C00
- strlen @ 0x80007C40
- memcpy @ 0x80007C68
- bcopy @ 0x8000D2B0
- bzero @ 0x80008590

### libm (10)
- modf @ 0x800028A0
- modff @ 0x80002A64
- sinf @ 0x80008730
- cosf @ 0x800088F0
- __isinf @ 0x80002BF0
- __isnan @ 0x80002C60
- fcvt @ 0x80002CD0
- __ecvt_internal @ 0x80003C3C
- __round_helper @ 0x80004674

### libgcc (17)
- __fixdfdi @ 0x800099F0
- __floatdidf @ 0x80009B64
- __lshrdi3 @ 0x8000A070
- __ashldi3 @ 0x8000A0A4
- __udivdi3 @ 0x8000A198
- __muldi3 @ 0x8000A278
- (etc)

### libultra OS (~100)
- osCreateMesgQueue @ 0x80006A00
- osSendMesg @ 0x80007440
- osRecvMesg @ 0x80007270
- osCreateThread @ 0x80006F30
- osStartThread @ 0x80007080
- osPiStartDma @ 0x8000DCD0
- osViInit @ 0x8000C540
- __osException @ 0x8000C980
- (many more)

### Inflate/Decompress (16)
- inflate_entry @ 0x80006814
- inflate_loop @ 0x80006678
- huft_build @ 0x80004D98
- lzss_decode @ 0x80004AFC
- (etc)

### Scheduler (13)
- osCreateScheduler @ 0x80000450
- __scMain @ 0x8000063C
- __scHandleRetrace @ 0x80000880
- __scExecTask @ 0x80000C54
- (etc)

### Display (8)
- display_update @ 0x800015F0
- viewport_setup @ 0x80001B44
- get_viewport_pos @ 0x80001E84
- (etc)

## Game Code Functions (build/game_code_disasm.txt)

These are decompressed from ROM at runtime to 0x80086A50:

### Key Functions (identified)
| Address | Size | Name | Description |
|---------|------|------|-------------|
| 0x800FD464 | 704 | game_loop | Main per-frame game logic |
| 0x800CA3B4 | 2560 | playgame | PLAYGAME state handler |
| 0x800DB81C | - | attract | ATTRACT mode |
| 0x80099BFC | 10048 | render_object | 3D model rendering |
| 0x80087A08 | 10048 | render_large | Major rendering |
| 0x800A04C4 | 2752 | render_scene | Viewport/camera |
| 0x800B37E8 | - | sound_control | Audio |

### All 752 Game Code Functions
See `build/game_code_disasm.txt` header for full list with sizes.

## Tools

```bash
# List all named symbols
grep -E '^\w+\s*=\s*0x' symbol_addrs.us.txt

# Extract specific function from game code
./tools/extract_func.py 0x800FD464

# Get call graph
./tools/call_graph.py 0x800FD464 --depth 2

# Decompile with Ghidra (on OptiPlex)
./tools/ghidra_decompile.sh func 0x800FD464
```
