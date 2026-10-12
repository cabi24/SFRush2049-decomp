# Rebind after lane w22g's libultra rename

`verification.json` pins the text of `asm/us/nonmatchings/rom/lib_1050/viTickStart.s`. The rename changed only two
operand names in it (`jal osSetGlobalIntMask` -> `jal osSetIntMask`, same
instruction words 0C001C74); the clock writer's words that `clock_witness()`
reads are unchanged. Both pins (`source_bindings` and `clock_contract.writers[0]`)
moved from 7d4fc71e9403da4beead7c910b464bd8fe2470b3abdc3eeeae6dcd83576654b5 to 271839def5547a87c5ddd00a567620d1306a31814b9afaebb0f78e42b17a123e; nothing else in the receipt changed.
