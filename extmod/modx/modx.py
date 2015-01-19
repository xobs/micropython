import micropython
ret = micropython.load('modx.mpy')
print(ret)
print(ret[5](42))
