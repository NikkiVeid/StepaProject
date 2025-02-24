#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <stdexcept>
#include "graph.hpp"

/**
 * @brief Класс BFSVisitor для обработки событий при обходе графа в ширину.
 */
struct BFSVisitor {
    /**
     * @brief Вызывается при обнаружении вершины.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void discoverVertex(size_t u, const Graph& graph) {
        std::cout << "BFS: Discovered vertex " << u << '\n';
    }

    /**
     * @brief Вызывается при начале обработки вершины.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void examineVertex(size_t u, const Graph& graph) {
        std::cout << "BFS: Examining vertex " << u << '\n';
    }

    /**
     * @brief Вызывается при обработке ребра.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void examineEdge(size_t u, size_t v, const Graph& graph) {
        std::cout << "BFS: Examining edge (" << u << ", " << v << ")" << '\n';
    }

    /**
     * @brief Вызывается для ребра, которое является частью дерева обхода.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void treeEdge(size_t u, size_t v, const Graph& graph) {
        std::cout << "BFS: Tree edge (" << u << ", " << v << ")" << '\n';
    }

    /**
     * @brief Вызывается для обратного или поперечного ребра.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void nonTreeEdge(size_t u, size_t v, const Graph& graph) {
        std::cout << "BFS: Non-tree edge (" << u << ", " << v << ")" << '\n';
    }

    /**
     * @brief Вызывается при завершении обработки вершины.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void finishVertex(size_t u, const Graph& graph) {
        std::cout << "BFS: Finished vertex " << u << '\n';
    }
};

/**
 * @brief Функция BFS (поиск в ширину) с поддержкой Visitor.
 * @tparam Graph Тип реализации графа.
 * @tparam Visitor Тип Visitor.
 * @param graph Граф, заданный одним из типов.
 * @param start Индекс стартовой вершины.
 * @param visitor Объект с методами-обработчиками.
 */
template <typename Graph, typename Visitor = BFSVisitor>
void BFS(const Graph& graph, size_t start, Visitor visitor = Visitor()) {
    if (!graph.Size()) {
        return;
    }
    // Для каждой вершины храним цвет (White/Gray/Black)
    // White — не посещена, Gray — обнаружена, но ещё не обработаны все соседи,
    // Black — все соседи обработаны (вершина завершена).
    std::vector<Color> color(graph.Size(), Color::White);

    std::queue<size_t> queue;

    // Начинаем со стартовой вершины
    color[start] = Color::Gray;
    visitor.discoverVertex(start, graph);
    queue.push(start);

    while (!queue.empty()) {
        size_t u = queue.front();
        queue.pop();

        // Вызываем ExamineVertex для текущей вершины
        visitor.examineVertex(u, graph);

        // Перебираем всех соседей
        for (auto v : graph.neighbors(u)) {
            // Событие "examineEdge"
            visitor.examineEdge(u, v, graph);

            if (color[v] == Color::White) {
                // Это ребро - дерево обхода
                visitor.treeEdge(u, v, graph);

                color[v] = Color::Gray;
                visitor.discoverVertex(v, graph);
                queue.push(v);
            } else {
                // Значит, это не дерево, а уже посещённая вершина
                visitor.nonTreeEdge(u, v, graph);
            }
        }

        // Завершаем обработку вершины
        color[u] = Color::Black;
        visitor.finishVertex(u, graph);
    }
}
