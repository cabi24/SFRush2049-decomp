"""Independently compile and reject changed initialization contracts."""
from init_behavior import *
import shutil

def verify_negatives(group):
 group=Path(group);source=(group/'group.c').read_text();out={}
 changes={
  'misplaced_first_y_reset':('D_80149AF8[i][0] = D_80149AF8[i][1] = 70;','D_80149AF8[i][0] = 70; if (i != 0 || D_80111968 == 0) D_80149AF8[i][1] = 70;'),
  'word_width_query':('extern volatile u8 D_80111950;','extern volatile s32 D_80111950;'),
  'wrong_queue_capacity':('(void **) &D_80152730, 1);','(void **) &D_80152730, 2);'),
  'blocked_queue_seed':('osSendMesg((OSMesgQueue *) &D_801497A8, NULL, 0);','osSendMesg((OSMesgQueue *) &D_801497A8, NULL, 1);'),
  'repeat_player_defaults':('if (D_80111954 == 0) {','if (1) {'),
 }
 a=source.index('void func_800E7038(void) {');b=source.index('/* Transfer controller snapshots',a)
 for label,(old,new) in changes.items():
  body=source[a:b]
  if label=='word_width_query':require(source.count(old)==1,'ambiguous query declaration');changed=source.replace(old,new)
  else:require(body.count(old)==1,'ambiguous initialization mutation');changed=source[:a]+body.replace(old,new)+source[b:]
  with tempfile.TemporaryDirectory(prefix='init-negative-') as tmp:
   d=Path(tmp)/'group';shutil.copytree(group,d);(d/'group.c').write_text(changed);obj=Path(tmp)/'bad.o';score.compile_group(d,obj)
   try:verify(obj)
   except ValueError as e:
    expected={'misplaced_first_y_reset':'case 18, program 1','word_width_query':'query status is not byte-wide','wrong_queue_capacity':'queue creation protocol','blocked_queue_seed':'queue seed protocol','repeat_player_defaults':'case 54, program 1'}
    require(str(e)==expected[label],'negative failed for unrelated reason: '+str(e));out[label]={'rejected':True,'reason':str(e)}
   else:raise ValueError('negative escaped: '+label)
 return out
if __name__=='__main__':print(json.dumps(verify_negatives(sys.argv[1]),indent=2))
