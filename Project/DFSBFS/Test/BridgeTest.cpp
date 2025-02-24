#include <gtest/gtest.h>
#include "BridgeFinder.cpp"
#include "graph.hpp"
#include "AdjMatrix.cpp"
#include "AdjSet.cpp"
#include "EdgesList.cpp"


TEST(AdjacencyMatrixGraphTests, SingleVertexNoEdges) {
    AdjacencyMatrixGraph<int> graph(1);
    BridgesAndArticulationPoints<AdjacencyMatrixGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_TRUE(finder.getBridges().empty());
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(AdjacencyMatrixGraphTests, TwoVerticesOneEdge) {
    AdjacencyMatrixGraph<int> graph(2);
    graph.AddEdge(0, 1);
    BridgesAndArticulationPoints<AdjacencyMatrixGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 1);
    EXPECT_EQ(finder.getArticulationPoints().size(), 0);
}

TEST(AdjacencyMatrixGraphTests, TriangleGraphNoBridges) {
    AdjacencyMatrixGraph<int> graph(3);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);
    BridgesAndArticulationPoints<AdjacencyMatrixGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_TRUE(finder.getBridges().empty());
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(AdjacencyMatrixGraphTests, StarGraphWithArticulation) {
    AdjacencyMatrixGraph<int> graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 2);
    graph.AddEdge(0, 3);
    BridgesAndArticulationPoints<AdjacencyMatrixGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 3);
    EXPECT_EQ(finder.getArticulationPoints().size(), 1);
    EXPECT_TRUE(finder.getArticulationPoints().contains(0));
}

TEST(AdjacencyMatrixGraphTests, DisconnectedGraphNoBridges) {
    AdjacencyMatrixGraph<int> graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);
    BridgesAndArticulationPoints<AdjacencyMatrixGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 2);
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(EdgeListGraphTests, SingleVertexNoEdges) {
    EdgeListGraph<int> graph;
    BridgesAndArticulationPoints<EdgeListGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_TRUE(finder.getBridges().empty());
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(EdgeListGraphTests, SimpleLineGraph) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    BridgesAndArticulationPoints<EdgeListGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 2);
    EXPECT_EQ(finder.getArticulationPoints().size(), 1);
    EXPECT_TRUE(finder.getArticulationPoints().contains(1));
}

TEST(EdgeListGraphTests, CycleGraphNoBridges) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);
    BridgesAndArticulationPoints<EdgeListGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_TRUE(finder.getBridges().empty());
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(EdgeListGraphTests, CompleteGraphNoBridges) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 2);
    graph.AddEdge(1, 2);
    BridgesAndArticulationPoints<EdgeListGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_TRUE(finder.getBridges().empty());
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(EdgeListGraphTests, ComplexGraphWithBridges) {
    EdgeListGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(3, 4);
    BridgesAndArticulationPoints<EdgeListGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 4);
    EXPECT_EQ(finder.getArticulationPoints().size(), 2);
}

TEST(AdjacencySetGraphTests, SingleVertexNoEdges) {
    AdjacencySetGraph<int> graph;
    BridgesAndArticulationPoints<AdjacencySetGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_TRUE(finder.getBridges().empty());
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(AdjacencySetGraphTests, TwoVerticesOneEdge) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    BridgesAndArticulationPoints<AdjacencySetGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 1);
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(AdjacencySetGraphTests, StarGraphWithArticulation) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 2);
    graph.AddEdge(0, 3);
    BridgesAndArticulationPoints<AdjacencySetGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 3);
    EXPECT_EQ(finder.getArticulationPoints().size(), 1);
    EXPECT_TRUE(finder.getArticulationPoints().contains(0));
}

TEST(AdjacencySetGraphTests, DisconnectedGraphNoBridges) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(2, 3);
    BridgesAndArticulationPoints<AdjacencySetGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 2);
    EXPECT_TRUE(finder.getArticulationPoints().empty());
}

TEST(AdjacencySetGraphTests, ComplexGraphWithArticulation) {
    AdjacencySetGraph<int> graph;
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(3, 4);
    graph.AddEdge(2, 4);
    BridgesAndArticulationPoints<AdjacencySetGraph<int>, int> finder(graph);
    finder.findBridgesAndAPs();
    EXPECT_EQ(finder.getBridges().size(), 1);
    EXPECT_EQ(finder.getArticulationPoints().size(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}