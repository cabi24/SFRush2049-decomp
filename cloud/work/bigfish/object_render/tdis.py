import sys,struct
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score,subprocess,tempfile
fn=sys.argv[1]; a=int(sys.argv[2]) if len(sys.argv)>2 else 0; n=int(sys.argv[3]) if len(sys.argv)>3 else 10**9
w=score.targets()[fn][a:a+n]
open('/tmp/claude-0/x.bin','wb').write(b''.join(struct.pack('>I',x) for x in w))
out=subprocess.run(['mips-linux-gnu-objdump','-b','binary','-mmips:4300','-D','-EB','--adjust-vma=%d'%(a*4),'/tmp/claude-0/x.bin'],capture_output=True,text=True).stdout
print(out.split('\n',7)[-1])
