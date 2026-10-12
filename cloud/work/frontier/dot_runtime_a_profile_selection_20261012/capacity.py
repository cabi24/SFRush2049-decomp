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
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(len(asset)==12418096 and sha(asset)==ASSET_SHA,'asset identity changed')
    game=inflate(asset[0xB0CB10-BASE:],647072)
    require(len(game)==647072 and sha(game)==GAME_SHA,'game identity changed')
    fmt=terminated(image_a,0x803B899C-image_base,0)
    require(fmt==b'%s %d','native selection writer format changed')
    rows=[];strings=[]
    for kind in range(6):
        rom=struct.unpack_from('>I',game,0x8011B5BC-GAME_BASE+kind*4)[0]
        codec=struct.unpack_from('>I',game,0x80123564-GAME_BASE+kind*4)[0]
        require(codec==2 and BASE<=rom<0xB0CB10,'language loader association changed')
        data=inflate(asset[rom-BASE:],65536);one,many,row=language(data,kind)
        row.update(rom=hex(rom),codec=codec);rows.append(row);strings.append(one)
    require(max(map(len,strings))==7,'native label29 bound changed')
    return dict(languages=rows,format_address='0x803B899C',format_sha256=sha(fmt+b'\0'),
                valid_player_range=[0,3],valid_writer_max_including_nul=10,
                independent_signed32_formatter_max_including_nul=20,
                capacity=32,original_capacity='unknown',
                scope='valid authentic language tables and synchronous read-only text consumers; signed32 bound is for formatter inputs, not invalid renderer player indices'),strings
