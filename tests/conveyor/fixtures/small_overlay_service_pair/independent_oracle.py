"""Independent step-rounded binary32 oracle; host semantics, not MIPS emulation.

Usage: python independent_oracle.py PATH_TO_COMPILED_SHARED_TEST_LIBRARY
All fixture data is synthetic. External angle helper is intentionally mocked.
"""
import ctypes, random, struct, sys
lib=ctypes.CDLL(sys.argv[1])
f=lambda v:struct.unpack('f',struct.pack('f',v))[0]
s16=lambda v:((v+32768)%65536)-32768
rng=random.Random(0xD3A4)
for i in range(30000):
 rem=rng.randint(-32768,32767)
 amount=rng.randint(-2147483648,2147483647)
 flag=rng.choice([0,4,8,12])
 model=rng.choice([0,0,0,-1,1])
 src=rng.randrange(6);tgt=rng.randrange(6);team=rng.choice([0,0,1])
 blocked=model!=0 or src==tgt or team
 a=int(f(f(amount)*f(.2))) if flag&4 else amount
 result=s16(rem-a)
 expected=rem if blocked else max(0,result)
 actual=lib.run_case(rem,amount,flag,model,src,tgt,team,-1)
 assert actual==expected,(i,rem,amount,actual,expected)
 assert lib.amount_result()==(12345 if blocked else s16(a))
 assert ctypes.c_int.in_dll(lib,'callback_count').value==(int(result<=0) if not blocked else 0)
# callback re-read and modular edge cases
assert lib.run_case(1,32770,0,0,0,1,0,-1)==32767
assert lib.run_case(32767,-1,0,0,0,1,0,-1)==0
assert lib.run_case(1,1,0,0,0,1,0,5)==0
assert lib.model_result(1)==0 and lib.model_result(5)==1
print('PASS: 30000 independently modeled D3A4 cases + 3 modular/callback cases')
players=(ctypes.c_ubyte*(12*952)).in_dll(lib,'small_players')
models=(ctypes.c_ubyte*(128*2056)).in_dll(lib,'small_models')
teams=(ctypes.c_byte*128).in_dll(lib,'small_teams')
count=ctypes.c_int16.in_dll(lib,'small_player_count')
lib.small_8038D798.argtypes=[ctypes.POINTER(ctypes.c_float),ctypes.POINTER(ctypes.c_float),ctypes.c_int,ctypes.c_float,ctypes.c_int]
put=lambda off,fmt,value:ctypes.memmove(ctypes.addressof(players)+off,struct.pack(fmt,value),struct.calcsize(fmt))
get=lambda off,fmt:struct.unpack(fmt,bytes(players[off:off+struct.calcsize(fmt)]))[0]
for it in range(3000):
 ctypes.memset(players,0,len(players));ctypes.memset(models,0,len(models))
 ctypes.c_int.in_dll(lib,'rewrite_index').value=-1
 for j in range(128):teams[j]=j
 n=rng.randint(-1,6);count.value=n
 origin_values=[f(rng.uniform(-20,20)) for _ in range(3)]
 origin=(ctypes.c_float*3)(*origin_values)
 endpoint=(ctypes.c_float*3)(1,2,3)
 threshold=f(rng.choice([0,-1,100,400,1600]))
 magnitude=rng.randrange(-1000,1001)
 expected=[]
 put(7*952+0x35b,'b',7)
 for j in range(max(n,0)):
  pos=[f(rng.uniform(-20,20)) for _ in range(3)]
  for k,p in enumerate(pos):put(j*952+8+k*4,'f',p)
  put(j*952+0x35b,'b',j)
  enabled=rng.choice([0,1,-1]);excluded=rng.choice([0,0,1]);kind=rng.choice([0,5]);flag=rng.choice([0,4]);model=rng.choice([0,0,1])
  put(j*952+0x308,'b',enabled);put(j*952+0x359,'b',excluded);put(j*952+0x384,'b',kind);put(j*952+0x38c,'I',flag);put(j*952+0x386,'h',10000)
  models[j*2056+0x640]=model
  delta=[f(a-b) for a,b in zip(origin_values,pos)]
  squares=[f(d*d) for d in delta]
  distance=f(squares[2]+f(squares[0]+squares[1]))
  a=0
  if enabled and not excluded and not model and distance<threshold:
   weight=f(f(threshold-distance)/threshold)
   af=f(f(weight*weight)*f(magnitude))
   if kind==5:af=f(af*f(.35)) # native angle mocked at zero
   a=int(af)
   if flag&4:a=int(f(f(a)*f(.2)))
  expected.append((10000-a,s16(a)))
 lib.small_8038D798(origin,endpoint,7,threshold,magnitude)
 for j,(rem,a) in enumerate(expected):
  assert get(j*952+0x386,'h')==rem,(it,j,rem,get(j*952+0x386,'h'))
  assert get(j*952+0x388,'h')==a
print('PASS: 3000 independently modeled D798 fixtures (mock angle zero; valid finite domain)')
