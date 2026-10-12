// Controller pak (Phase 6): a 32 KiB memory pak on controller 1, kept in the
// standard .mpk layout (ID area, inode table + backup, note table, 123 data
// pages; the format emulators use) and backed by a file under --home.
//
// N64ModernRuntime cdf5abbd implements the osPfs* calls as "no pak"
// (librecomp/src/pak.cpp: every call returns PFS_ERR_NOPACK), so the game
// shows NO CONTROLLER PAK DETECTED at boot and never saves. The recompiled
// game's calls are renamed to the rush_* versions below at compile time
// (CMakeLists RUSH_PAK_RENAMED, the same mechanism as RUSH_WRAPPED), so the
// runtime's stubs stay linked but unused. The game's own libultra pfs code is
// not emitted (N64Recomp replaces osPfs* by name), so these are high-level
// emulations of libultra's osPfs* semantics (return codes as libultra 2.0)
// over the image; the image itself stays consistent (checksums, inode chains)
// so it can be opened by other tools.
//
// Modes (main.cpp): wall clock: --home/rush2049.mpk, created formatted when
// missing, rewritten after every change (temp file + rename). --clock frames
// (lockstep): a freshly formatted image in memory, never written (every run
// starts from the same bytes: deterministic), unless --pak FILE names a
// starting image (still not written). --no-pak: the runtime's behaviour (no
// pak; the boot notice).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

#include "recomp.h"
#include "ultramodern/ultramodern.hpp"
#include "rush_runtime.h"

namespace {

constexpr size_t PAK_SIZE = 0x8000;
constexpr int PAGE = 256;
constexpr int PAGES = 128;
constexpr int FIRST_DATA_PAGE = 5;   // 0 ID, 1 inode, 2 inode backup, 3-4 notes
constexpr int NOTES = 16;
constexpr int BLOCK = 32;            // read/write granularity (BLOCKSIZE)
constexpr uint16_t INODE_END = 0x0001, INODE_FREE = 0x0003;

enum : int32_t {
    PFS_ERR_NOPACK = 1, PFS_ERR_NEW_PACK = 2, PFS_ERR_INCONSISTENT = 3, PFS_ERR_CONTRFAIL = 4,
    PFS_ERR_INVALID = 5, PFS_ERR_BAD_DATA = 6, PFS_DATA_FULL = 7, PFS_DIR_FULL = 8, PFS_ERR_EXIST = 9,
};

std::mutex mu;
bool enabled = false;          // a pak is plugged into controller 1
bool persist = false;          // rewrite the file after every change
std::filesystem::path file;
std::vector<uint8_t> img(PAK_SIZE);
uint64_t writes = 0;            // game changes (allocate, delete, write): status pak_writes=

uint16_t be16(const uint8_t* p) { return (uint16_t)(p[0] << 8 | p[1]); }
uint32_t be32(const uint8_t* p) { return (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3]; }
void put16(uint8_t* p, uint16_t v) { p[0] = (uint8_t)(v >> 8); p[1] = (uint8_t)v; }
void put32(uint8_t* p, uint32_t v) { p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16); p[2] = (uint8_t)(v >> 8); p[3] = (uint8_t)v; }

uint8_t* inode() { return img.data() + 1 * PAGE; }
uint8_t* note(int n) { return img.data() + 3 * PAGE + n * 32; }
uint16_t inode_get(int page) { return be16(inode() + 2 * page); }

// libultra __osSumcalc over the data part of the inode page, stored in entry 0;
// the backup page (2) mirrors page 1.
void inode_commit() {
    uint8_t* in = inode();
    int sum = 0;
    for (int i = 2 * FIRST_DATA_PAGE; i < PAGE; i++) sum += in[i];
    in[0] = 0;
    in[1] = (uint8_t)sum;
    std::memcpy(img.data() + 2 * PAGE, in, PAGE);
}

void format() {
    std::fill(img.begin(), img.end(), 0);
    // Block 0: label (mupen64plus' formatted pak); ID block at 0x20 with copies
    // at 0x60, 0x80, 0xC0 (__OSPackId: repaired, random, serial, deviceid, banks,
    // version, checksum, 0xFFF2 - checksum).
    img[0] = 0x81;
    for (int i = 1; i < 32; i++) img[i] = (uint8_t)i;
    uint8_t id[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0x05, 0x1A, 0x5F, 0x13};
    put16(id + 0x18, 0x0001);   // deviceid
    id[0x1A] = 1;               // banks
    id[0x1B] = 0;               // version
    uint16_t sum = 0;
    for (int i = 0; i < 0x1C; i += 2) sum = (uint16_t)(sum + be16(id + i));
    put16(id + 0x1C, sum);
    put16(id + 0x1E, (uint16_t)(0xFFF2 - sum));
    for (int off : {0x20, 0x60, 0x80, 0xC0}) std::memcpy(img.data() + off, id, 32);
    for (int p = FIRST_DATA_PAGE; p < PAGES; p++) put16(inode() + 2 * p, INODE_FREE);
    inode_commit();
}

void save_locked() {
    if (!persist) return;
    std::filesystem::path tmp = file;
    tmp += ".tmp";
    {
        std::ofstream f(tmp, std::ios::binary | std::ios::trunc);
        f.write(reinterpret_cast<const char*>(img.data()), (std::streamsize)img.size());
        if (!f) { fprintf(stderr, "[rush] pak: cannot write %s\n", tmp.string().c_str()); return; }
    }
    std::error_code ec;
    std::filesystem::rename(tmp, file, ec);
    if (ec) fprintf(stderr, "[rush] pak: cannot replace %s: %s\n", file.string().c_str(), ec.message().c_str());
}

int free_pages() {
    int n = 0;
    for (int p = FIRST_DATA_PAGE; p < PAGES; p++) n += inode_get(p) == INODE_FREE;
    return n;
}

bool note_used(int n) { return be16(note(n) + 4) != 0 && be32(note(n)) != 0; }

// The data pages of a note in order; empty when the chain is broken.
std::vector<int> chain(int n) {
    std::vector<int> pages;
    uint16_t p = be16(note(n) + 6);
    while (p >= FIRST_DATA_PAGE && p < PAGES && (int)pages.size() < PAGES) {
        pages.push_back(p);
        uint16_t next = inode_get(p);
        if (next == INODE_END) return pages;
        p = next;
    }
    return {};
}

// --- guest memory (rdram is word-swapped; MEM_* do the address fix-up)
gpr G(uint32_t a) { return (gpr)(int32_t)a; }
uint8_t gb(uint8_t* rdram, uint32_t a) { return MEM_BU(0, G(a)); }
void sb(uint8_t* rdram, uint32_t a, uint8_t v) { MEM_B(0, G(a)) = (int8_t)v; }
void sw(uint8_t* rdram, uint32_t a, uint32_t v) { MEM_W(0, G(a)) = (int32_t)v; }
uint32_t lw(uint8_t* rdram, uint32_t a) { return (uint32_t)MEM_W(0, G(a)); }
uint32_t arg(uint8_t* rdram, recomp_context* ctx, int n) {
    switch (n) {
        case 0: return (uint32_t)ctx->r4;
        case 1: return (uint32_t)ctx->r5;
        case 2: return (uint32_t)ctx->r6;
        case 3: return (uint32_t)ctx->r7;
        default: return lw(rdram, (uint32_t)ctx->r29 + 4 * n);
    }
}
void ret(recomp_context* ctx, int32_t v) { ctx->r2 = (gpr)v; }

// OSPfs (0x68 bytes): status, queue, channel, id[32], label[32], version,
// dir_size, inode_table, minode_table, dir_table, inode_start_page, banks, activebank.
int32_t pfs_channel(uint8_t* rdram, uint32_t pfs) { return (int32_t)lw(rdram, pfs + 8); }
bool pak_on(uint8_t* rdram, uint32_t pfs) { return enabled && pfs_channel(rdram, pfs) == 0; }

int find(uint8_t* rdram, uint16_t company, uint32_t game, uint32_t name, uint32_t ext) {
    for (int n = 0; n < NOTES; n++) {
        const uint8_t* d = note(n);
        if (be16(d + 4) != company || be32(d) != game) continue;
        bool same = true;
        for (int i = 0; name && same && i < 16; i++) same = d[16 + i] == gb(rdram, name + i);
        for (int i = 0; ext && same && i < 4; i++) same = d[12 + i] == gb(rdram, ext + i);
        if (same) return n;
    }
    return -1;
}

const char* ascii_name(const uint8_t* d, char* out) {
    // N64 font codes -> ASCII for the log (0x0F space, 0x10-0x19 digits, 0x1A-0x33 A-Z)
    int k = 0;
    for (int i = 0; i < 16; i++) {
        uint8_t c = d[16 + i];
        if (c == 0) break;
        out[k++] = c == 0x0F ? ' ' : c >= 0x10 && c <= 0x19 ? (char)('0' + c - 0x10)
                 : c >= 0x1A && c <= 0x33 ? (char)('A' + c - 0x1A) : '?';
    }
    out[k] = 0;
    return out;
}

} // namespace

void rush::pak_setup(bool on, const std::filesystem::path& path, bool persistent) {
    std::lock_guard<std::mutex> lock(mu);
    enabled = on;
    persist = persistent;
    file = path;
    if (!on) {
        fprintf(stderr, "[rush] pak: none (controller 1 reports no controller pak)\n");
        return;
    }
    format();
    const char* how = "new";
    if (!path.empty() && std::filesystem::exists(path)) {
        std::ifstream f(path, std::ios::binary);
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        if (data.size() == PAK_SIZE) {
            img = data;
            how = "loaded";
        } else {
            fprintf(stderr, "[rush] pak: %s is %zu bytes, not 32768: using a fresh image\n",
                    path.string().c_str(), data.size());
        }
    }
    int used = 0;
    for (int n = 0; n < NOTES; n++) used += note_used(n);
    fprintf(stderr, "[rush] pak: controller 1, %s %s (%d notes used, %d free pages)%s\n", how,
            path.empty() ? "(in memory)" : path.string().c_str(), used, free_pages(),
            persist ? "" : ", not saved (fixed image)");
    if (persist && std::strcmp(how, "new") == 0) save_locked();
}

bool rush::pak_present(int channel) { return enabled && channel == 0; }
uint64_t rush::pak_writes() { return writes; }

#define PAKFN(name) extern "C" void rush_##name##_recomp(uint8_t* rdram, recomp_context* ctx)

// s32 osPfsInitPak(OSMesgQueue* mq, OSPfs* pfs, int channel)
PAKFN(osPfsInitPak) {
    std::lock_guard<std::mutex> lock(mu);
    uint32_t mq = arg(rdram, ctx, 0), pfs = arg(rdram, ctx, 1);
    int32_t channel = (int32_t)arg(rdram, ctx, 2);
    static bool first = true;
    if (first) {
        first = false;
        rush::trace_event("pak: osPfsInitPak(channel %d) -> %s", channel,
                          enabled && channel == 0 ? "ok" : "PFS_ERR_NOPACK");
    }
    if (!(enabled && channel == 0)) { ret(ctx, PFS_ERR_NOPACK); return; }
    sw(rdram, pfs + 0x00, 1);              // PFS_INITIALIZED
    sw(rdram, pfs + 0x04, mq);
    sw(rdram, pfs + 0x08, (uint32_t)channel);
    for (int i = 0; i < 32; i++) sb(rdram, pfs + 0x0C + i, img[0x20 + i]);
    for (int i = 0; i < 32; i++) sb(rdram, pfs + 0x2C + i, img[i]);
    sw(rdram, pfs + 0x4C, img[0x20 + 0x1B]);   // version
    sw(rdram, pfs + 0x50, NOTES);              // dir_size
    sw(rdram, pfs + 0x54, 1 * 8);              // inode_table (block)
    sw(rdram, pfs + 0x58, 2 * 8);              // minode_table
    sw(rdram, pfs + 0x5C, 3 * 8);              // dir_table
    sw(rdram, pfs + 0x60, FIRST_DATA_PAGE);    // inode_start_page
    sb(rdram, pfs + 0x64, 1);                  // banks
    sb(rdram, pfs + 0x65, 0);                  // activebank
    ret(ctx, 0);
}

// s32 osPfsRepairId(OSPfs*), s32 osPfsChecker(OSPfs*): the image is kept consistent.
PAKFN(osPfsRepairId) { ret(ctx, pak_on(rdram, arg(rdram, ctx, 0)) ? 0 : PFS_ERR_NOPACK); }
PAKFN(osPfsChecker) { ret(ctx, pak_on(rdram, arg(rdram, ctx, 0)) ? 0 : PFS_ERR_NOPACK); }

// s32 osPfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used)
PAKFN(osPfsFreeBlocks) {
    std::lock_guard<std::mutex> lock(mu);
    if (!pak_on(rdram, arg(rdram, ctx, 0))) { ret(ctx, PFS_ERR_NOPACK); return; }
    sw(rdram, arg(rdram, ctx, 1), (uint32_t)(free_pages() * PAGE));
    ret(ctx, 0);
}

// s32 osPfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used)
PAKFN(osPfsNumFiles) {
    std::lock_guard<std::mutex> lock(mu);
    if (!pak_on(rdram, arg(rdram, ctx, 0))) {
        sw(rdram, arg(rdram, ctx, 1), 0);
        sw(rdram, arg(rdram, ctx, 2), 0);
        ret(ctx, PFS_ERR_NOPACK);
        return;
    }
    int used = 0;
    for (int n = 0; n < NOTES; n++) used += note_used(n);
    sw(rdram, arg(rdram, ctx, 1), NOTES);
    sw(rdram, arg(rdram, ctx, 2), (uint32_t)used);
    ret(ctx, 0);
}

// s32 osPfsFindFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name,
//                   u8* ext_name, s32* file_no)
PAKFN(osPfsFindFile) {
    std::lock_guard<std::mutex> lock(mu);
    uint32_t file_no = arg(rdram, ctx, 5);
    if (!pak_on(rdram, arg(rdram, ctx, 0))) { ret(ctx, PFS_ERR_NOPACK); return; }
    uint16_t company = (uint16_t)arg(rdram, ctx, 1);
    uint32_t game = arg(rdram, ctx, 2);
    if (company == 0 || game == 0) { ret(ctx, PFS_ERR_INVALID); return; }
    int n = find(rdram, company, game, arg(rdram, ctx, 3), arg(rdram, ctx, 4));
    sw(rdram, file_no, (uint32_t)n);
    ret(ctx, n < 0 ? PFS_ERR_INVALID : 0);
}

// s32 osPfsAllocateFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name,
//                       u8* ext_name, int file_size_in_bytes, s32* file_no)
PAKFN(osPfsAllocateFile) {
    std::lock_guard<std::mutex> lock(mu);
    if (!pak_on(rdram, arg(rdram, ctx, 0))) { ret(ctx, PFS_ERR_NOPACK); return; }
    uint16_t company = (uint16_t)arg(rdram, ctx, 1);
    uint32_t game = arg(rdram, ctx, 2), name = arg(rdram, ctx, 3), ext = arg(rdram, ctx, 4);
    int32_t size = (int32_t)arg(rdram, ctx, 5);
    uint32_t file_no = arg(rdram, ctx, 6);
    if (company == 0 || game == 0) { ret(ctx, PFS_ERR_INVALID); return; }
    int existing = find(rdram, company, game, name, ext);
    sw(rdram, file_no, (uint32_t)existing);
    if (existing >= 0) { ret(ctx, PFS_ERR_EXIST); return; }
    int pages = (size + PAGE - 1) / PAGE;
    if (size > free_pages() * PAGE) { ret(ctx, PFS_DATA_FULL); return; }
    if (pages <= 0) { ret(ctx, PFS_ERR_INVALID); return; }
    int n = 0;
    while (n < NOTES && note_used(n)) n++;
    if (n == NOTES) { ret(ctx, PFS_DIR_FULL); return; }
    std::vector<int> list;
    for (int p = FIRST_DATA_PAGE; p < PAGES && (int)list.size() < pages; p++)
        if (inode_get(p) == INODE_FREE) list.push_back(p);
    for (size_t i = 0; i < list.size(); i++)
        put16(inode() + 2 * list[i], i + 1 < list.size() ? (uint16_t)list[i + 1] : INODE_END);
    inode_commit();
    uint8_t* d = note(n);
    std::memset(d, 0, 32);
    put32(d, game);
    put16(d + 4, company);
    put16(d + 6, (uint16_t)list[0]);
    d[8] = 0x02;   // DIR_STATUS_OCCUPIED
    for (int i = 0; i < 4; i++) d[12 + i] = ext ? gb(rdram, ext + i) : 0;
    for (int i = 0; i < 16; i++) d[16 + i] = name ? gb(rdram, name + i) : 0;
    for (int p : list) std::memset(img.data() + p * PAGE, 0, PAGE);
    writes++;
    save_locked();
    char nm[17];
    rush::oracle_event("pak allocate");
    rush::trace_event("pak: allocated note %d \"%s\" (company 0x%04X game 0x%08X), %d pages", n,
                      ascii_name(d, nm), company, game, pages);
    sw(rdram, file_no, (uint32_t)n);
    ret(ctx, 0);
}

// s32 osPfsDeleteFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name)
PAKFN(osPfsDeleteFile) {
    std::lock_guard<std::mutex> lock(mu);
    if (!pak_on(rdram, arg(rdram, ctx, 0))) { ret(ctx, PFS_ERR_NOPACK); return; }
    uint16_t company = (uint16_t)arg(rdram, ctx, 1);
    uint32_t game = arg(rdram, ctx, 2);
    if (company == 0 || game == 0) { ret(ctx, PFS_ERR_INVALID); return; }
    int n = find(rdram, company, game, arg(rdram, ctx, 3), arg(rdram, ctx, 4));
    if (n < 0) { ret(ctx, PFS_ERR_INVALID); return; }
    for (int p : chain(n)) put16(inode() + 2 * p, INODE_FREE);
    inode_commit();
    std::memset(note(n), 0, 32);
    writes++;
    save_locked();
    rush::trace_event("pak: deleted note %d", n);
    ret(ctx, 0);
}

// s32 osPfsFileState(OSPfs* pfs, s32 file_no, OSPfsState* state)
// OSPfsState: u32 file_size, u32 game_code, u16 company_code, char ext_name[4], char game_name[16].
PAKFN(osPfsFileState) {
    std::lock_guard<std::mutex> lock(mu);
    if (!pak_on(rdram, arg(rdram, ctx, 0))) { ret(ctx, PFS_ERR_NOPACK); return; }
    int32_t n = (int32_t)arg(rdram, ctx, 1);
    uint32_t st = arg(rdram, ctx, 2);
    if (n < 0 || n >= NOTES || !note_used(n)) { ret(ctx, PFS_ERR_INVALID); return; }
    std::vector<int> pages = chain(n);
    if (pages.empty()) { ret(ctx, PFS_ERR_INCONSISTENT); return; }
    const uint8_t* d = note(n);
    sw(rdram, st + 0, (uint32_t)(pages.size() * PAGE));
    sw(rdram, st + 4, be32(d));
    sb(rdram, st + 8, d[4]);
    sb(rdram, st + 9, d[5]);
    for (int i = 0; i < 4; i++) sb(rdram, st + 10 + i, d[12 + i]);
    for (int i = 0; i < 16; i++) sb(rdram, st + 14 + i, d[16 + i]);
    ret(ctx, 0);
}

// s32 osPfsReadWriteFile(OSPfs* pfs, s32 file_no, u8 flag (0 read, 1 write),
//                        int offset, int size_in_bytes, u8* data_buffer)
PAKFN(osPfsReadWriteFile) {
    std::lock_guard<std::mutex> lock(mu);
    if (!pak_on(rdram, arg(rdram, ctx, 0))) { ret(ctx, PFS_ERR_NOPACK); return; }
    int32_t n = (int32_t)arg(rdram, ctx, 1);
    uint8_t flag = (uint8_t)arg(rdram, ctx, 2);
    int32_t offset = (int32_t)arg(rdram, ctx, 3), size = (int32_t)arg(rdram, ctx, 4);
    uint32_t buf = arg(rdram, ctx, 5);
    if (n < 0 || n >= NOTES || size <= 0 || size % BLOCK || offset < 0 || offset % BLOCK) {
        ret(ctx, PFS_ERR_INVALID);
        return;
    }
    if (!note_used(n)) { ret(ctx, PFS_ERR_INVALID); return; }
    std::vector<int> pages = chain(n);
    if (pages.empty()) { ret(ctx, PFS_ERR_INCONSISTENT); return; }
    if ((size_t)offset + (size_t)size > pages.size() * PAGE) { ret(ctx, PFS_ERR_INVALID); return; }
    for (int32_t i = 0; i < size; i++) {
        int32_t o = offset + i;
        uint8_t* b = img.data() + pages[o / PAGE] * PAGE + o % PAGE;
        if (flag) *b = gb(rdram, buf + i);
        else sb(rdram, buf + i, *b);
    }
    if (flag) {
        writes++;
        save_locked();
        rush::trace_event("pak: write note %d, %d bytes at %d%s", n, size, offset, persist ? "" : " (in memory only)");
        char ev[64];
        snprintf(ev, sizeof ev, "pak write note %d %d@%d", n, size, offset);
        rush::oracle_event(ev);
    } else {
        rush::trace_event("pak: read note %d, %d bytes at %d", n, size, offset);
    }
    ret(ctx, 0);
}
