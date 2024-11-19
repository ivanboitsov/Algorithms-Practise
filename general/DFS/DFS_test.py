import random
import time
import sys
import matplotlib.pyplot as plt
from collections import defaultdict

sys.stdout.reconfigure(encoding='utf-8')

# Класс графа
class Graph:
    def __init__(self):
        self.graph = defaultdict(list)

    def add_edge(self, u, v, weight):
        self.graph[u].append((v, weight))

    def find_max_path(self, start, end, visited=None, current_sum=0):
        if visited is None:
            visited = set()
        if start == end:
            return current_sum

        visited.add(start)
        max_path = float('-inf')

        for neighbor, weight in self.graph[start]:
            if neighbor not in visited:
                max_path = max(max_path, self.find_max_path(neighbor, end, visited, current_sum + weight))

        visited.remove(start)
        return max_path

# Генерация графа с заданным числом вершин и рёбер
def generate_graph(num_vertices, num_edges):
    g = Graph()
    vertices = [str(i) for i in range(1, num_vertices + 1)]
    for _ in range(num_edges):
        u, v = random.sample(vertices, 2)
        weight = random.randint(1, 10)
        g.add_edge(u, v, weight)
    return g, vertices[0], vertices[-1]

# Тестирование времени работы
def measure_time(graph, start, end):
    start_time = time.time()
    graph.find_max_path(start, end)
    end_time = time.time()
    return end_time - start_time

# Создание графов и измерение времени
examples = [
    (5, 7),
    (10, 15),
    (15, 25),
    (20, 35),
    (25, 50),
    (30, 65),
    (35, 85),
    (40, 105),
    (45, 130),
    (50, 160),
]

times = []
sizes = []

for num_vertices, num_edges in examples:
    g, start, end = generate_graph(num_vertices, num_edges)
    exec_time = measure_time(g, start, end)
    times.append(exec_time)
    sizes.append(num_vertices)

# Визуализация сложности
plt.figure(figsize=(10, 6))
plt.plot(sizes, times, marker="o")
plt.title("Зависимость времени выполнения от числа вершин")
plt.xlabel("Число вершин в графе")
plt.ylabel("Время выполнения (секунды)")
plt.grid(True)
plt.show()