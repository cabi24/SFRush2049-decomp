FS=('s32 frame_sync(s32,s16,s32,s32);','s32 frame_sync(s32,s32,s32,s32);')
W=('weighted=D_8011F060[i]*model->power[i];','weighted=model->power[i]*D_8011F060[i];')
V={'fs':[FS],'fs_w':[FS,W]}
