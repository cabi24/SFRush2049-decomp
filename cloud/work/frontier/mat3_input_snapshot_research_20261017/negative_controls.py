from host_behavior import *
s=extracted_source()
negatives={
 'wrong_y_calibration':s.replace('(gStickCal[i][1] < gStickRaw[i][1] || gStickRaw[i][1] < -gStickCal[i][1]) && gStickCal[i][1] < 127','(gStickCal[i][0] < gStickRaw[i][1] || gStickRaw[i][1] < -gStickCal[i][0]) && gStickCal[i][0] < 127').replace('gStickCal[i][1]++;','gStickCal[i][0]++;'),
 'collapsed_y_output':s.replace('gInputStick[i][1] = func_', 'gInputStick[0][1] = func_'),
 'wrong_direction_threshold':s.replace('gStickRaw[i][0] > 56','gStickRaw[i][0] > 57'),
 'edge_history_overwrite':s.replace('gPrevPressed[i] |= pressed;', 'gPrevPressed[i] = pressed;'),
}
def verify_negatives():
 result={}
 for name,source in negatives.items():
  require(source!=s,'inert negative')
  try:verify_host(source)
  except ValueError as e:
   require(str(e).startswith('host mismatch'),f'negative failed for unrelated reason {e}');result[name]=str(e)
  else:raise ValueError('negative escaped '+name)
 return result
