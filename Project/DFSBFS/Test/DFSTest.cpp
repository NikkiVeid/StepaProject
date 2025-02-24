#include <gtest/gtest.h>
#include "graph.hpp"
#include "DFS.cpp"
#include "AdjMatrix.cpp"
#include "AdjSet.cpp"
#include "EdgesList.cpp"

struct TestVisitor : DFSVisitor {
    std::vector<size_t> discovered;
    std::vector<size_t> finished;
    std::vector<std::pair<size_t, size_t>> treeEdges;
    std::vector<std::pair<size_t, size_t>> nonTreeEdges;

    template <typename Graph>
    void discoverVertex(size_t u, const Graph& graph) {
        discovered.push_back(u);
    }

    template <typename Graph>
    void finishVertex(size_t u, const Graph& graph) {
        finished.push_back(u);
    }

    template <typename Graph>
    void treeEdge(size_t u, size_t v, const Graph& graph) {
        treeEdges.emplace_back(u, v);
    }

    template <typename Graph>
    void nonTreeEdge(size_t u, size_t v, const Graph& graph) {
        nonTreeEdges.emplace_back(u, v);
    }
};

TEST(AdjacencyMatrixGraph, DFS_DisconnectedGraph) {
    AdjacencyMatrixGraph<int> graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    ASSERT_EQ(visitor.discovered.size(), 4);
}

TEST(AdjacencyMatrixGraph, DFS_ConnectedGraph) {
    AdjacencyMatrixGraph<int> graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.discovered, (std::vector<size_t>{0, 1, 2, 3}));
}

TEST(AdjacencyMatrixGraph, DFS_DirectedGraph) {
    AdjacencyMatrixGraph<int> graph(4, true);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.treeEdges, (std::vector<std::pair<size_t, size_t>>{{0, 1}, {1, 2}}));
}

TEST(AdjacencyMatrixGraph, DFS_TreeEdges) {
    AdjacencyMatrixGraph<int> graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.treeEdges.size(), 3);
}

TEST(AdjacencyMatrixGraph, DFS_NonTreeEdges) {
    AdjacencyMatrixGraph<int> graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.nonTreeEdges.size(), 1);
}

TEST(EdgeListGraph, DFS_DisconnectedGraph) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    ASSERT_EQ(visitor.discovered.size(), 4);
}

TEST(EdgeListGraph, DFS_ConnectedGraph) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.discovered, (std::vector<size_t>{0, 1, 2, 3}));
}

TEST(EdgeListGraph, DFS_DirectedGraph) {
    EdgeListGraph<int> graph(true);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.treeEdges, (std::vector<std::pair<size_t, size_t>>{{0, 1}, {1, 2}}));
}

TEST(EdgeListGraph, DFS_TreeEdges) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.treeEdges.size(), 3);
}

TEST(EdgeListGraph, DFS_NonTreeEdges) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.nonTreeEdges.size(), 1);
}

TEST(AdjacencySetGraph, DFS_DisconnectedGraph) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    ASSERT_EQ(visitor.discovered.size(), 4);
}

TEST(AdjacencySetGraph, DFS_ConnectedGraph) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.discovered, (std::vector<size_t>{0, 1, 2, 3}));
}

TEST(AdjacencySetGraph, DFS_DirectedGraph) {
    AdjacencySetGraph<int> graph(true);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.treeEdges, (std::vector<std::pair<size_t, size_t>>{{0, 1}, {1, 2}}));
}

TEST(AdjacencySetGraph, DFS_TreeEdges) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.treeEdges.size(), 3);
}

TEST(AdjacencySetGraph, DFS_NonTreeEdges) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);

    TestVisitor visitor;
    DFS(graph, visitor);

    EXPECT_EQ(visitor.nonTreeEdges.size(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}