#include <gtest/gtest.h>

#include "graph.hpp"
#include "AdjSet.cpp"

using WeightType = int;
using GraphType = AdjacencySetGraph<WeightType>;

// Тест 1: Инициализация графа
TEST(AdjacencySetGraphTest, Initialization) {
    GraphType graph;
    EXPECT_EQ(graph.Size(), 0);
}

// Тест 2: Добавление вершины
TEST(AdjacencySetGraphTest, AddVertex) {
    GraphType graph;
    graph.AddEdge(0, 1, 5);
    EXPECT_EQ(graph.Size(), 2);
}

// Тест 3: Добавление ребра в неориентированный граф
TEST(AdjacencySetGraphTest, AddEdgeUndirected) {
    GraphType graph;
    graph.AddEdge(0, 1, 5);
    EXPECT_TRUE(graph.GetEdgeWeight(0, 1).has_value());
    EXPECT_TRUE(graph.GetEdgeWeight(1, 0).has_value());
}

// Тест 4: Добавление ребра в ориентированный граф
TEST(AdjacencySetGraphTest, AddEdgeDirected) {
    GraphType graph(true);
    graph.AddEdge(0, 1, 5);
    EXPECT_TRUE(graph.GetEdgeWeight(0, 1).has_value());
    EXPECT_FALSE(graph.GetEdgeWeight(1, 0).has_value());
}

// Тест 5: Удаление существующего ребра
TEST(AdjacencySetGraphTest, RemoveEdge) {
    GraphType graph;
    graph.AddEdge(0, 1, 5);
    graph.RemoveEdge(0, 1);
    EXPECT_FALSE(graph.GetEdgeWeight(0, 1).has_value());
}

// Тест 6: Удаление несуществующего ребра
TEST(AdjacencySetGraphTest, RemoveNonExistentEdge) {
    GraphType graph;
    graph.RemoveEdge(0, 1);
    EXPECT_FALSE(graph.GetEdgeWeight(0, 1).has_value());
}

// Тест 7: Получение веса существующего ребра
TEST(AdjacencySetGraphTest, GetEdgeWeight) {
    GraphType graph;
    graph.AddEdge(0, 1, 10);
    EXPECT_EQ(graph.GetEdgeWeight(0, 1).value(), 10);
}

// Тест 8: Получение веса несуществующего ребра
TEST(AdjacencySetGraphTest, GetEdgeWeightNonExistent) {
    GraphType graph;
    EXPECT_FALSE(graph.GetEdgeWeight(0, 1).has_value());
}

// Тест 9: Итерация по соседям
TEST(AdjacencySetGraphTest, NeighborIterator) {
    GraphType graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 2);

    auto neighbors = graph.neighbors(0);
    std::unordered_set<size_t> expected_neighbors = {1, 2};

    for (auto neighbor : neighbors) {
        EXPECT_TRUE(expected_neighbors.count(neighbor));
    }
}

// Тест 10: Фильтрованный итератор (по весу)
TEST(AdjacencySetGraphTest, FilteredNeighborIterator) {
    GraphType graph;
    graph.AddEdge(0, 1, 5);
    graph.AddEdge(0, 2, 10);
    auto filtered = graph.neighborsFiltered(0, [](size_t, int weight) {
        return weight > 5;
    });

    std::unordered_set<size_t> expected_neighbors = {2};

    for (auto neighbor : filtered) {
        EXPECT_TRUE(expected_neighbors.count(neighbor));
    }
}

// Тест 11: Проверка итератора на пустом графе
// TEST(AdjacencySetGraphTest, EmptyNeighborIterator) {
//     GraphType graph;
//     auto neighbors = graph.neighbors(0);
//     EXPECT_EQ(neighbors.begin(), neighbors.end());
// }

// Тест 12: Проверка направленности графа
// TEST(AdjacencySetGraphTest, IsDirected) {
//     GraphType directedGraph(true);
//     GraphType undirectedGraph(false);

//     EXPECT_TRUE(directedGraph.IsDirected());
//     EXPECT_FALSE(undirectedGraph.IsDirected());
// }

// Тест 13: Проверка размера графа после добавления и удаления рёбер
TEST(AdjacencySetGraphTest, SizeAfterAddRemove) {
    GraphType graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);
    EXPECT_EQ(graph.Size(), 4);
    graph.RemoveEdge(0, 1);
    EXPECT_EQ(graph.Size(), 4);  // Вершины остаются, даже если ребро удалено
}

// Тест 14: Проверка итератора по фильтрованным соседям с предикатом, который никого не пропускает
TEST(AdjacencySetGraphTest, FilteredNeighborIteratorNoMatch) {
    GraphType graph;
    graph.AddEdge(0, 1, 5);
    graph.AddEdge(0, 2, 10);
    auto filtered = graph.neighborsFiltered(0, [](size_t, int weight) {
        return weight > 20;
    });
    EXPECT_EQ(filtered.begin(), filtered.end());
}

// Тест 15: Проверка некорректного индекса вершины
TEST(AdjacencySetGraphTest, InvalidVertexIndex) {
    GraphType graph;
    graph.AddEdge(0, 1);
    EXPECT_THROW(graph.neighbors(2), std::out_of_range);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}