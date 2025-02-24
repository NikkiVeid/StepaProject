#include <gtest/gtest.h>

#include "graph.hpp"
#include "AdjMatrix.cpp"

// Используем AdjacencyMatrixAdjacencyMatrixGraph<int> с весами типа int

// Тесты для базовой функциональности графа
TEST(AdjacencyMatrixGraphTest, Constructor) {
    AdjacencyMatrixGraph<int> g(5);
    EXPECT_EQ(g.Size(), 5);
}

TEST(AdjacencyMatrixGraphTest, AddEdgeUndirected) {
    AdjacencyMatrixGraph<int> g(3);
    g.AddEdge(0, 1, 10);
    EXPECT_EQ(g.GetEdgeWeight(0, 1), 10);
    EXPECT_EQ(g.GetEdgeWeight(1, 0), 10); // Проверка на неориентированный граф
}

TEST(AdjacencyMatrixGraphTest, AddEdgeDirected) {
    AdjacencyMatrixGraph<int> g(3, true); // Ориентированный граф
    g.AddEdge(0, 1, 20);
    EXPECT_EQ(g.GetEdgeWeight(0, 1), 20);
    EXPECT_EQ(g.GetEdgeWeight(1, 0), std::nullopt); // Проверка направления
}

TEST(AdjacencyMatrixGraphTest, RemoveEdgeUndirected) {
    AdjacencyMatrixGraph<int> g(3);
    g.AddEdge(0, 1, 15);
    g.RemoveEdge(0, 1);
    EXPECT_EQ(g.GetEdgeWeight(0, 1), std::nullopt);
    EXPECT_EQ(g.GetEdgeWeight(1, 0), std::nullopt);
}

TEST(AdjacencyMatrixGraphTest, RemoveEdgeDirected) {
    AdjacencyMatrixGraph<int> g(3, true);
    g.AddEdge(0, 1, 25);
    g.RemoveEdge(0, 1);
    EXPECT_EQ(g.GetEdgeWeight(0, 1), std::nullopt);
    EXPECT_EQ(g.GetEdgeWeight(1, 0), std::nullopt); // Направление не меняется
}

TEST(AdjacencyMatrixGraphTest, SelfLoop) {
    AdjacencyMatrixGraph<int> g(3);
    g.AddEdge(1, 1, 5);
    EXPECT_EQ(g.GetEdgeWeight(1, 1), 5);
}

TEST(AdjacencyMatrixGraphTest, NoEdge) {
    AdjacencyMatrixGraph<int> g(4);
    EXPECT_EQ(g.GetEdgeWeight(0, 1), std::nullopt);
}

TEST(AdjacencyMatrixGraphTest, Size) {
    AdjacencyMatrixGraph<int> g(6);
    EXPECT_EQ(g.Size(), 6);
}

// Тесты для итератора по соседям
TEST(AdjacencyMatrixGraphTest, NeighborIteratorEmpty) {
    AdjacencyMatrixGraph<int> g(3);
    auto neighbors = g.neighbors(0);
    auto it = neighbors.begin();
    EXPECT_EQ(it, neighbors.end());
}

TEST(AdjacencyMatrixGraphTest, NeighborIteratorSingleEdge) {
    AdjacencyMatrixGraph<int> g(3);
    g.AddEdge(0, 1, 10);
    auto neighbors = g.neighbors(0);
    auto it = neighbors.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(it, neighbors.end());
}

TEST(AdjacencyMatrixGraphTest, NeighborIteratorMultipleEdges) {
    AdjacencyMatrixGraph<int> g(4);
    g.AddEdge(0, 1, 10);
    g.AddEdge(0, 2, 20);
    g.AddEdge(0, 3, 30);
    auto neighbors = g.neighbors(0);
    auto it = neighbors.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, neighbors.end());
}

TEST(AdjacencyMatrixGraphTest, NeighborIteratorNoNeighbors) {
    AdjacencyMatrixGraph<int> g(3);
    g.AddEdge(1, 2, 15);
    auto neighbors = g.neighbors(0);
    auto it = neighbors.begin();
    EXPECT_EQ(it, neighbors.end());
}

TEST(AdjacencyMatrixGraphTest, FilteredNeighborIterator) {
    AdjacencyMatrixGraph<int> g(4);
    g.AddEdge(0, 1, 10);
    g.AddEdge(0, 2, 20);
    g.AddEdge(0, 3, 30);

    auto neighbors = g.neighborsFiltered(0, [](int, int weight) {
        return weight > 15;
    });
    auto it = neighbors.begin();
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, neighbors.end());
}

TEST(AdjacencyMatrixGraphTest, FilteredNeighborIteratorNoMatch) {
    AdjacencyMatrixGraph<int> g(3);
    g.AddEdge(0, 1, 10);
    auto neighbors = g.neighborsFiltered(0, [](int, int weight) {
        return weight > 50;
    });
    auto it = neighbors.begin();
    EXPECT_EQ(it, neighbors.end());
}

TEST(AdjacencyMatrixGraphTest, MultipleEdgesDirected) {
    AdjacencyMatrixGraph<int> g(4, true);
    g.AddEdge(0, 1, 10);
    g.AddEdge(0, 2, 20);
    g.AddEdge(1, 2, 30);
    g.AddEdge(2, 3, 40);

    EXPECT_EQ(g.GetEdgeWeight(0, 1), 10);
    EXPECT_EQ(g.GetEdgeWeight(0, 2), 20);
    EXPECT_EQ(g.GetEdgeWeight(1, 2), 30);
    EXPECT_EQ(g.GetEdgeWeight(2, 3), 40);
    EXPECT_EQ(g.GetEdgeWeight(3, 2), std::nullopt);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}