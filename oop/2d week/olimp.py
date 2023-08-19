for a in range(0, 15):
    for b in range(0, 15):
        for c in range(0, 15):
            for d in range(0, 15):
                if (a+b+c+d) == 14 and (2*b + 5*d) == (6*a + 15*c):
                    print(a, b, c, d)