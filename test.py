def muls(s):
    w = [ 2, 3, 6, 7, 5, 9, 3, 4, 5]
    v = [6, 5, 8, 9, 6, 7, 3, 6, 8]

    ws = 0;
    vs = 0;
    print(s.split())
    for i, data in enumerate(s.split()):
        f = float(data)
        print(f)
        ws += w[i] * f
        vs += v[i] * f 

    return ws, vs

x = input()
print(muls(x))
