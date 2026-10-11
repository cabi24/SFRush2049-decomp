# df.sh SRC TARGET VER FLAGS : diff objdump of target vs candidate
cd ~/rush2049/scratch/frontier/w15g
python3 sc.py "$1" "$2" --ver "$3" --flags "$4" --keep /tmp/w15g_c.o | tail -1
D="mips-linux-gnu-objdump -drz -m mips:4300 -j .text"
diff <($D tgt/$2.o | sed 's/^ *[0-9a-f]*:\t[0-9a-f]* \t//' | tail -n +4) <($D /tmp/w15g_c.o | sed 's/^ *[0-9a-f]*:\t[0-9a-f]* \t//' | tail -n +4) | head -${5:-40}
