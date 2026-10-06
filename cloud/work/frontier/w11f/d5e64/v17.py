exec(open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11f/d5e64/v8.py').read().split('V = {}')[0])
b0 = H1 + R(R(B, '&D_8010FFC4[player]', 'rdy(player)'), ' s8 *ready;\n', ' f32 vec[3];\n s8 *ready;\n')
V = {}
IF1 = '''   if(vehicle->mode==2)state->pair[i].handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
   else state->pair[i].handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
'''
V['neq'] = R(b0, IF1, '''   if(vehicle->mode!=2)state->pair[i].handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);
   else state->pair[i].handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);
''')
V['rev'] = R(b0, 'vehicle->mode==2', '2==vehicle->mode')
V['sw'] = R(b0, IF1, '''   switch(vehicle->mode){case 2:state->pair[i].handle=high_scores_display(object,player,0,0,0.0f,0.0f,0.0f);break;
   default:state->pair[i].handle=camera_target_track(D_801141B0,D_801141B0,400.0f,0.0f,1.0f,0.0f,object,player,0,128);break;}
''')
V['le1'] = R(b0, 'i<2;', 'i<=1;')
V['nest'] = R(b0, '  if(object!=-1) {\n', '  if(object!=-1) {\n').replace('  object=state->definition->item[i].object;\n','  object=state->definition->item[i].object;\n')
V['early'] = R(R(b0, '  if(object!=-1) {\n', '  if(object==-1) continue;\n  {\n'), 'xx', 'xx') if 'xx' in b0 else R(b0, '  if(object!=-1) {\n', '  if(object==-1) continue;\n  {\n')
