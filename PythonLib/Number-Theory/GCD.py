def SteinGCD(x,y):
    if x==0:return y
    if y==0:return x
    
    gcdExponent=((x|y)&-(x|y)).bit_length()-1
    
    x>>=(x&-x).bit_length()-1
    y>>=(y&-y).bit_length()-1

    while x!=y:
        if x<y:x,y=x,y
        x-=y
        x>>=(x&-x).bit_length()-1

    return x<<gcdExponent

def EuclideanGCD(x,y):
    while x%y:
      z=x%y
      x=y
      y=z
    return y
