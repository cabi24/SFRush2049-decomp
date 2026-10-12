"""Independent, fail-closed ELF32 big-endian reader (no target payloads)."""
import struct


def require(value, detail):
    if not value:
        raise ValueError(detail)


def read(path):
    raw = path.read_bytes()
    require(raw[:6] == b'\x7fELF\x01\x02', 'ELF32 big endian required')
    require(struct.unpack_from('>H', raw, 18)[0] == 8, 'MIPS required')
    offset = struct.unpack_from('>I', raw, 32)[0]
    stride, count, names_index = struct.unpack_from('>HHH', raw, 46)
    require(stride == 40, 'section stride')
    rows = [struct.unpack_from('>10I', raw, offset + i * stride) for i in range(count)]
    names_row = rows[names_index]
    names = raw[names_row[4]:names_row[4] + names_row[5]]
    sections, symbols, tables, relocations = {}, {}, {}, []
    for i, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        data = raw[row[4]:row[4] + row[5]]
        sections[name] = (i, row, data)
        if row[1] != 2:
            continue
        strings_row = rows[row[6]]
        strings = raw[strings_row[4]:strings_row[4] + strings_row[5]]
        table = []
        for at in range(0, len(data), 16):
            no, value, size, info, other, index = struct.unpack_from('>IIIBBH', data, at)
            symbol = strings[no:].split(b'\0')[0].decode()
            table.append(symbol)
            if symbol:
                symbols[symbol] = (value, size, info & 15, index)
        tables[i] = table
    for row in rows:
        if row[1] not in (4, 9):
            continue
        require(row[1] == 9, 'RELA unsupported')
        for at in range(row[4], row[4] + row[5], 8):
            address, info = struct.unpack_from('>II', raw, at)
            relocations.append((address, info & 255, tables[row[6]][info >> 8], row[7]))
    return sections, symbols, relocations
