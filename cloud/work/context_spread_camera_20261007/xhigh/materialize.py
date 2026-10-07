from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, re, time
from pycparser import c_parser

root=Path(__file__).parent
seed=(root/'baseline.c').read_text()
expected='1bac499d4feded2f749f32b3493e28beba8667bd493d385e1ca1332d5c0160b2'
assert hashlib.sha256(seed.encode()).hexdigest()==expected
helper_start=seed.index('static s32 set_volume(')
helper_end=seed.index('void camera_transform(void)')
helper_prefix=seed[:helper_start]
body=seed[helper_end:]
params=[('volume','func_8001FEA4','u8','value*127.0f'),
        ('pan','func_8001FFF4','u8','(value+1.0f)*0.5f*127.0f'),
        ('depth','func_8001FF4C','u8','(value+1.0f)*0.5f*127.0f'),
        ('rate','func_8001FFA0','u16','rate')]

def helpers(mode='fail', arithmetic='expression', positive_voice=False, rate_quantize=False, finite_rate=False, single_exit=False):
    out=[]
    for name,callback,cast,expr in params:
        declarations=[]
        calculation=[]
        arg='('+cast+')(u32)('+expr+')'
        if name=='rate':
            declarations.append('f32 rate;')
            if arithmetic=='staged':
                declarations.append('f32 product;')
                calculation += ['product=value*8192.0f;', 'rate=product-1.0f;']
            else:
                calculation.append('rate=value*8192.0f-1.0f;')
            if rate_quantize or finite_rate:
                declarations.append('u32 amount;')
                if finite_rate:
                    calculation += ['if (rate>0.0f) amount=(u32)rate;', 'else amount=0;']
                else:
                    calculation += ['if (rate<0.0f) amount=0;', 'else amount=(u32)rate;']
                arg='('+cast+')amount'
            else:
                calculation.append('if (rate<0.0f) rate=0.0f;')
        elif arithmetic=='staged':
            if name=='volume':
                declarations.append('f32 scaled;')
                calculation.append('scaled=value*127.0f;')
            else:
                declarations += ['f32 shifted;', 'f32 mapped;', 'f32 scaled;']
                calculation += ['shifted=value+1.0f;', 'mapped=shifted*0.5f;', 'scaled=mapped*127.0f;']
            arg='('+cast+')(u32)scaled'
        if arithmetic=='integer' and not (name=='rate' and (rate_quantize or finite_rate)):
            declarations.append('u32 amount;')
            calculation.append('amount=(u32)('+expr+');')
            arg='('+cast+')amount'
        call=callback+'(entry->voice,'+arg+')'
        if single_exit:
            declarations += ['s32 result;', 's32 success;']
            stmts=['success=1;', 'if (entry->voice!=(u32)-1) {']
            stmts += ['    '+line for line in calculation]
            stmts += ['    result='+call+';', '    if (result!=0) success=0;', '    else success=1;', '}', 'return success;']
        elif positive_voice:
            stmts=['if (entry->voice!=(u32)-1) {']
            stmts += ['    '+line for line in calculation]
            if mode=='success':
                stmts += ['    if ('+call+'==0) return 1;', '    return 0;']
            else:
                stmts += ['    if ('+call+'!=0) return 0;', '    return 1;']
            stmts += ['}', 'return 1;']
        else:
            stmts=['if (entry->voice==(u32)-1) return 1;'] + calculation
            if mode=='success':
                stmts += ['if ('+call+'==0) return 1;', 'return 0;']
            else:
                stmts += ['if ('+call+'!=0) return 0;', 'return 1;']
        out.append('static s32 set_'+name+'(Entry *entry,f32 value) {\n'+''.join('    '+l+'\n' for l in declarations+stmts)+'}\n')
    return ''.join(out)

def split_recycle(source):
    old='        if (entry && entry->stopRequested && node->kind!=2) goto recycle;'
    new='''        if (entry && entry->stopRequested && node->kind!=2) {
            func_8009211C(&D_80146160,node);
            func_80091FBC(&D_80146138,node,D_80146138.first);
            continue;
        }'''
    assert old in source
    source=source.replace(old,new).replace('recycle:\n','')
    old='        if (node->retries>=10 && node->kind!=0) goto recycle;'
    new='''        if (node->retries>=10 && node->kind!=0) {
            func_8009211C(&D_80146160,node);
            func_80091FBC(&D_80146138,node,D_80146138.first);
        }'''
    assert old in source
    return source.replace(old,new)

def retry_guards(source):
    old='''        if (node->retries>=10 && node->kind!=0) {
            func_8009211C(&D_80146160,node);
            func_80091FBC(&D_80146138,node,D_80146138.first);
        }'''
    new='''        if (node->retries<10) continue;
        if (node->kind==0) continue;
        func_8009211C(&D_80146160,node);
        func_80091FBC(&D_80146138,node,D_80146138.first);'''
    assert old in source
    return source.replace(old,new)

def operand_stages(source, nested_only=False):
    old='''        case 3:
            if (node->value!=-2.0f && !set_volume(entry,node->value)) goto retry;
            if (node->stream!=-2.0f && !set_pan(node->entry,node->stream)) goto retry;
            if (node->priority!=-2.0f && !set_depth(node->entry,node->priority)) goto retry;
            if (node->route!=-2.0f && !set_rate(node->entry,node->route)) goto retry;
            break;'''
    lines=['        case 3:']
    for field,name,entry in [('value','volume','entry'),('stream','pan','node->entry'),('priority','depth','node->entry'),('route','rate','node->entry')]:
        if nested_only:
            lines += ['            if (node->'+field+'!=-2.0f) {', '                if (!set_'+name+'('+entry+',node->'+field+')) goto retry;', '            }']
        else:
            lines += ['            {', '                f32 operand;', '                operand=node->'+field+';', '                if (operand!=-2.0f) {', '                    if (!set_'+name+'('+entry+',operand)) goto retry;', '                }', '            }']
    lines.append('            break;')
    assert old in source
    return source.replace(old,'\n'.join(lines))

records={}
variants={}
def add(identifier,helper_source,queue_body,edit,semantic,native,extra=None):
    source=helper_prefix+helper_source+queue_body
    variants[identifier]=source
    record={'source':'candidates/'+identifier+'.c','sha256':hashlib.sha256(source.encode()).hexdigest(),'edit':edit,'semantic_justification':semantic,'native_effect':native,'check':{'kind':'diagnostic','signal_ids':[]}}
    if extra: record.update(extra)
    records[identifier]=record

base_sem='The invalid-voice path still returns success without arithmetic or callbacks. Each real callback uses the same live voice, arithmetic order, casts and return-zero success contract. MP_TargetSteerPos and stop_entry remain exact seed bodies; declarations and types are unchanged.'
recycle_sem=' Cancellation, normal completion and exhausted-retry paths each preserve one ordered removal/insertion pair, reload the free-list head between calls, and advance using the successor captured before callbacks. Retry byte wrapping, blocked marking and the live kind-zero exemption are preserved.'
fp_sem=' The staged f32 assignments preserve the original operation order and single-precision value flow on the target; no algebraic reassociation is introduced. Excess-precision evaluation, exceptional values and FCSR effects are not established by this source argument.'

add('X01',helpers(),body,
    'Isolate explicit failure-first callback-result diamonds in all four setters without splitting recycling.',
    base_sem,
    'Separates the independently observed native setter diamonds from the previously regressing combined diamond/recycling reconstruction. Predicts extra setter branch blocks while retaining baseline list-call-site layout.')
add('X02',helpers(mode='success',positive_voice=True),split_recycle(body),
    'Use positive live-voice guards and success-first callback diamonds with three path-local recycling blocks.',
    base_sem+recycle_sem,
    'Unlike the known polarity-only control, moves invalid-voice success to the trailing helper return. Tests whether native guard orientation and shared success joins alter conversion-block placement and register webs.')
add('X03',helpers(arithmetic='integer'),split_recycle(body),
    'Materialize genuine u32 quantized operands before narrow setter calls, retaining failure diamonds and path-local recycling.',
    base_sem+' Each u32 amount is precisely the existing inner cast, consumed by the unchanged outer u8/u16 cast; no conversion is hoisted before the invalid-voice guard.'+recycle_sem,
    'Tests integer conversion-result lifetime independently of float expression grouping. Native conversion machinery feeds byte/halfword API arguments; a real quantized operand can alter conversion/argument allocation without pressure-only storage.')
add('X04',helpers(arithmetic='staged'),split_recycle(body),
    'Expose the real float arithmetic stages in setter-local f32 values, with native-shaped diamonds and recycling.',
    base_sem+fp_sem+recycle_sem,
    'Tests the lifetimes of shifted, half-scaled and final-scaled pan/depth values and a separate rate product/subtraction. Native FP temporaries and rate subtraction order motivate these actual value webs; the missing private helper ABI is not repaired.')
add('X05',helpers(rate_quantize=True),body,
    'Form the rate result with a subtraction-tested u32 quantization join while isolating setter diamonds from recycling.',
    base_sem+' Rate computes value*8192-1 before its sign test. A negative result directly selects unsigned zero; other results use the same u32 conversion as baseline. For ordinary finite conversion-domain inputs this equals clamping the float to zero first. NaN/infinity, out-of-range casts and FCSR behavior remain uncertain.',
    'The conditional integer join makes the rate value feed the comparison and conversion separately. It may retain native subtraction-before-zero testing rather than the baseline compiler\'s product-versus-one rewrite; no instruction-shape claim is assumed.',
    {'semantic_scope':'ordinary finite conversion-domain inputs; FP exceptional/FCSR behavior not verified'})
add('X06',helpers(),operand_stages(split_recycle(body)),
    'Combine per-stage command f32 operands with setter failure diamonds and three local recycle sites.',
    base_sem+' Each operand is loaded only after earlier setter callbacks and used for its sentinel test and call. Later entry pointers remain live node reloads; the first volume stage retains the original pre-dispatch entry.'+recycle_sem,
    'Tests genuine call-argument lifetimes together with the recovered native branch/call-site structure. The earlier operand-only experiment lacked these helper diamonds; this is a bounded interaction hypothesis rather than a resubmitted source.')
add('X07',helpers(arithmetic='integer',single_exit=True),body,
    'Give each setter a single-exit success join and real callback-result/quantized-operand locals.',
    base_sem+' Success begins at one for the absent-voice path. The live-voice callback result alone selects zero or one at the shared return, with no added callback or memory effect.',
    'Tests the original-source possibility of a result variable and shared helper success join. These are actual semantic results, not artificial preserved state. IDO may eliminate the locals or choose branchless lowering; that negative outcome would be diagnostic.')
add('X08',helpers(),retry_guards(operand_stages(split_recycle(body),nested_only=True)),
    'Combine explicit per-parameter sentinel scopes, setter diamonds and two retry-exhaustion skip edges.',
    base_sem+' Nested stage guards preserve sentinel short-circuit order and all post-callback field/entry reloads. The retry tail first skips sub-threshold retries, then exempts kind zero, reading both after blocked marking.'+recycle_sem,
    'Tests whether the native independent per-stage diamonds and retry skip edges survive as separate CFG joins once genuine recycling sites are present. It differs from the known all-diamond/full-split candidate in both command-stage and exhaustion structure.')
add('X09',helpers(arithmetic='staged',finite_rate=True),split_recycle(body),
    'Use a positive post-subtraction rate quantization guard with staged f32 arithmetic and native-shaped control flow.',
    base_sem+fp_sem+' For finite values in the defined conversion domain, positive rate casts identically; zero or negative rate converts to unsigned zero. This version intentionally routes unordered NaN to zero instead of attempting baseline\'s nonportable float-to-u32 conversion. FP exceptional/FCSR equivalence is explicitly unresolved.'+recycle_sem,
    'A bounded ordinary-finite hypothesis for the native subtraction-before-zero guard. Positive-rate conversion versus a zero integer arm may change compare polarity, rate lifetime and conversion placement; it is not a general exceptional-input equivalence claim.',
    {'semantic_scope':'ordinary finite conversion-domain inputs only; NaN behavior intentionally differs at source level and FP/FCSR equivalence is unresolved'})
add('X10',helpers(mode='success',arithmetic='staged',positive_voice=True),retry_guards(operand_stages(split_recycle(body))),
    'Integrate live-voice success joins, staged setter arithmetic, stage-local command operands and path-local recycling.',
    base_sem+fp_sem+' Operand locals are refreshed after each earlier callback; only the initial volume stage uses the pre-dispatch entry, matching baseline. Retry skip guards preserve byte wraparound and evaluate the kind exemption only after blocked marking.'+recycle_sem,
    'A combined source-reconstruction hypothesis using only independently justified real lifetimes and native control-flow features. Tests their interaction without a keeper, invented caller, pressure-only local, compiler change, or claim to recover the still-unresolved private helper context.')

assert set(variants)=={'X%02d'%i for i in range(1,11)}
parser=c_parser.CParser()
old_paths=list(Path('cloud/work/large_effort_camera_20261007').glob('*/candidates/*.c'))+[Path('cloud/work/camera_context_20261007/candidate.c'),Path('cloud/work/lean_camera_transform_20261006/candidate.c')]
# Compare actual token-like text as well as byte hashes so formatting is not a new proposal.
normalize=lambda s:re.sub(r'\s+','',re.sub(r'/\*.*?\*/','',s,flags=re.S))
old_sources={normalize(p.read_text()) for p in old_paths}
new_norm=[]
for identifier,source in variants.items():
    parser.parse(re.sub(r'/\*.*?\*/','',source,flags=re.S))
    assert source.startswith(helper_prefix)
    assert normalize(source) not in old_sources,identifier+' repeats an old complete source'
    new_norm.append(normalize(source))
    (root/records[identifier]['source']).write_text(source)
assert len(set(new_norm))==10
(root/'predictions.json').write_text(json.dumps(records,indent=2)+'\n')
(root/'README.md').write_text('''# Xhigh source-structure-informed camera lane

Ten complete proposals start from the exact clean 533/557 seed. All keep the
actual exported MP_TargetSteerPos and exact stop_entry body. No compiler, scorer,
custom semantic suite or additional context probe was run before freeze.
Pycparser syntax checks and complete-source integrity/deduplication checks passed.

The portfolio separates setter branch diamonds from list-site duplication, then
varies real arithmetic/quantization lifetimes, success joins and parameter-stage
scopes. The already known combined failure-diamond/full-recycling source and its
polarity-only control are not resubmitted. Prior candidate sources were consulted
only to avoid repeating complete proposals and known no-information outcomes.

X05 and X09 are explicit ordinary-finite, defined-conversion-domain rate
hypotheses. X09 deliberately takes unordered NaN to integer zero; no exceptional
input, out-of-range conversion, excess-precision or FCSR equivalence is claimed.
Other staged-float proposals also retain the stated excess-precision/FCSR limits.
Native private helper visibility/inlining context remains unresolved for all ten.

Timing is the real marked planning/tool wall window. Initial assignment and
shared-document reads occurred before the marker and are not backdated. The
window includes reading, reasoning, source materialization and allowed checks;
it is not active LLM inference time. Candidate sources are immutable after freeze.
''')
(root/'source-checks.json').write_text(json.dumps({'candidate_count':10,'pycparser_pass':True,'declarations_types_helper_prefix_preserved':True,'stop_entry_unchanged':True,'MP_TargetSteerPos_unchanged_exported':True,'distinct_complete_sources':10,'distinct_normalized_complete_sources':10,'old_complete_sources_checked':len(old_paths),'no_old_complete_source_duplicates':True,'compilers_run':False,'scorers_run':False,'semantic_suite_run':False},indent=2)+'\n')
start=json.loads((root/'planning-events.jsonl').read_text().splitlines()[0])
end={'event':'freeze','utc':datetime.now(timezone.utc).isoformat(),'monotonic_s':time.monotonic()}
end['elapsed_planning_wall_s']=end['monotonic_s']-start['monotonic_s']
with (root/'planning-events.jsonl').open('a') as f: f.write(json.dumps(end)+'\n')
freeze={'lane':'xhigh','requested_model':'gpt-6.1-sol','requested_effort':'xhigh','baseline_source':'baseline.c','baseline_sha256':expected,'candidate_count':10,'source_hashes':{k:v['sha256'] for k,v in records.items()},'planning_start':start,'freeze':end,'elapsed_planning_wall_s':end['elapsed_planning_wall_s'],'timing_kind':'observed marked elapsed wall window, not active inference','claims':[]}
(root/'freeze.json').write_text(json.dumps(freeze,indent=2)+'\n')
print(json.dumps({'frozen':True,'lane':'xhigh','candidate_count':10,'elapsed_planning_wall_s':end['elapsed_planning_wall_s'],'freeze_utc':end['utc'],'baseline_sha256':expected,'syntax_integrity_checks':'passed','semantic_limits':'X05/X09 ordinary finite conversion-domain rate hypotheses; X09 NaN arm differs'}))
