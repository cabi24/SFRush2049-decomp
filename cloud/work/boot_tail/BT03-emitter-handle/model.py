"""Caller-boundary state model with explicit output-definedness, never defaults.

The final StartContinousEmitters call is an opaque observation boundary. These
fixtures describe caller contracts, not full spatial/audio-engine executions.
"""
import itertools
import struct
MASK=0xFFFFFFFF
NAMES=('volume','x_pan','y_pan','z_pan','pitch')


def bits(value):return struct.unpack('>I',struct.pack('>f',value))[0]
def f32(value):return struct.unpack('>f',struct.pack('>f',value))[0]


class Domain(Exception):
    def __init__(self,kind,name=None):self.kind,self.name=kind,name


def node(flags=0x20001,identifier=MASK,group=0,fade=.875):
    return dict(flags=flags,identifier=identifier,group=group,fade=fade)


def fixtures():
    out=[dict(nodes=[],empty=False,fail=False,label='empty')]
    upper=(0,0x20000,0x40000,0x80000,0x100000,0x120000,0x180000,0x140000)
    for low,high,identifier,empty,fail in itertools.product(range(8),upper,(MASK,0x1001,0xDEAD0001),(False,True),(False,True)):
        out.append(dict(nodes=[node(high|low,identifier)],empty=empty,fail=fail,label='single'))
    for first_volume in (0.,.5):
        for flags in (0,2,4,0x80000,0x100000):
            out.append(dict(nodes=[node(1,0x1001,0),node(flags,0x1002,1)],empty=False,fail=False,label='defined-carry',volume=first_volume))
    out += [dict(nodes=[node(1,0x1001),node(0x40000,0x1002),node(0,0x1003)],empty=False,fail=False,label='remove-middle'),
            dict(nodes=[node(0x40000,0x1001),node(0x20001)],empty=False,fail=False,label='remove-head'),
            dict(nodes=[node(0x20000, MASK,0,.5),node(0x100000,0x1001,1,.5)],empty=False,fail=False,label='fade-below'),
            dict(nodes=[node(0x20001,MASK,0) for _ in range(33)],empty=False,fail=False,label='large-pool-full'),
            dict(nodes=[node(0x20001,MASK,0) for _ in range(32)]+[node(0x120001,0x1001,1)],empty=False,fail=False,label='new-group-before-large-failure'),
            dict(nodes=[node(1,0x1001,0) for _ in range(33)],empty=False,fail=False,label='small-pool-overflow'),
            dict(nodes=[node(1,0x1001,i) for i in range(33)],empty=False,fail=False,label='group-overflow')]
    return out


def initial(config):
    rows=[]
    for i,n in enumerate(config['nodes']):
        rows.append(dict(n,next=i+1 if i+1<len(config['nodes']) else -1,previous=i-1,
                         sound_id=7+i,counter=0x1200+i,
                         unknown=bytes(0x80+(i*7+j)%128 for j in range(40))))
    return dict(nodes=rows,root=0 if rows else -1,groups=[],large=0,small=0,starts=0,trace=[])


def event(state,code,*args):state['trace'].append((code,tuple(a&MASK for a in args)))


def snapshot(state):
    # The observation is immediately on entry to the final opaque helper.
    words=[]
    for code,args in state['trace']:words += [code,len(args),*args]
    words += [state['root']&MASK,len(state['nodes']),len(state['groups']),state['large'],state['small'],state['starts'],*state['groups']]
    for n in state['nodes']:
        words += [n['next']&MASK,n['previous']&MASK,n['flags'],n['identifier'],n['group'],n['sound_id'],n['counter'],bits(n['fade'])]
        words += [int.from_bytes(n['unknown'][i:i+4],'big') for i in range(0,40,4)]
    h=2166136261
    for word in words:
        for byte in (word&MASK).to_bytes(4,'big'):h=((h^byte)*16777619)&MASK
    return h


def run(config):
    state=initial(config);outputs={name:None for name in NAMES};writers={name:None for name in NAMES};consumed=carried=0
    def read(name,current):
        nonlocal consumed,carried
        if outputs[name] is None:raise Domain('undefined_output',name)
        consumed+=1;carried+=writers[name]!=current
        return outputs[name]
    def group(key,checked):
        if key not in state['groups']:
            if len(state['groups'])==32:
                if checked:return False
                raise Domain('capacity')
            state['groups'].append(key)
        return True
    event(state,0)
    index=state['root']
    while index!=-1:
        em=state['nodes'][index];following=em['next']
        if em['flags']&0x40000:
            event(state,1,index)
            if em['next']!=-1:state['nodes'][em['next']]['previous']=em['previous']
            if em['previous']!=-1:state['nodes'][em['previous']]['next']=em['next']
            else:state['root']=em['next']
            em['flags']&=65535
            index=following;continue
        if em['flags']&0x20001:
            event(state,2,index)
            values={'volume':0. if config['empty'] else config.get('volume',.5)+index/64.,'pitch':1. if config['empty'] else 1.25+index/64.}
            if not config['empty']:values.update(x_pan=.125+index/128.,y_pan=-.25+index/128.,z_pan=.75-index/128.)
            for name,value in values.items():outputs[name]=f32(value);writers[name]=index
        if not em['flags']&0x80000:
            found=False
            if em['flags']&0x20000:
                if read('volume',index)==0. and em['flags']&4:
                    em['flags']|=0x80000;em['flags']&=~0x20000;found=True
                if not found:
                    if em['flags']&1:
                        values=[read(name,index) for name in NAMES]
                        event(state,3,index,*map(bits,values))
                        if group(em['group'],True) and state['large']<32:
                            state['large']+=1;index=following;continue
                    else:
                        event(state,4,em['sound_id'],127,64)
                        if config['fail']:em['identifier']=MASK
                        else:state['starts']+=1;em['identifier']=0x1000+state['starts']
                        if em['identifier']==MASK:
                            if not em['flags']&2:em['flags']|=0x40000;em['flags']&=~0x20000
                            else:index=following;continue
            else:
                event(state,5,em['identifier'])
                if not 0x1000<=em['identifier']<0x1100:em['identifier']=MASK
                if em['identifier']==MASK:em['flags']|=0x20000 if em['flags']&2 else 0x40000
            if em['identifier']!=MASK:
                if em['flags']&1:
                    volume=read('volume',index);event(state,6,index,bits(volume));group(em['group'],False)
                    if state['small']==32:raise Domain('capacity')
                    state['small']+=1
                if read('volume',index)==0. and em['flags']&4:
                    event(state,7,em['identifier']);em['identifier']=MASK
                    em['flags']|=0x80000 if em['flags']&2 else 0x40000
                else:
                    values=[read(name,index) for name in NAMES]
                    event(state,8,index,em['identifier'],*map(bits,values))
            if em['flags']&0x100000:
                em['fade']=f32(em['fade']+.25)
                if em['fade']>=1.:em['flags']&=~0x100000
        elif read('volume',index)!=0.:
            em['flags']&=~0x80000;em['flags']|=0x20000
        index=following
    event(state,9)
    return dict(hash=snapshot(state),consumed=consumed,carried=carried,state=state)
