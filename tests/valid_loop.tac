decl int ক
ক = 1
L1:
t1 = ক <= 3
if t1 goto L2
goto L3
L2:
print ক
t2 = ক + 1
ক = t2
goto L1
L3:
ক = 1
L4:
t3 = ক <= 3
if t3 goto L5
goto L6
L5:
print ক
t4 = ক + 1
ক = t4
goto L4
L6:
