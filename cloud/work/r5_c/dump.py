import sys, tempfile, subprocess
from pathlib import Path
import rb
src=Path(sys.argv[1]).read_text()
with tempfile.TemporaryDirectory() as t:
    obj=Path(t)/'o.o'; print(rb.build(src,obj))
    print(subprocess.run(['mips-linux-gnu-objdump','-d','-M','no-aliases,gpr-names=numeric' if False else 'reg-names=o32',str(obj)],capture_output=True,text=True).stdout)
