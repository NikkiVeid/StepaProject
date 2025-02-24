#include <gtest/gtest.h>
#include "BFS.cpp"
#include "graph.hpp"
#include "AdjMatrix.cpp"
#include "AdjSet.cpp"
#include "EdgesList.cpp"

// Тестовый Visitor для проверки порядка вызовов
struct TestBFSVisitor : public BFSVisitor {
    std::vector<size_t> discoverOrder;
    std::vector<size_t> finishOrder;

    template <typename Graph>
    void discoverVertex(size_t u, const Graph& graph) {
        discoverOrder.push_back(u);
    }

    template <typename Graph>
    void finishVertex(size_t u, const Graph& graph) {
        finishOrder.push_back(u);
    }
};

// --- Тесты для AdjacencyMatrixGraph ---
TEST(BFS_AdjacencyMatrixGraph, EmptyGraph) {
    AdjacencyMatrixGraph<int> graph(0);
    TestBFSVisitor visitor;
    EXPECT_NO_THROW(BFS(graph, 0, visitor));
}

TEST(BFS_AdjacencyMatrixGraph, SingleNodeGraph) {
    AdjacencyMatrixGraph<int> graph(1);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.finishOrder, std::vector<size_t>({0}));
}

TEST(BFS_AdjacencyMatrixGraph, ConnectedGraph) {
    AdjacencyMatrixGraph<int> graph(3);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.finishOrder, std::vector<size_t>({0, 1, 2}));
}

TEST(BFS_AdjacencyMatrixGraph, DisconnectedGraph) {
    AdjacencyMatrixGraph<int> graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.finishOrder, std::vector<size_t>({0, 1}));
}

TEST(BFS_AdjacencyMatrixGraph, CycleGraph) {
    AdjacencyMatrixGraph<int> graph(3);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0, 1, 2}));
}

// --- Тесты для EdgeListGraph ---
TEST(BFS_EdgeListGraph, EmptyGraph) {
    EdgeListGraph<int> graph;
    TestBFSVisitor visitor;
    EXPECT_NO_THROW(BFS(graph, 0, visitor));
}

TEST(BFS_EdgeListGraph, SingleNodeGraph) {
    EdgeListGraph<int> graph;
    TestBFSVisitor visitor;
    graph.AddEdge(0, 0);
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0}));
}

TEST(BFS_EdgeListGraph, ConnectedGraph) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0, 1, 2}));
}

TEST(BFS_EdgeListGraph, DisconnectedGraph) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0, 1}));
}

TEST(BFS_EdgeListGraph, CycleGraph) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0, 1, 2}));
}

// --- Тесты для AdjacencySetGraph ---
TEST(BFS_AdjacencySetGraph, EmptyGraph) {
    AdjacencySetGraph<int> graph;
    TestBFSVisitor visitor;
    EXPECT_NO_THROW(BFS(graph, 0, visitor));
}

TEST(BFS_AdjacencySetGraph, SingleNodeGraph) {
    AdjacencySetGraph<int> graph;
    TestBFSVisitor visitor;
    graph.AddEdge(0, 0);
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0}));
}

TEST(BFS_AdjacencySetGraph, ConnectedGraph) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0, 1, 2}));
}

TEST(BFS_AdjacencySetGraph, DisconnectedGraph) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0, 1}));
}

TEST(BFS_AdjacencySetGraph, CycleGraph) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);
    TestBFSVisitor visitor;
    BFS(graph, 0, visitor);
    EXPECT_EQ(visitor.discoverOrder, std::vector<size_t>({0, 1, 2}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
