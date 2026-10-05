/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research NONMATCH: snapshot selected scalar/vector fields and a 3x3 basis.
 * The coefficient 2.72727275f is a natural compiler-owned float verified
 * against the protected own-data artifact. Offsets describe the observed
 * object fields; no arcade donor or semantic field names are established.
 * The amount is stored directly, without a register-shaping temporary.
 */
typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef float f32;
#define FLOAT_AT(p,n) (*(f32 *)((u8 *)(p)+(n)))
extern void math_utility(f32 *,f32 *);
void func_800D4DFC(void *object) {
 math_utility(&FLOAT_AT(object,748),&FLOAT_AT(object,1952));
 *(s16 *)((u8 *)object+1880)=(s32)(((FLOAT_AT(object,1328)*FLOAT_AT(object,1256)+FLOAT_AT(object,1420)*FLOAT_AT(object,1348))*0.5f)*2.72727275f);
 FLOAT_AT(object,1884)=FLOAT_AT(object,1484);
 FLOAT_AT(object,1900)=FLOAT_AT(object,1516);
 FLOAT_AT(object,1888)=FLOAT_AT(object,1488);
 FLOAT_AT(object,1904)=FLOAT_AT(object,1520);
 FLOAT_AT(object,1892)=FLOAT_AT(object,1492);
 FLOAT_AT(object,1908)=FLOAT_AT(object,1524);
 FLOAT_AT(object,1896)=FLOAT_AT(object,1496);
 FLOAT_AT(object,1912)=FLOAT_AT(object,1528);
 FLOAT_AT(object,1916)=FLOAT_AT(object,1844);
 FLOAT_AT(object,1920)=FLOAT_AT(object,1848);
 FLOAT_AT(object,1924)=FLOAT_AT(object,1852);
 FLOAT_AT(object,1928)=FLOAT_AT(object,1856);
 FLOAT_AT(object,1932)=FLOAT_AT(object,1860);
 FLOAT_AT(object,1936)=FLOAT_AT(object,1864);
 FLOAT_AT(object,1940)=FLOAT_AT(object,1868);
 FLOAT_AT(object,1944)=FLOAT_AT(object,1872);
 FLOAT_AT(object,1948)=FLOAT_AT(object,1876);
}
