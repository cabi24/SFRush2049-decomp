#!/bin/bash
# assemble group.c from parts/ in MAT3 (GC) source order
cd "$(dirname "$0")"
ORDER="header gbi decls func_8038E5DC func_8038DFEC func_8038B940 func_8038C5EC func_8038B834 func_8038ACB0 func_8038A634 func_8038C934 func_8038A62C func_8038AEA8 func_8038AE50 func_8038AEB0 func_8038B388 func_8038B3EC func_8038CD14 func_8038CCF4 func_8038CD0C func_8038CD04 func_8038CCFC func_8038B930 func_8038A820 func_8038CCEC func_8038CCE4 func_8038C93C func_8038B938 func_8038BBB4"
{ for p in $ORDER; do cat parts/$p.c; echo; done; } > group.c.new && mv group.c.new group.c
