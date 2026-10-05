/* Compile the unchanged matching source on the host, with layout assertions. */
#include <stddef.h>
#include <string.h>
#include CANDIDATE_SOURCE
#define CHECK(name, condition) typedef char name[(condition) ? 1 : -1]
CHECK(float_width, sizeof(f32) == 4);
CHECK(short_width, sizeof(s16) == 2);
CHECK(object_width, sizeof(ModelSnapshot) == 1988);
CHECK(uv_offset, offsetof(ModelSnapshot, uv) == 748);
CHECK(radius2_offset, offsetof(ModelSnapshot, rear2_radius) == 1256);
CHECK(angvel2_offset, offsetof(ModelSnapshot, rear2_angvel) == 1328);
CHECK(radius3_offset, offsetof(ModelSnapshot, rear3_radius) == 1348);
CHECK(angvel3_offset, offsetof(ModelSnapshot, rear3_angvel) == 1420);
CHECK(suscomp_offset, offsetof(ModelSnapshot, suscomp) == 1484);
CHECK(airdist_offset, offsetof(ModelSnapshot, airdist) == 1516);
CHECK(base_vectors_offset, offsetof(ModelSnapshot, base_vectors) == 1844);
CHECK(speed_offset, offsetof(ModelSnapshot, speed_scaled) == 1880);
CHECK(saved_suscomp_offset, offsetof(ModelSnapshot, reckon_suscomp) == 1884);
CHECK(saved_airdist_offset, offsetof(ModelSnapshot, reckon_airdist) == 1900);
CHECK(saved_vectors_offset, offsetof(ModelSnapshot, reckon_vectors) == 1916);
CHECK(saved_uv_offset, offsetof(ModelSnapshot, reckon_uv) == 1952);
static unsigned int calls;
void math_utility(f32 *source, f32 *destination)
{
    int i;
    for (i = 0; i < 9; i++) destination[i] = source[i];
    calls++;
}
unsigned int run_case(const unsigned char *input, unsigned char *output)
{
    struct Record { ModelSnapshot value; unsigned char tail[68]; } record;
    memcpy(&record, input, sizeof(record));
    calls = 0;
    func_800D4DFC(&record.value);
    memcpy(output, &record, sizeof(record));
    return calls;
}
