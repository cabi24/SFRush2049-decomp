"""Private proposal for complete source-built logical sections inside ROM slots.
No live tools or registry are changed. The linker must supply the original
verified section placement before this helper receives payload bytes.
"""
import hashlib
from dataclasses import dataclass


class ExtentRefusal(ValueError):
    pass


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def logical_section(payload, logical_size, *, input_alignment, slot_alignment, relocation_offsets=()):
    """Retain every logical byte and exclude only proved natural zero tail.

    Slot alignment can be four bytes for a real MIPS word table. The input
    object's larger alignment is separate; a generated linker must enforce
    the reviewed slot alignment and exact logical output extent.
    """
    if not isinstance(payload, bytes):
        raise ExtentRefusal('compiled section must be immutable bytes')
    if type(logical_size) is not int or logical_size <= 0:
        raise ExtentRefusal('positive logical extent required')
    if slot_alignment not in (4, 16) or type(slot_alignment) is not int:
        raise ExtentRefusal('reviewed 4-byte or 16-byte slot alignment required')
    if (type(input_alignment) is not int or input_alignment < slot_alignment
            or input_alignment > 4096 or input_alignment & (input_alignment - 1)):
        raise ExtentRefusal('valid power-of-two input alignment required')
    if logical_size % slot_alignment:
        raise ExtentRefusal('logical section violates its slot alignment')
    padded_size = (logical_size + input_alignment - 1) & -input_alignment
    if len(payload) != padded_size:
        raise ExtentRefusal('emitted section does not equal its logical alignment extent')
    for offset in relocation_offsets:
        if type(offset) is not int or offset < 0 or offset % 4 or offset + 4 > logical_size:
            raise ExtentRefusal('relocation lies outside complete logical section bytes')
    if any(payload[logical_size:]):
        raise ExtentRefusal('excluded compiler alignment tail contains nonzero content')
    return payload[:logical_size]


@dataclass(frozen=True)
class Slot:
    offset: int
    original_sha256: str
    compiled_logical_bytes: bytes
    alignment: int


def compose_exact(container, slots, *, original_container_sha256):
    """Replace only verified real slots; preserve all original neighboring bytes.

    Both the full protected container and every slot must retain their recorded
    identity. Equality to original slot content is an independent acceptance
    check after real link relocation, rather than a scoring or target mask.
    """
    if not isinstance(container, bytes) or sha256(container) != original_container_sha256:
        raise ExtentRefusal('original container or neighboring bytes changed')
    ordered = sorted(slots, key=lambda slot: slot.offset)
    end = 0
    output = bytearray(container)
    for slot in ordered:
        if (type(slot.offset) is not int or slot.offset < 0
                or type(slot.alignment) is not int or slot.alignment not in (4, 16)
                or not isinstance(slot.compiled_logical_bytes, bytes)
                or not slot.compiled_logical_bytes):
            raise ExtentRefusal('malformed slot extent or alignment')
        size = len(slot.compiled_logical_bytes)
        if slot.offset % slot.alignment or size % slot.alignment:
            raise ExtentRefusal('slot violates actual alignment')
        if slot.offset < end:
            raise ExtentRefusal('logical ROM slots overlap')
        if slot.offset + size > len(container):
            raise ExtentRefusal('logical ROM slot exceeds original container')
        original = container[slot.offset:slot.offset+size]
        if sha256(original) != slot.original_sha256:
            raise ExtentRefusal('protected original slot identity changed')
        if slot.compiled_logical_bytes != original:
            raise ExtentRefusal('source-built relocated logical section is not exact')
        output[slot.offset:slot.offset+size] = slot.compiled_logical_bytes
        end = slot.offset + size
    if len(output) != len(container) or bytes(output) != container:
        raise ExtentRefusal('composition changed an original byte outside exact source-built slots')
    return bytes(output)
