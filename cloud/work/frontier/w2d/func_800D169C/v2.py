DOT="    dot = delta1[0] * delta2[0] + delta1[1] * delta2[1] + delta2[2] * delta1[2];\n    if (dot < 0.0f && close_index > 0) {"
V={
 'dx': [("    f32 dot;\n",""),(DOT, DOT.replace("dot","dx"))],
 'dxk': [(DOT, DOT.replace("dot","dx"))],
 'dist': [("    f32 dot;\n",""),(DOT, DOT.replace("dot","dist"))],
}
