//
// Created by take_ on 2023/2/14.
//

#ifndef GCP_HEAD_2_GRAPHCOLORING_H
#define GCP_HEAD_2_GRAPHCOLORING_H

#include <array>
#include <vector>
#include <functional>


namespace szx {

    using NodeId = int;
    using EdgeId = NodeId;
    using ColorId = NodeId;

    using Edge = std::array<NodeId, 2>; // undirected link.

    struct GraphColoring {
        NodeId nodeNum;
        EdgeId edgeNum;
        ColorId colorNum;
        std::vector<Edge> edges;
    };

    using NodeColors = std::vector<ColorId>; // `NodeColors[n]` is the color of node `n`.


    void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed);

}

#endif //GCP_HEAD_2_GRAPHCOLORING_H
