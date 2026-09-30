import sys,re; sys.path.insert(0,'.')
import ev
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
def show(text, anchor_op=None, n=22, start_pat=r'addiu\s+\w+,s\d,-1'):
    got,_,_=ev.compile_words(text)
    dis=[score.disasm_word(w) for w in got]
    for i,d in enumerate(dis):
        if re.search(start_pat,d) and i>300:
            for k in range(max(0,i-2),min(len(dis),i+n)): print('  ',k,dis[k])
            return
    print('no anchor')
