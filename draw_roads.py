import matplotlib.pyplot as plt

citys = open('tenCitys.txt', 'r')
x = []
y = []
for i in range(10):
    a, b = citys.readline().split()
    x.append(int(a))
    y.append(int(b))
    
roads = open("city_roads.txt", 'r')
data = [int(c) for c in roads.readline().split()]
print data
plt.plot(x, y, 'ro')
for i in range(9):
    plt.plot([x[data[i]], x[data[i+1]]], [y[data[i]], y[data[i+1]]])
plt.show()
citys.close()
roads.close()