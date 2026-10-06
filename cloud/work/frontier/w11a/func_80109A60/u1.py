OLD = """    if (world_height < world_width) {
        map_width = D_801161C4 - 8;
        map_height = (D_801161C4 - 8) * world_height / world_width;
    } else {
        map_height = D_801161C4 - 8;
        map_width = (D_801161C4 - 8) * world_width / world_height;
    }"""
def arms(w1, h1, h2, w2):
    return """    if (world_height < world_width) {
        map_width = %s;
        map_height = %s;
    } else {
        map_height = %s;
        map_width = %s;
    }""" % (w1, h1, h2, w2)
D = "D_801161C4 - 8"
V = {
 'a1': arms(D, "(s32)((D_801161C4 - 8U) * world_height) / world_width", D, "(s32)((D_801161C4 - 8U) * world_width) / world_height"),
 'a2': arms("D_801161C4 - 8U", "(s32)((D_801161C4 - 8U) * world_height) / world_width", "D_801161C4 - 8U", "(s32)((D_801161C4 - 8U) * world_width) / world_height"),
 'a3': arms(D, "(s32)(world_height * (u32)(D_801161C4 - 8)) / world_width", D, "(s32)(world_width * (u32)(D_801161C4 - 8)) / world_height"),
 'a4': arms(D, "(s32)((u32)(D_801161C4 - 8) * world_height) / world_width", D, "(s32)((u32)(D_801161C4 - 8) * world_width) / world_height"),
 'a5': arms(D, "(s32)((D_801161C4 - 8) * (u32)world_height) / world_width", D, "(s32)((D_801161C4 - 8) * (u32)world_width) / world_height"),
 'a6': arms(D, "(s32)((u32)world_height * (D_801161C4 - 8)) / world_width", D, "(s32)((u32)world_width * (D_801161C4 - 8)) / world_height"),
 'a7': arms(D, "world_height * (D_801161C4 - 8) / world_width", D, "world_width * (D_801161C4 - 8) / world_height"),
 'a8': arms(D, "(D_801161C4 + -8) * world_height / world_width", D, "(D_801161C4 + -8) * world_width / world_height"),
 'a9': arms(D, "(-8 + D_801161C4) * world_height / world_width", D, "(-8 + D_801161C4) * world_width / world_height"),
}
