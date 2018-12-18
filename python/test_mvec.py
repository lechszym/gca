from gca import mvec


m1 = mvec("3 e1 + 1.2 e1^e2")
m2 = mvec("3 e2 - 1.2 e1^e2")

m3 = m1*m2

print("%s*%s=%s" % (m1,m2,m3))
