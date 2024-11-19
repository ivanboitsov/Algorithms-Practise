from collections import defaultdict
import sys

sys.stdout.reconfigure(encoding='utf-8')

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


# Пример работы
g = Graph()
g.add_edge("A", "B", 3)
g.add_edge("A", "C", 2)
g.add_edge("B", "D", 4)
g.add_edge("C", "D", 1)

start, end = "A", "D"
result = g.find_max_path(start, end)
print(f"Максимальный путь из {start} в {end}: {result}")