#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct Edge {
    std::string stat_id;
    int time;

    Edge(std::string stat_id, int time) : stat_id(stat_id), time(time) {}
};

extern std::vector<std::vector<Edge>> graph;
extern std::unordered_map<std::string, int> index;

void buildIndex();
void buildGraph();
