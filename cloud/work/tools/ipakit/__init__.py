"""ipakit: static IPA (IDO -O3 interprocedural register allocation) analysis from retail words.

Modules: mipsdec (decoder), analyze (per function CFG/liveness), clobber (clobber sets),
deps (register-specific dependency edges and minimal closure), heads (function head audit).
Stdlib only.  `load_corpus()` is the shared entry point: it reads the committed retail
targets (tools/cloud/score.py targets()) and the inflated game image.
"""
import json
import struct
import sys
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
IMAGE_BASE = 0x80086A50
IMAGE_BYTES = 647072
DATA_BIN = ROOT / 'assets' / 'us' / 'data.bin'
SYMBOLS_JSON = ROOT / 'asm' / 'us' / 'blob' / 'symbols.json'
_DEFLATE_OFF, _DEFLATE_LEN, _ROM_OFF = 0xB0CB10 - 0x10000, 326180, 0xB0CB10


def image_words():
    """Inflate the game-code image from assets/us/data.bin (as cloud/work/tools/extscore.py does)."""
    data = DATA_BIN.read_bytes()
    img = zlib.decompressobj(-15).decompress(data[_DEFLATE_OFF:_DEFLATE_OFF + _DEFLATE_LEN])
    if len(img) != IMAGE_BYTES:
        raise SystemExit('game image has unexpected size %d' % len(img))
    return list(struct.unpack('>%dI' % (len(img) // 4), img))


def func_name(addr):
    return 'func_%08X' % addr


class Func:
    __slots__ = ('name', 'addr', 'words', 'discovered')

    def __init__(self, name, addr, words, discovered=False):
        self.name, self.addr, self.words, self.discovered = name, addr, words, discovered

    @property
    def end(self):
        return self.addr + 4 * len(self.words)

    def __repr__(self):
        return 'Func(%s @%08x, %dw)' % (self.name, self.addr, len(self.words))


class Corpus:
    """Retail targets + game image.  Functions are the `.text.<name>` sections of asm/us/blob
    (exact retail extents); `add_head()` adds functions found inside the opaque runs."""

    def __init__(self, funcs, image, symbols):
        self.image = image
        self.symbols = symbols
        self.funcs = {}
        self.by_addr = {}
        self._starts = None
        for f in funcs:
            self._add(f)

    def _add(self, f):
        self.funcs[f.name] = f
        self.by_addr[f.addr] = f
        self._starts = None

    @property
    def starts(self):
        if self._starts is None:
            self._starts = sorted(self.by_addr)
        return self._starts

    def in_image(self, addr):
        return IMAGE_BASE <= addr < IMAGE_BASE + 4 * len(self.image)

    def word_at(self, addr):
        if not self.in_image(addr) or addr & 3:
            return None
        return self.image[(addr - IMAGE_BASE) >> 2]

    def words_at(self, addr, n):
        o = (addr - IMAGE_BASE) >> 2
        return self.image[o:o + n]

    def containing(self, addr):
        """Function whose extent contains addr, or None."""
        import bisect
        s = self.starts
        i = bisect.bisect_right(s, addr) - 1
        if i >= 0:
            f = self.by_addr[s[i]]
            if addr < f.end:
                return f
        return None

    def resolve(self, addr):
        """Classify a call/jump target address.

        -> (kind, Func|None, alt) where kind is 'func' (exact start), 'alt' (start+4, the
        copied-first-instruction optimization), 'interior' (inside a known function, not at a
        head), 'opaque' (inside the game image but in no known function) or 'external'
        (outside the game image: static boot/library code, O32 ABI)."""
        f = self.by_addr.get(addr)
        if f is not None:
            return 'func', f, False
        f = self.by_addr.get(addr - 4)
        if f is not None and len(f.words) > 1:
            return 'alt', f, True
        c = self.containing(addr)
        if c is not None:
            return 'interior', c, False
        if self.in_image(addr):
            return 'opaque', None, False
        return 'external', None, False

    def add_head(self, addr, name=None, nwords=None):
        """Register a function discovered at addr (extent scanned from the image when nwords is None)."""
        if addr in self.by_addr:
            return self.by_addr[addr]
        if nwords is None:
            from ipakit import heads
            nwords = heads.scan_extent(self, addr)
        f = Func(name or func_name(addr), addr, self.words_at(addr, nwords), True)
        self._add(f)
        return f


_cache = {}


def load_corpus(discover=True):
    """Shared corpus (cached per process).  With discover=True, `jal` targets that land in
    opaque image runs are registered as discovered heads (see heads.py)."""
    if 'base' not in _cache:
        sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
        import score
        T = score.targets()
        syms = {k: int(v, 16) for k, v in json.loads(SYMBOLS_JSON.read_text())['symbols'].items()}
        funcs = [Func(n, syms[n], list(w)) for n, w in T.items() if n in syms and w]
        _cache['base'] = (funcs, image_words(), syms)
    funcs, img, syms = _cache['base']
    key = 'disc' if discover else 'plain'
    if key not in _cache:
        c = Corpus([Func(f.name, f.addr, f.words) for f in funcs], img, syms)
        if discover:
            from ipakit import heads
            heads.discover_call_heads(c)
        _cache[key] = c
    return _cache[key]
