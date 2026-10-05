#include "sequence_context.h"
#define OFFSET(type, member) ((unsigned int)&(((type *)0)->member))
#define CHECK(name, condition) typedef char check_##name[(condition) ? 1 : -1]
#define FIELD(member, offset) CHECK(member, OFFSET(SequenceContext, member) == offset)
CHECK(pointer_width, sizeof(void *) == 4);
CHECK(record_stride, sizeof(SequenceContext) == 0xFF8);
CHECK(array_bytes, sizeof(D_80043EB8) == 8 * 0xFF8);
CHECK(node_stride, sizeof(SequenceNode) == 0x18);
CHECK(node_identifier, OFFSET(SequenceNode, identifier) == 8);
CHECK(timed_stride, sizeof(SequenceTimedValue) == 8);
FIELD(identifier, 0x000);
FIELD(first110, 0x110);
FIELD(second114, 0x114);
FIELD(half120, 0x120);
FIELD(rate124, 0x124);
FIELD(channels528, 0x528);
FIELD(timingStartF68, 0xF68);
FIELD(currentF6C, 0xF6C);
FIELD(lowF70, 0xF70);
FIELD(highF74, 0xF74);
FIELD(active, 0xF78);
FIELD(pending, 0xF7C);
FIELD(activeFC0, 0xFC0);
FIELD(inactiveFC1, 0xFC1);
FIELD(valueFC2, 0xFC2);
FIELD(channelFC4, 0xFC4);
FIELD(valueFE0, 0xFE0);
FIELD(firstFE4, 0xFE4);
FIELD(secondFE8, 0xFE8);
FIELD(valueFEC, 0xFEC);
FIELD(flagsFEE, 0xFEE);
FIELD(pendingFF0, 0xFF0);
