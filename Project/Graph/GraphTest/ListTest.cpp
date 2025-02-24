#include <gtest/gtest.h>

#include "graph.hpp"
#include "EdgesList.cpp"

// Тестирование конструктора
TEST(EdgeListGraphTest, Constructor) {
    EdgeListGraph<int> g1;
    EdgeListGraph<double> g2(true);

    EXPECT_EQ(g1.Size(), 0);
    EXPECT_EQ(g2.Size(), 0);
}

// Тестирование добавления рёбер
TEST(EdgeListGraphTest, AddEdge) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);
    g.AddEdge(1, 2, 20);

    EXPECT_EQ(g.Size(), 3);
    EXPECT_EQ(g.GetEdgeWeight(0, 1), 10);
    EXPECT_EQ(g.GetEdgeWeight(1, 2), 20);
}

// Тестирование добавления рёбер в неориентированном графе
TEST(EdgeListGraphTest, AddEdgeUndirected) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);

    EXPECT_EQ(g.GetEdgeWeight(0, 1), 10);
    EXPECT_EQ(g.GetEdgeWeight(1, 0), 10);
}

// Тестирование добавления рёбер в ориентированном графе
TEST(EdgeListGraphTest, AddEdgeDirected) {
    EdgeListGraph<int> g(true);
    g.AddEdge(0, 1, 10);

    EXPECT_EQ(g.GetEdgeWeight(0, 1), 10);
    EXPECT_EQ(g.GetEdgeWeight(1, 0).has_value(), false);
}

// Тестирование удаления рёбер
TEST(EdgeListGraphTest, RemoveEdge) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);
    g.RemoveEdge(0, 1);

    EXPECT_EQ(g.GetEdgeWeight(0, 1).has_value(), false);
}

// Тестирование итерации по соседям
TEST(EdgeListGraphTest, NeighborIteration) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);
    g.AddEdge(0, 2, 20);
    g.AddEdge(2, 1, 30);

    std::unordered_set<int> neighbors;
    for (auto neighbor : g.neighbors(1)) {
        neighbors.insert(neighbor);
    }

    EXPECT_EQ(neighbors.size(), 2);
    EXPECT_TRUE(neighbors.count(0));
    EXPECT_TRUE(neighbors.count(2));
}

// Тестирование пустого диапазона соседей
TEST(EdgeListGraphTest, EmptyNeighborIteration) {
    EdgeListGraph<int> g;
    g.AddEdge(1, 2, 10);

    auto neighbors = g.neighbors(0);
    EXPECT_EQ(neighbors.begin(), neighbors.end());
}

// Тестирование итерации по фильтрованным соседям
TEST(EdgeListGraphTest, FilteredNeighborIteration) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 5);
    g.AddEdge(0, 2, 15);
    g.AddEdge(0, 3, 25);

    auto range = g.neighborsFiltered(0, [](int, int weight) { return weight > 10; });
    std::vector<int> filtered;
    for (auto neighbor : range) {
        filtered.push_back(neighbor);
    }

    EXPECT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0], 2);
    EXPECT_EQ(filtered[1], 3);
}

// Тестирование операторов сравнения итераторов
TEST(EdgeListGraphTest, IteratorEquality) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);
    auto it1 = g.neighbors(0).begin();
    auto it2 = g.neighbors(0).begin();
    auto end = g.neighbors(0).end();

    EXPECT_EQ(it1, it2);
    EXPECT_NE(it1, end);
}

// Тестирование постфиксного инкремента итератора
TEST(EdgeListGraphTest, PostfixIncrement) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);
    g.AddEdge(0, 2, 20);

    auto it = g.neighbors(0).begin();
    auto prev = it++;
    EXPECT_EQ(*prev, 1);
    EXPECT_EQ(*it, 2);
}

// Тестирование работы с весами рёбер
TEST(EdgeListGraphTest, GetEdgeWeight) {
    EdgeListGraph<double> g;
    g.AddEdge(0, 1, 1.5);
    g.AddEdge(1, 2, 2.5);

    EXPECT_EQ(g.GetEdgeWeight(0, 1), 1.5);
    EXPECT_EQ(g.GetEdgeWeight(1, 2), 2.5);
    EXPECT_FALSE(g.GetEdgeWeight(0, 2).has_value());
}

// Тестирование работы с разными типами весов
TEST(EdgeListGraphTest, TemplateTypeWeights) {
    EdgeListGraph<std::string> g;
    g.AddEdge(0, 1, "A");
    g.AddEdge(1, 2, "B");

    EXPECT_EQ(g.GetEdgeWeight(0, 1), "A");
    EXPECT_EQ(g.GetEdgeWeight(1, 2), "B");
}

// Тестирование работы с пустым графом
TEST(EdgeListGraphTest, EmptyGraph) {
    EdgeListGraph<int> g;
    EXPECT_EQ(g.Size(), 0);
    EXPECT_FALSE(g.GetEdgeWeight(0, 1).has_value());
}

// Тестирование размера графа
TEST(EdgeListGraphTest, GraphSize) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);
    g.AddEdge(1, 2, 20);

    EXPECT_EQ(g.Size(), 3);
}

// Тестирование корректности работы при удалении несуществующего ребра
TEST(EdgeListGraphTest, RemoveNonExistentEdge) {
    EdgeListGraph<int> g;
    g.AddEdge(0, 1, 10);
    g.RemoveEdge(1, 2);  // Удаляем несуществующее ребро

    EXPECT_EQ(g.Size(), 2);
    EXPECT_EQ(g.GetEdgeWeight(0, 1), 10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}