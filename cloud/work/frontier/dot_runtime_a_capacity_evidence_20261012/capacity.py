"""Independent, authenticated language/writer bound; never exports native payloads."""
import hashlib
import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
ASSET_SHA = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
GAME_SHA = 'bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'
BASE = 0x283D0
GAME_BASE = 0x80086A50


def require(ok, why):
    if not ok:
        raise ValueError(why)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def inflate(data, limit):
    decoder = zlib.decompressobj(-15)
    result = decoder.decompress(data, limit + 1)
    require(decoder.eof and len(result) <= limit and not decoder.unconsumed_tail,
            'invalid or oversized resource stream')
    return result


def tables(data):
    """The six-section parser in the native, locked func_800A4E58."""
    position = 0
    result = []
    for width in (4, 2, 4, 2, 2, 4):
        require(position + 4 <= len(data), 'truncated resource count')
        count = struct.unpack_from('>I', data, position)[0]
        position += 4
        require(count <= 65535 and position + count * width <= len(data), 'bad resource table extent')
        result.append(struct.unpack_from('>%d%s' % (count, 'I' if width == 4 else 'H'), data, position))
        position += ((count + 1) & ~1) * 2 if width == 2 else count * 4
    for section in (0, 2, 5):
        require(all(position <= offset < len(data) for offset in result[section]), 'string pointer outside payload')
    return result, position


def terminated(data, offset, start):
    require(start <= offset < len(data), 'invalid string offset')
    end = data.find(b'\0', offset)
    require(end >= offset, 'unterminated string')
    return data[offset:end]


def language(data, kind):
    require(type(kind) is int and 0 <= kind < 6, 'resource kind outside supported language domain')
    sections, start = tables(data)
    require(len(sections[0]) == 237 and len(sections[1]) == 58 and len(sections[2]) == 374,
            'language table shape changed')
    plural = sections[1][20]
    require(plural < len(sections[2]), 'plural selector out of range')
    one = terminated(data, sections[0][29], start)
    many = terminated(data, sections[2][plural], start)
    require(len(one) <= 7 and len(many) <= 9, 'supported player text exceeds established length')
    return one, many, {'kind': kind, 'resource_sha256': sha(data), 'resource_bytes': len(data),
                       'counts': list(map(len, sections)), 'plural_index': plural,
                       'singular_bytes': len(one), 'plural_bytes': len(many),
                       'singular_sha256': sha(one + b'\0'), 'plural_sha256': sha(many + b'\0')}


def audit(image_a, image_base):
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(len(asset) == 12418096 and sha(asset) == ASSET_SHA, 'asset identity changed')
    game = inflate(asset[0xB0CB10-BASE:], 647072)
    require(len(game) == 647072 and sha(game) == GAME_SHA, 'game identity changed')
    initial = asset[0x8002E860 - 0x7FFFF400 - BASE]
    require(initial == 5, 'initial language selector changed')
    formats = []
    for address in (0x803B8854, 0x803B885C):
        fmt = terminated(image_a, address-image_base, 0)
        require(fmt == b'%d %s', 'native writer format changed')
        formats.append({'address': hex(address), 'sha256': sha(fmt+b'\0')})
    rows, strings = [], []
    for kind in range(6):
        rom = struct.unpack_from('>I', game, 0x8011B5BC-GAME_BASE + kind*4)[0]
        codec = struct.unpack_from('>I', game, 0x80123564-GAME_BASE + kind*4)[0]
        require(codec == 2 and BASE <= rom < 0xB0CB10, 'language loader association changed')
        data = inflate(asset[rom-BASE:], 65536)
        one, many, row = language(data, kind)
        row.update(rom=hex(rom), codec=codec)
        rows.append(row)
        strings.append((one, many))
    # Decimal conversion has no field width or precision. Signed16 reaches six
    # characters including a possible minus sign; add one separator and one NUL.
    maximum = max(6 + 1 + len(many) + 1 for one, many in strings)
    require(maximum == 17 and max(len(one)+3 for one, many in strings) == 10,
            'formatted writer bound changed')
    for invalid in (-1, 6, 255, True):
        try:
            language(data, invalid)
        except ValueError:
            pass
        else:
            raise ValueError('invalid language resource accepted')
    malformed = bytearray(data)
    # The actual u16 plural index is after the first count and 237 pointers.
    struct.pack_into('>H', malformed, 4+237*4+4+20*2, 65535)
    try:
        language(malformed, 5)
    except ValueError:
        pass
    else:
        raise ValueError('out-of-range plural selector accepted')
    return {'asset_sha256': ASSET_SHA, 'game_sha256': GAME_SHA, 'initial_language': initial,
            'formats': formats, 'languages': rows, 'signed16_domain': [-32768, 32767],
            'singular_output_max_including_nul': 10, 'all_output_max_including_nul': maximum,
            'reconstructed_capacity': 36, 'original_capacity': 'unknown',
            'scope': 'six authentic supported language resources with valid loaded tables; corrupt saves or mutated pointers excluded',
            'controls': 'out-of-domain resource and out-of-range plural index refused'}, strings
