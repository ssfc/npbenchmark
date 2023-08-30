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

while y:
    popped_element = heapq.heappop(y)
    print(popped_element.a, popped_element.b, popped_element.c)