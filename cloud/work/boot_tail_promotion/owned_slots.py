"""Registered owned-slot acceptance for the boot-tail research verifiers (w20p).

A ROM TU whose function owns a `.rodata` slot (rom_owned_data.json, made with
`owned_data prepare`) compiles that slot into its own object section: a C body
emits it, otherwise the slot's passthrough companion does. The packet
verifiers were written when these TUs carried no allocated data, and the
scorer leaves a section-relative reference unverified when no retail data
artifact is tracked (asm/us/boot_tail has none). This module accepts exactly
the registered slots and nothing else:

- `allocated_sections(obj, tu)`: every allocated non-text section other than
  .reginfo must be the owner section of exactly one non-storage slot registered
  for `tu`, fit it (`owned_data.section_fits`: a logical-extent table may carry
  IDO's 16-byte alignment padding, which must be zero and unrelocated), and,
  relocated to its slot address, hash to the registry's original-byte SHA-256.
- `relocate(obj, tu, words, start, end)`: score.relocate, then every
  section-relative HI16/LO16 that the scorer left unverified must resolve
  inside such a slot (addend within the slot's extent); it is then applied at
  the slot's address, so callers compare the full relocated words. Anything
  else stays masked/unverified and the caller fails closed.

The registry is read with check_source=False (no extracted assets on CI); the
slot content is proven by the registry hash instead.
"""
import hashlib
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline import owned_data

SHF_ALLOC = 2
SHT_NOBITS = 8
SHT_REL = 9
R_MIPS_32, R_MIPS_HI16, R_MIPS_LO16 = 2, 5, 6


def _tu_path(tu):
    tu = str(tu)
    return tu if tu.startswith('src/rom/') else 'src/rom/' + tu + '.c'


def slots(tu):
    """Non-storage ROM slots registered for this TU, keyed by owner section."""
    rows = {}
    for row in owned_data.rom_slots(ROOT, check_source=False):
        if row.get('storage_owner') or row['tu'] != _tu_path(tu):
            continue
        if row['owner_section'] in rows:
            raise ValueError(tu + ': two registered slots share ' + row['owner_section'])
        rows[row['owner_section']] = row
    return rows


def _flags(data, index):
    shoff, = struct.unpack_from('>I', data, 0x20)
    stride, = struct.unpack_from('>H', data, 0x2E)
    flags, = struct.unpack_from('>I', data, shoff + index * stride + 8)
    return flags


def _text_address(obj, addend, addresses):
    """A .text section addend through its owning function (score.relocate's rule)."""
    data, secs = score._elf(obj)
    text = score._text_index(secs)
    length = secs[text]['size']
    symtab = next(i for i, s in enumerate(secs) if s['type'] == 2)
    functions = [s for s in score._symbol_table(data, secs, symtab)
                 if s['section'] == text and s['type'] == 2]
    for fn in functions:
        following = min((f['value'] for f in functions if f['value'] > fn['value']), default=length)
        limit = min(following, fn['value'] + fn['size']) if fn['size'] else following
        if fn['value'] <= addend < limit:
            base = addresses.get(fn['name'], score.address_named(fn['name']))
            if base is not None:
                return base + addend - fn['value']
    return None


def _slot_bytes(obj, data, secs, index, row, addresses):
    """The section's bytes relocated to the slot's address (R_MIPS_32 only)."""
    section = secs[index]
    raw = bytearray(data[section['off']:section['off'] + section['size']])
    text = score._text_index(secs)
    for rel in secs:
        if rel['type'] != SHT_REL or rel['info'] != index:
            continue
        symbols = score._symbol_table(data, secs, rel['link'])
        for k in range(rel['size'] // 8):
            offset, info = struct.unpack_from('>II', data, rel['off'] + 8 * k)
            sym, rtype = symbols[info >> 8], info & 0xFF
            if rtype != R_MIPS_32 or offset % 4 or offset + 4 > row['size']:
                raise ValueError('%s: relocation type %d at +0x%x outside the slot table'
                                 % (section['name'], rtype, offset))
            addend, = struct.unpack_from('>I', raw, offset)
            if sym['type'] == 3 and sym['section'] == text:
                value = _text_address(obj, addend, addresses)
            elif sym['type'] == 3:
                value = None
            else:
                base = addresses.get(sym['name'], score.address_named(sym['name']))
                value = None if base is None else base + addend
            if value is None:
                raise ValueError('%s: unresolved table relocation %s+0x%x'
                                 % (section['name'], sym['name'], addend))
            struct.pack_into('>I', raw, offset, value & 0xFFFFFFFF)
    return bytes(raw)


def allocated_sections(obj, tu, addresses=None):
    """{section: report} for the object's allocated data; fail closed outside
    the TU's registered slots."""
    addresses = score.image_symbols() if addresses is None else addresses
    registered = slots(tu)
    data, secs = score._elf(obj)
    out = {}
    for index, section in enumerate(secs):
        name = section['name']
        if not _flags(data, index) & SHF_ALLOC or name in ('.text', '.reginfo') or not section['size']:
            continue
        row = registered.get(name)
        if row is None or section['type'] == SHT_NOBITS:
            raise ValueError('unexpected allocated data: %s (%d B) is not a registered owned slot of %s'
                             % (name, section['size'], _tu_path(tu)))
        if not owned_data.section_fits(row, section['size']):
            raise ValueError('%s: %d B exceeds the owned slot of %s (%d B)'
                             % (name, section['size'], row['owner'], row['size']))
        relocated = _slot_bytes(obj, data, secs, index, row, addresses)
        if any(relocated[row['size']:]):
            raise ValueError(name + ': nonzero padding past the slot extent')
        digest = hashlib.sha256(relocated[:row['size']]).hexdigest()
        if digest != row['sha256']:
            raise ValueError('%s: relocated slot bytes differ from the registered original (%s)'
                             % (name, row['owner']))
        out[name] = {'slot': row['owner'], 'members': owned_data.slot_members(row),
                     'size': section['size'], 'slot_size': row['size'],
                     'address': '0x%08X' % (row['container_vram'] + row['offset']),
                     'relocated_slot_sha256': digest}
    return out


def relocate(obj, tu, words, start, end, addresses=None):
    """score.relocate, with section-relative references into a registered
    slot of `tu` applied at the slot's address. Returns score.relocate's tuple
    plus the list of resolved references."""
    addresses = score.image_symbols() if addresses is None else addresses
    got, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, addresses)
    if not unverified:
        return got, masks, unresolved, unverified, errors, []
    registered = slots(tu)
    data, secs = score._elf(obj)
    text = score._text_index(secs)
    resolved, sites = [], set()
    for rel in secs:
        if rel['type'] != SHT_REL or rel['info'] != text:
            continue
        symbols = score._symbol_table(data, secs, rel['link'])
        pending = []
        for k in range(rel['size'] // 8):
            offset, info = struct.unpack_from('>II', data, rel['off'] + 8 * k)
            index, rtype = info >> 8, info & 0xFF
            if not start <= offset < end:
                continue
            sym = symbols[index]
            if rtype == R_MIPS_HI16:
                pending.append((offset, index, words[offset // 4] & 0xFFFF))
                continue
            if rtype != R_MIPS_LO16:
                continue
            lo = words[offset // 4] & 0xFFFF
            lo = lo - 0x10000 if lo & 0x8000 else lo
            his = [h for h in pending if h[1] == index]
            pending = [h for h in pending if h[1] != index]
            if sym['type'] != 3 or sym['section'] == text:
                continue
            addend = ((his[0][2] << 16) if his else 0) + lo
            row = registered.get(sym['name'])
            if row is None or not 0 <= addend < row['size'] or not his:
                continue          # stays masked and unverified: caller fails
            value = row['container_vram'] + row['offset'] + addend
            for hi_offset, _, _ in his:
                got[hi_offset // 4] = (words[hi_offset // 4] & 0xFFFF0000) | (((value + 0x8000) >> 16) & 0xFFFF)
                sites.add(hi_offset)
            got[offset // 4] = (words[offset // 4] & 0xFFFF0000) | (value & 0xFFFF)
            sites.add(offset)
            resolved.append('%s+%#x at +0x%x -> 0x%08X (slot %s)'
                            % (sym['name'], addend, offset - start, value, row['owner']))
    if set(masks) == sites:
        masks, unverified = {}, []
    return got, masks, unresolved, unverified, errors, resolved


def accepted(result, obj, tu, fn, start=None):
    """Strict acceptance: the scorer's verdict, except that unverified
    references are allowed only if every one resolves inside a registered slot
    and the full relocated words then equal the target."""
    if result.accepted(False):
        return True
    if not result.accepted(True):
        return False
    want = score.targets()[fn]
    start = score.symbols(obj)[fn] if start is None else start
    got, masks, unresolved, unverified, errors, _ = relocate(
        obj, tu, score.text_words(obj), start, start + len(want) * 4)
    return (not (masks or unresolved or unverified or errors)
            and got[start // 4:start // 4 + len(want)] == want)
