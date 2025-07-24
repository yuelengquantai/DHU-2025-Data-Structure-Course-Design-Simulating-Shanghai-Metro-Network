#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "station.h"
#include "graph.h"

std::vector<std::vector<Edge>> graph;
std::unordered_map<std::string, int> index;

void buildIndex() {
    for (int i = 0; i < stations.size(); ++i) {
        std::string stat_id = stations[i]->stat_id;
        index[stat_id] = i;
    }
}

void buildGraph() {
    graph.clear();
    graph.resize(stations.size());

    for (int i = 0; i < graph.size(); ++i) graph[i].clear();

    std::ifstream test_file("./data/Edge.csv");
    if (!test_file.good()) {
        std::cout << "Edge.csv 文件不存在" << std::endl;
        system("pause");
    }

    std::ifstream fs("data/Edge.csv", std::ios::in);

    if (!fs.is_open()) {
        std::cout << "Edge.csv 文件打开失败" << std::endl;
        system("pause");
    }

    std::string line;
    std::getline(fs, line);

    while (std::getline(fs, line)) {
        std::stringstream ss(line);
        std::string startID, endID, line_name, time;

        std::getline(ss, startID, ',');
        std::getline(ss, endID, ',');
        std::getline(ss, line_name, ',');
        std::getline(ss, time, ',');

        int idx_1 = index[startID], idx_2 = index[endID];
        if (stations[idx_1]->open && stations[idx_2]->open) graph[idx_1].push_back(Edge(endID, std::stoi(time)));

    }

}