func যোগফল:
return 42
endfunc
decl int ক
decl int খ
decl int i
decl int ফ
decl int ইন
decl float ভ
read ইন
ক = 5
t1 = ক + 6
খ = t1
ভ = ক
t2 = call যোগফল, 0
ফ = t2
t3 = খ > 10
if t3 goto L1
goto L2
L1:
print খ
goto L3
L2:
print 0
L3:
i = 1
L4:
t4 = i <= 3
if t4 goto L5
goto L6
L5:
print i
t5 = i + 1
i = t5
goto L4
L6:
i = 1
L7:
t6 = i <= 3
if t6 goto L8
goto L9
L8:
t7 = i + ইন
print t7
t8 = i + 1
i = t8
goto L7
L9:
print ফ
print ভ
