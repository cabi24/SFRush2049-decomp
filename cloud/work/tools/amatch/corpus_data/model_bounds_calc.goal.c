s32 model_bounds_calc(s32 flag, ModelObj *obj) {
    if (flag) {
        model_transform_setup(obj->model, 0, 15);
    } else {
        model_data_load(obj->model, 1, 15);
    }
    return flag;
}