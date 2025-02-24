#include <iostream>
#include <vector>
#include <algorithm>

#include "graph.hpp"


/**
 * @brief Класс для поиска мостов и точек сочленения в графе.
 * 
 * @tparam Graph Тип графа.
 */
template <typename Graph, typename WeightType>
class BridgesAndArticulationPoints {
 public:
    /**
     * @brief Конструктор класса.
     * 
     * @param g Константная ссылка на граф.
     */
    BridgesAndArticulationPoints(const Graph& g)
        : graph(g), time(0), time_in(g.Size(), -1), low(g.Size(), -1), 
          visited(g.Size(), Color::White), parent(g.Size(), -1) {}

    /**
     * @brief Находит мосты и точки сочленения в графе.
     */
    void findBridgesAndAPs() {
        for (size_t u = 0; u < graph.Size(); ++u) {
            if (visited[u] == Color::White) {
                rootChildren = 0;
                BridgeDFS(u);
                // Проверка для корня DFS
                if (rootChildren > 1) {
                    articulationPoints.insert(u);
                }
            }
        }
    }

    std::vector<Edge<WeightType>> getBridges() {
        return bridges;
    }

    std::unordered_set<size_t> getArticulationPoints() {
        return articulationPoints;
    }

    /**
     * @brief Печатает найденные мосты и точки сочленения.
     */
    void printResults() const {
        std::cout << "Bridges:\n";
        for (auto& bridge : bridges) {
            std::cout << bridge.first << " - " << bridge.second << "\n";
        }
        
        std::cout << "\nArticulation Points:\n";
        for (auto& ap : articulationPoints) {
            std::cout << ap << "\n";
        }
    }

 private:
    const Graph& graph;                            //!< Ссылка на граф
    size_t time;                                   //!< Время входа в вершину
    std::vector<int> time_in, low;                 //!< Времена входа и минимальные времена
    std::vector<Color> visited;                    //!< Статусы посещения вершин
    std::vector<int> parent;                       //!< Родители вершин в DFS дереве
    std::vector<Edge<WeightType>> bridges;         //!< Найденные мосты
    std::unordered_set<size_t> articulationPoints; //!< Найденные точки сочленения
    size_t rootChildren;                           //!< Количество детей у корня DFS

    /**
     * @brief Рекурсивная функция DFS для поиска мостов и точек сочленения.
     * 
     * @param u Текущая вершина.
     */
    void BridgeDFS(size_t u) {
        visited[u] = Color::Gray;
        time_in[u] = time;
        low[u] = time++;
        
        for (size_t v : graph.neighbors(u)) {
            if (visited[v] == Color::White) { // Tree Edge
                parent[v] = u;
                if (parent[u] == -1) ++rootChildren; // Подсчет детей корня
                
                BridgeDFS(v);

                // Проверка на мост
                if (low[v] > time_in[u]) {
                    bridges.push_back({u, v});
                }

                // Проверка на точку сочленения (для не корня)
                if (low[v] >= time_in[u] && parent[u] != -1) {
                    articulationPoints.insert(u);
                }

                // Обновление low для текущей вершины
                low[u] = std::min(low[u], low[v]);
            }
            else if (v != parent[u]) { // Back Edge
                low[u] = std::min(low[u], time_in[v]);
            }
        }

        visited[u] = Color::Black;
    }
};
