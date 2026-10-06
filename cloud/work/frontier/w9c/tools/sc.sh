#!/bin/bash
# usage: sc.sh cand.c [NAME]  -- score candidate in the unit, show word diff summary
R=/home/cburnes/projects/rush2049-decomp; N=${2:-func_8008E408}
cd $R && python3 -m tools.conveyor.pipeline.blob_unit --tag w9c --jobs 2 score $N --with $1 2>&1 | grep -E "MATCH|FAIL|equal" | cut -c1-200
$R/cloud/work/frontier/w9c/tools/udiff.sh $N 0 | head -2 | tr '\n' ' '; echo
diff /tmp/claude-1000/udiff/rn.txt /tmp/claude-1000/udiff/un.txt | grep -c '^[<>]'
n2(){ sed -E 's/(addiu [a-z0-9]+,[a-z0-9]+,)-?[0-9]+$/\1L/; s/(beq|bne|bnez|beqz|bgez|bgezl|beql|bnel|b|jal) (.*)[0-9a-fx]+$/\1 \2/; s/0x[0-9a-f]+$//; s/\b[0-9a-f]{3,}$//; s/-?[0-9]+\(sp\)/N(sp)/; s/sp,-?[0-9]+/sp,N/; s/(lui [a-z0-9]+,).*/\1H/; s/(addiu [a-z0-9]+,[a-z0-9]+,)-?[0-9]+$/\1L/; s/-?[0-9]+\((at|a0)\)/L(\1)/; s/(lw [a-z0-9]+,)-?[0-9]+\(t7\)/\1L(t7)/' $1; }
n2 /tmp/claude-1000/udiff/r.txt > /tmp/claude-1000/udiff/r3; n2 /tmp/claude-1000/udiff/u.txt > /tmp/claude-1000/udiff/u3
echo "regdiff=$(diff /tmp/claude-1000/udiff/r3 /tmp/claude-1000/udiff/u3 | grep -c '^[<>]')"
