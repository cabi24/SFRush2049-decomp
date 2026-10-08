/* Host-only native-fixture replay of the unchanged matching source. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cloud/matches/dot_route_crossing_20261006/group.c"
RouteSet D_801407F0;
TrackerSet D_80151CE8;
f32 D_80152800, D_801543AC;
s16 func_800BA61C(s16 n) { (void)n; abort(); return 0; }
s16 func_800BA2B8(s16 a, s16 b) { (void)a; (void)b; abort(); return 0; }
f32 audio_channel_alloc(s32 a, s32 b) { (void)a; (void)b; abort(); return 0; }
int main(int argc, char **argv)
{
    FILE *input;
    unsigned int raw_who, raw_route;
    int count, type, num, range, expected, x, y, z, i, who, got;
    float px, pz, dx, dz;
    unsigned long cases = 0;
    Route routes[15];
    RoutePoint points[32767];
    TrackerSet saved;
    if (argc != 2 || (input = fopen(argv[1], "r")) == NULL) return 2;
    while (fscanf(input, "%u %u %d %d %d %d %f %f %f %f %d", &raw_who, &raw_route,
                  &count, &type, &num, &range, &px, &pz, &dx, &dz, &expected) == 11) {
        if (num < 0 || num > 32767 || count < 0 || count > 15) return 3;
        for (i = 0; i < num; ++i) {
            if (fscanf(input, "%d %d %d", &x, &y, &z) != 3) return 4;
            points[i].x = (s16)x; points[i].y = (s16)y; points[i].z = (s16)z;
        }
        memset(&D_80151CE8, 0xa5, sizeof D_80151CE8);
        memset(routes, 0x5a, sizeof routes);
        for (i = 0; i < 15; ++i) { routes[i].type = (u8)type; routes[i].num = (u16)num; routes[i].pts = points; }
        D_801407F0.count = (u8)count; D_801407F0.routes = routes;
        who = (s16)raw_who;
        if (who < 0 || who >= 10) return 5;
        D_80151CE8.tracks[who].x = px; D_80151CE8.tracks[who].z = pz;
        D_80151CE8.tracks[who].dx = dx; D_80151CE8.tracks[who].dz = dz;
        D_80151CE8.tracks[who].range = range;
        saved = D_80151CE8;
        got = audio_priority_find((s16)raw_who, (s16)raw_route);
        if (got != expected || memcmp(&saved, &D_80151CE8, sizeof saved)) {
            fprintf(stderr, "case %lu: got %d expected %d\n", cases, got, expected); return 1;
        }
        ++cases;
    }
    fclose(input);
    printf("%lu cases passed\n", cases);
    return 0;
}
