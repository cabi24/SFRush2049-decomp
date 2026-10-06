s=s.replace("\tmove\t$4, $2\n","",1)
i=s.index("\tsw\t$0, 0($20)\n")
s=s[:i]+"\tsw\t$0, 0($20)\n\tmove\t$4, $2\n"+s[i+len("\tsw\t$0, 0($20)\n"):]
