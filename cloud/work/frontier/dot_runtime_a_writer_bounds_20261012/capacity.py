"""Authenticate all file-menu writer inputs; keep native payloads in memory only."""
import hashlib
from pathlib import Path
import struct
import zlib

ROOT = Path(__file__).resolve().parents[4]
ASSET_SHA = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
GAME_SHA = 'bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'
ROM_BASE = 0x283D0
GAME_BASE = 0x80086A50
LABELS = (85, 86, 88, 90, 91, 92, 149, 155)
FORMATS = {0x803B88DC: b' ', 0x803B88E0: b'%d', 0x803B88E4: b'.',
           0x803B88E8: b'%d', 0x803B88EC: b'%d', 0x803B88F0: b'1%s %s %d%s%s'}


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
    """Six-section parser from locked func_800A4E58; all pointers stay offsets."""
    position = 0
    result = []
    for width in (4, 2, 4, 2, 2, 4):
        require(position + 4 <= len(data), 'truncated count')
        count = struct.unpack_from('>I', data, position)[0]
        position += 4
        require(count <= 65535 and position + count*width <= len(data), 'bad table extent')
        result.append(struct.unpack_from('>%d%s' % (count, 'I' if width == 4 else 'H'), data, position))
        position += ((count + 1) & ~1)*2 if width == 2 else count*4
    for section in (0, 2, 5):
        require(all(position <= offset < len(data) for offset in result[section]), 'pointer outside payload')
    return result, position


def plain(data, offset, start=0):
    require(start <= offset < len(data), 'bad string pointer')
    end = data.find(b'\0', offset)
    require(end >= offset and data[offset] != 255, 'not a terminated byte-mode string')
    return data[offset:end]


def language(data, kind):
    require(type(kind) is int and 0 <= kind < 6, 'unsupported language')
    sections, start = tables(data)
    require(list(map(len, sections[:3])) == [237, 58, 374], 'language shape changed')
    selector = sections[1][47]  # native D_8017A4E0+12, halfword offset94
    require(selector+1 < len(sections[2]), 'file-action selector out of range')
    labels = {i: plain(data, sections[0][i], start) for i in LABELS}
    choices = [plain(data, sections[2][selector+i], start) for i in (0,1)]
    # All relevant authentic pointers are non-null. Conservatively include both
    # title fragments and both singular/plural branches for each language.
    title = len(choices[0]) + 1 + len(choices[1]) + 1
    available = max(6 + len(labels[85]) + len(labels[91]) + 1,
                    6 + len(labels[86]) + len(labels[92]) + 1)
    required = 1 + len(labels[88]) + 1 + len(labels[90]) + 1 + 3 + len(labels[86]) + len(labels[155]) + 1
    empty = len(labels[149]) + 1
    result = {'kind': kind, 'resource_sha256': sha(data), 'resource_bytes': len(data),
              'counts': list(map(len, sections)), 'file_action_selector': selector,
              'labels': {str(i): {'bytes': len(s), 'sha256': sha(s+b'\0')} for i,s in labels.items()},
              'choices': [{'bytes': len(s), 'sha256': sha(s+b'\0')} for s in choices],
              'output_bounds_including_nul': {'title': title, 'available': available,
                                              'required': required, 'empty_record': empty}}
    return result, {'labels': labels, 'choices': choices}


def caller_census(targets, symbol, address):
    return {name: [i*4 for i, word in enumerate(words) if word >> 26 == 3 and
                  (word & 0x3ffffff)*4 == (address & 0xfffffff)]
            for name, words in targets.items() if any(word >> 26 == 3 and
                  (word & 0x3ffffff)*4 == (address & 0xfffffff) for word in words)}


def audit(image_a, image_base, targets, symbols):
    asset = (ROOT/'assets/us/data.bin').read_bytes()
    require(len(asset) == 12418096 and sha(asset) == ASSET_SHA, 'asset identity changed')
    game = inflate(asset[0xB0CB10-ROM_BASE:], 647072)
    require(len(game) == 647072 and sha(game) == GAME_SHA, 'game identity changed')
    rows, strings = [], []
    for address, expected in FORMATS.items():
        require(plain(image_a, address-image_base) == expected, 'native format changed')
    for kind in range(6):
        rom = struct.unpack_from('>I', game, 0x8011B5BC-GAME_BASE+kind*4)[0]
        codec = struct.unpack_from('>I', game, 0x80123564-GAME_BASE+kind*4)[0]
        require(codec == 2 and ROM_BASE <= rom < 0xB0CB10, 'loader resource association changed')
        data = inflate(asset[rom-ROM_BASE:], 65536)
        row, source = language(data, kind)
        row.update(rom=hex(rom), codec=codec)
        rows.append(row); strings.append(source)
    # The actual encoder emits at most 2*n+3 bytes for wide strings and n+1
    # for byte strings. Its byte-mode table entries must never manufacture a
    # leading 255 escape that would make the copy helper misread the encoding.
    mapping_raw = game[0x8011EAEC-GAME_BASE:0x8011EAEC-GAME_BASE+512]
    mapping = list(struct.unpack('>256H', mapping_raw))
    require(all(value < 255 for value in mapping[:66]), 'byte-mode character can become a wide escape')
    encode_calls = caller_census(targets, 'func_800A1644', symbols['func_800A1644'])
    insert_calls = caller_census(targets, 'no_catchup', symbols['no_catchup'])
    require(encode_calls == {'track_process_main': [0x664,0x684], 'track_render_process': [0xBA0,0xBB0]},
            'native fixed-width encoder caller set changed')
    require(insert_calls == {'track_process_main': [0x68C], 'track_render_process': [0xBCC]},
            'native list insertion caller set changed')
    # Pin operand facts rather than exporting instruction words or listings.
    for name, checks in {
        'track_process_main': [(0x644,9,18,4,18),(0x648,9,0,6,16),(0x670,9,18,4,53),(0x674,9,0,6,4)],
        'track_render_process': [(0xB6C,9,0,6,16),(0xB84,9,16,4,18),(0xBA8,9,16,4,53),(0xBB4,9,0,6,4)]
    }.items():
        for offset,op,base,reg,value in checks:
            word=targets[name][offset//4]
            require((word>>26,(word>>21)&31,(word>>16)&31,word&65535)==(op,base,reg,value),
                    'producer destination/width fact changed')
    for name,shift,store,source_reg,result_reg,record_reg in (
            ('track_process_main',0x5C0,0x5C4,15,25,18),
            ('track_render_process',0xB94,0xB98,14,24,16)):
        word=targets[name][shift//4]
        require((word>>26,(word>>21)&31,(word>>16)&31,(word>>11)&31,(word>>6)&31,word&63)==
                (0,0,source_reg,result_reg,8,2),'native unsigned page shift changed')
        word=targets[name][store//4]
        require((word>>26,(word>>21)&31,(word>>16)&31,word&65535)==
                (43,record_reg,result_reg,68),'native page destination changed')
    maxima = {key:max(row['output_bounds_including_nul'][key] for row in rows)
              for key in rows[0]['output_bounds_including_nul']}
    # Native mixed-encoding concatenation widens byte sources as necessary.
    # Max 16 name chars + period + 4 suffix chars: FF + 2*21 + 00,00 =45.
    # Prefix availability is signed16, final required count is u8; row labels
    # conservatively use signed32. Both Pak producers store (u32+255)>>8,
    # so pages is at most 0xFFFFFF (eight digits), always representable as int.
    maxima.update(record_name=45, row_number=12, suffix_copy=11, suffix_decimal=9)
    require(max(maxima.values()) == 45 and maxima['suffix_decimal'] <= 16, 'writer bound changed')
    malformed = bytearray(data)
    struct.pack_into('>H', malformed, 4+237*4+4+47*2, 65535)
    for bad, kind in [(data, -1),(data,6),(data,True),(bytes(malformed),5)]:
        try: language(bad, kind)
        except ValueError: pass
        else: raise ValueError('invalid resource/index accepted')
    return {'asset_sha256':ASSET_SHA,'game_sha256':GAME_SHA,'languages':rows,
            'formats':[{'address':hex(a),'sha256':sha(s+b'\0')} for a,s in FORMATS.items()],
            'character_map_sha256':sha(mapping_raw),'encoder_calls':encode_calls,'list_insert_calls':insert_calls,
            'maxima_including_terminator':maxima,'text_capacity':76,'suffix_capacity':16,
            'original_capacities':'unknown','bound_method':'native flow and matched two-encoding helper analysis',
            'scope':'six authentic loaded languages; well-formed nodes from both fixed-width native producer paths; no arbitrary pointer mutation',
            'invalid_resource_controls':4}, strings, mapping
