#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "station.h"
#include "graph.h"
using namespace std;

vector<vector<Edge>> graph;
unordered_map<string, int> index;

void bulidIndex() {
    for (int i = 0; i < stations.size(); ++i) {
        string stat_id = stations[i]->stat_id;
        index[stat_id] = i;
    }
}

void buildGraph() {
    graph.clear();
    graph.resize(stations.size());

    for (int i = 0; i < graph.size(); ++i) graph[i].clear();

    if (!filesystem::exists("./data/Edge.csv")) {
        cout << "Edge.csv 文件不存在" << endl;
        system("pause");
    }

    ifstream fs("data/Edge.csv", ios::in);

    if (!fs.is_open()) {
        cout << "Edge.csv 文件打开失败" << endl;
        system("pause");
    }

    string line;
    getline(fs, line);

    while (getline(fs, line)) {
        stringstream ss(line);
        string startID, endID, line, time;

        getline(ss, startID, ',');
        getline(ss, endID, ',');
        getline(ss, line, ',');
        getline(ss, time, ',');

        int idx_1 = index[startID], idx_2 = index[endID];
        if (stations[idx_1]->open && stations[idx_2]->open) graph[idx_1].push_back(Edge(endID, stoi(time)));

    }

}