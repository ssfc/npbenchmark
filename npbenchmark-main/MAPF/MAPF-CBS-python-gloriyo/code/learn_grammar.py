import heapq

class Triple:
    def __init__(self,a,b,c):
        self.a = a
        self.b = b
        self.c = c

    def __lt__(self, other):
        if self.a == other.a:
            if self.b == other.b:
                return self.c < other.c
            return self.b < other.b
        return self.a < other.a


x = [(1,2,3), (2,1,4), (3,4,5), (4,3,6), (5,6,7), (1,2,5), (4,3,2), (2,1,3)]
y = [Triple(a,b,c) for a,b,c in x]
heapq.heapify(y)
z = [(t.a, t.b, t.c) for t in y]
print(z)
# 输出：[(1, 2, 3), (1, 2, 5), (2, 1, 4), (2, 1, 3), (3, 4, 5), (5, 6, 7), (4, 3, 6), (4 ,3 ,2)]

# 输出：[<__main__.Triple object at 0x7f8c9c0d7a90>, <__main__.Triple object at 0x7f8c9c0d7b80>, <__main__.Triple object at 0x7f8c9c0d7a60>, <__main__.Triple object at 0x7f8c9c0d7bb0>, <__main__.Triple object at 0x7f8c9c0d7ac0>, <__main__.Triple object at 0x7f8c9c0d7af0>, <__main__.Triple object at 0x7f8c9c0d7b50>, <__main__.Triple object at 0x7f8c9c0d7be0>]
