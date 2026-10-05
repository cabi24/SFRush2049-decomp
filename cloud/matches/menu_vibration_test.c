/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Historical label menu_vibration_test is misleading: this is arcade
 * make_uvs_from_quat (reference/repos/rushtherock/game/resurrect.c:874),
 * "create uv matrix from quaternion", proven by the body: the arcade source
 * compiles to the retail words unchanged except that the three double
 * constants in `s = (Nq > 0.0) ? (1.0 / Nq) : 0.0;` are float on the N64
 * (0.0f, 1.0f, 0.0f).  Declarations, statement order and the integer `2`
 * are the arcade's.  Also matches at -O2.  No shaping quirk.
 */
typedef float F32;

void menu_vibration_test(F32 q[], F32 uvs[][3]) {
    F32 Nq, q1, q2, q3, q4, q11, q22, q33, q44;
    F32 q12, q13, q14, q23, q24, q34;
    F32 s;

    q1 = q[0];      q2 = q[1];      q3 = q[2];      q4 = q[3];

    q11 = q1*q1;    q22 = q2*q2;    q33 = q3*q3;    q44 = q4*q4;
    q14 = q1*q4;    q13 = q1*q3;    q12 = q1*q2;
    q24 = q2*q4;    q23 = q2*q3;
    q34 = q3*q4;

    Nq = q11 + q22 + q33+ q44;
    s = (Nq > 0.0f) ? (1.0f / Nq) : 0.0f;

    uvs[0][0] = s * (q11 + q22 - q33 - q44);
    uvs[0][1] = s * (2 * (q23 - q14));
    uvs[0][2] = s * (2 * (q24 + q13));
    uvs[1][0] = s * (2 * (q23 + q14));
    uvs[1][1] = s * (q11 - q22 + q33 - q44);
    uvs[1][2] = s * (2 * (q34 - q12));
    uvs[2][0] = s * (2 * (q24 - q13));
    uvs[2][1] = s * (2 * (q34 + q12));
    uvs[2][2] = s * (q11 - q22 - q33 + q44);
}
