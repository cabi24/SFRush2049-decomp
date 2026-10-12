"""Authenticated unlock-panel language/writer bound; never exports native payloads."""
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
    require(type(kind) is int and 0 <= kind < 6, 'unsupported language')
    sections, start = tables(data)
    require(list(map(len, sections[:3])) == [237, 58, 374], 'language shape changed')
    selector=sections[1][44]
    require(selector+3 < len(sections[2]), 'unlock selector outside indexed table')
    choices=[terminated(data,sections[2][selector+i],start) for i in (1,2,3)]
    label=terminated(data,sections[0][42],start)
    require(all(s and s[0] != 255 for s in choices+[label]),'wide or empty unlock text')
    maxima=[len(s)+1+len(label)+1 for s in choices]
    return {'kind':kind,'resource_sha256':sha(data),'resource_bytes':len(data),
            'counts':list(map(len,sections)),'selector':selector,
            'choices':[{'bytes':len(s),'sha256':sha(s+b'\0')} for s in choices],
            'label':{'bytes':len(label),'sha256':sha(label+b'\0')},
            'output_bytes_including_nul':maxima}, choices, label


def audit(image):
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(len(asset)==12418096 and sha(asset)==ASSET_SHA,'asset identity changed')
    game=inflate(asset[0xB0CB10-BASE:],647072)
    require(len(game)==647072 and sha(game)==GAME_SHA,'game identity changed')
    initial=asset[0x8002E860-0x7FFFF400-BASE]
    require(initial==5,'initial language selector changed')
    fmt=terminated(image,0x803B8BDC-0x8038A400,0)
    require(fmt==b'%s %s','unlock format changed')
    rows=[]
    for kind in range(6):
        rom=struct.unpack_from('>I',game,0x8011B5BC-GAME_BASE+kind*4)[0]
        codec=struct.unpack_from('>I',game,0x80123564-GAME_BASE+kind*4)[0]
        require(codec==2 and BASE<=rom<0xB0CB10,'language loader association changed')
        data=inflate(asset[rom-BASE:],65536)
        row,choices,label=language(data,kind)
        row.update(rom=hex(rom),codec=codec);rows.append(row)
        # This is a byte-count oracle, not native formatter execution. The
        # native %s path is audited separately and its complete target pinned.
        for choice,maximum in zip(choices,row['output_bytes_including_nul']):
            require(len(choice+b' '+label+b'\0')==maximum,'concat length differs')
    maximum=max(n for row in rows for n in row['output_bytes_including_nul'])
    require(maximum==24 and maximum<=44,'unlock text capacity insufficient')
    malformed=bytearray(data)
    struct.pack_into('>H',malformed,4+237*4+4+44*2,65535)
    for bad,kind in ((data,-1),(data,6),(data,True),(bytes(malformed),5)):
        try:language(bad,kind)
        except ValueError:pass
        else:raise ValueError('invalid resource/index accepted')
    return {'asset_sha256':ASSET_SHA,'game_sha256':GAME_SHA,'initial_language':initial,
            'format':{'address':'0x803B8BDC','sha256':sha(fmt+b'\0')},'languages':rows,
            'maximum_output_including_nul':maximum,'buffer_bytes':44,
            'original_capacity':'unknown','scope':'six authentic loaded languages; valid resource tables; no arbitrary pointer mutation',
            'invalid_resource_controls':4,'all_selected_strings_byte_mode':True}
