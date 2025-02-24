#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <stdexcept>
#include "graph.hpp"

struct DFSVisitor {
    /**
     * @brief Вызывается при обнаружении вершины.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void discoverVertex(size_t u, const Graph& graph) {
        std::cout << "DFS: Discovered vertex " << u << '\n';
    }

    /**
     * @brief Вызывается при обработке ребра.
     * @tparam Graph Тип графа.
     * @param u Индекс начала.
     * @param v Индекс конца.
     * @param graph Граф.
     */
    template <typename Graph>
    void examineEdge(size_t u, size_t v, const Graph& graph) {
        std::cout << "DFS: Examining edge (" << u << ", " << v << ")" << '\n';
    }

    /**
     * @brief Вызывается для ребра, которое является частью дерева обхода.
     * @tparam Graph Тип графа.
     * @param u Индекс начала.
     * @param v Индекс конца.
     * @param graph Граф.
     */
    template <typename Graph>
    void treeEdge(size_t u, size_t v, const Graph& graph) {
        std::cout << "DFS: Tree edge (" << u << ", " << v << ")" << '\n';
    }

    /**
     * @brief Вызывается для обратного или поперечного ребра.
     * @tparam Graph Тип графа.
     * @param u Индекс начала.
     * @param v Индекс конца.
     * @param graph Граф.
     */
    template <typename Graph>
    void nonTreeEdge(size_t u, size_t v, const Graph& graph) {
        std::cout << "DFS: Non-tree edge (" << u << ", " << v << ")" << '\n';
    }

    /**
     * @brief Вызывается при завершении обработки вершины.
     * @tparam Graph Тип графа.
     * @param u Индекс вершины.
     * @param graph Граф.
     */
    template <typename Graph>
    void finishVertex(size_t u, const Graph& graph) {
        std::cout << "DFS: Finished vertex " << u << '\n';
    }
};

/**
 * @brief Рекурсивная функция DFS (поиск в глубину) с поддержкой Visitor.
 * @tparam Graph Тип реализации графа.
 * @tparam Visitor Тип Visitor.
 * @param graph Граф, заданный одним из типов.
 * @param start Индекс стартовой вершины.
 * @param visitor Объект с методами-обработчиками.
 * @param color Вектор цветов вершин графа.
 */
template <typename Graph, typename Visitor = DFSVisitor>
void DFS_Recursive(const Graph& graph, size_t start, Visitor& visitor,
                   std::vector<Color>& color) {
    color[start] = Color::Gray;
    visitor.discoverVertex(start, graph);

    for (auto v : graph.neighbors(start)) {
        visitor.examineEdge(start, v, graph);

        if (color[v] == Color::White) {
            visitor.treeEdge(start, v, graph);
            DFS_Recursive(graph, v, visitor, color);
        } else {
            visitor.nonTreeEdge(start, v, graph);
        }
    }

    color[start] = Color::Black;
    visitor.finishVertex(start, graph);
}

/**
 * @brief Функция-обертка для DFS (поиск в глубину) для обхода всех вершин.
 * @tparam Graph Тип реализации графа.
 * @tparam Visitor Тип Visitor.
 * @param graph Граф, заданный одним из типов.
 * @param visitor Объект с методами-обработчиками.
 */
template <typename Graph, typename Visitor = DFSVisitor>
void DFS(const Graph& graph, Visitor visitor = Visitor()) {
    std::vector<Color> color(graph.Size(), Color::White);

    for (size_t u = 0; u < graph.Size(); ++u) {
        if (color[u] == Color::White) {
            DFS_Recursive(graph, u, visitor, color);
        }
    }
}