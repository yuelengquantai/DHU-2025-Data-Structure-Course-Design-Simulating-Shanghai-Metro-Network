#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <stack>
#include <algorithm>
#include <Windows.h>
#include "menu.h"
#include "station.h"
#include "graph.h"
#include "pathfinder.h"
using namespace std;

struct CompareEdge {
    bool operator()(const Edge& a, const Edge& b) {
        return a.time > b.time;
    }
};

bool find(const string& name, const vector<string>& list) {
    for (int i = 0; i < list.size(); ++i) {
        if (list[i] == name) return true;
    }
    return false;
}

string listStations() {
    string name;
    cin >> name;
    vector<string> name_matches;
    vector<string> id_matches;
    for (int i = 0; i < stations.size(); ++i) {
        if (stations[i]->name.find(name) != string::npos && !find(stations[i]->name, name_matches)) {
            name_matches.push_back(stations[i]->name);
            id_matches.push_back(stations[i]->stat_id);
        }
    }

    string startID;
    if (name_matches.empty()) {
        cout << "没有找到匹配的站点" << endl;
        Sleep(1200);
        system("cls");
        minTimePath();
    }
    else {
        cout << "匹配的站点如下: " << endl;
        for (int i = 0; i < name_matches.size(); ++i) {
            printf("%d. %s\n", i + 1, name_matches[i].c_str());
        }

        cout << "请输入对应编号选择站点: ";
        string op;
        cin >> op;
        for (int i = 0; i < name_matches.size(); ++i) {
            if (to_string(i + 1) == op) return id_matches[i];
        }

        cout << "输入无效, 请输入对应编号选择站点";
        Sleep(1200);
        system("cls");
        minTimePath();
    }
}

int findLastSameStation(const vector<int>& list) {
    if (list.empty()) return 0;
    string name = stations[list[0]]->name;
    for (int i = 0; i < list.size(); ++i) {
        int idx = list[i];
        if (stations[idx]->name != name) return i - 1;
    }
    return 0;
}

void minTimePath() {
    cout << "请输入起点站关键词: ";
    string startID = listStations();
    int start_idx = index[startID];
    
    cout << "请输入终点站关键词: ";
    string endID = listStations();
    int end_idx = index[endID];

	priority_queue<Edge, vector<Edge>, CompareEdge> pq;
    pq.push(Edge(startID, 0));

    vector<int> dist(stations.size(), INT_MAX);
    dist[start_idx] = 0;
    vector<int> prev(stations.size(), -1);

    while (!pq.empty()) {
        Edge curr = pq.top();
        pq.pop();

        int curr_idx = index[curr.stat_id];

        if (dist[curr_idx] < curr.time) continue;

        for (const Edge& toEdge : graph[curr_idx]) {
            int toEdge_idx = index[toEdge.stat_id];
            int new_time = curr.time + toEdge.time;
            if (new_time < dist[toEdge_idx]) {
                dist[toEdge_idx] = curr.time + toEdge.time;
                prev[toEdge_idx] = curr_idx;
                pq.push(Edge(toEdge.stat_id, new_time));
            }
        }

    }

    if (dist[end_idx] == INT_MAX) {
        cout << "无法到达目标站点" << endl;
        system("pause");
        system("cls");
        getMinTransferPath();
    }

    stack<int> path;
    int curr_idx = index[endID];
    while (curr_idx != -1) {
        path.push(curr_idx);
        curr_idx = prev[curr_idx];
    }

    vector<int> result;
    while (!path.empty()) {
        result.push_back(path.top());
        path.pop();
    }

    int start_i = findLastSameStation(result);
    int transfer_cnt = 0;
    int station_cnt = 0;
    string prev_name = "";

    for (int i = start_i; i < result.size(); ++i) {
        int idx = result[i];
        if (prev_name != stations[idx]->name) ++station_cnt;
        else ++transfer_cnt;
        prev_name = stations[idx]->name;
        if (stations[idx]->name == stations[end_idx]->name) break;
    }
    cout << endl;
    printf("换乘 %d 次, 耗时 %d 分钟, 途径站台数(含起点) %d 个\n", transfer_cnt, dist[end_idx], station_cnt);

    prev_name = "";
    for (int i = start_i; i < result.size(); ++i) {
        int idx = result[i];
        if (prev_name != stations[idx]->name) {
            if (i > start_i) cout << " -> ";
            printf("%s(%s)", stations[idx]->name.c_str(), stations[idx]->line.c_str());
        }
        else {
            printf(" 换乘至 [%s]", stations[idx]->line.c_str());
        }
        prev_name = stations[idx]->name;
        if (stations[idx]->name == stations[end_idx]->name) break;
    }
    cout << endl;

    system("pause");
    system("cls");
    getShortestTimePath();
}

void printPath(const vector<int>& path, int total_time) {
    if (path.empty()) return;

    int transfer_cnt = 0;
    int station_cnt = 1;
    vector<string> output_lines;

    output_lines.push_back(stations[path[0]]->name + "(" + stations[path[0]]->line + ")");
    string prev_name = stations[path[0]]->name;

    for (int i = 1; i < path.size(); ++i) {
        int idx = path[i];
        string current_name = stations[idx]->name;
        if (current_name != prev_name) {
            station_cnt++;
            output_lines.push_back(" -> ");
            output_lines.push_back(stations[idx]->name + "(" + stations[idx]->line + ")");
        }
        else {
            output_lines.push_back(" 换乘至 [" + stations[idx]->line + "]");
            transfer_cnt++;
        }
        prev_name = current_name;
    }

    printf("(换乘 %d 次, 耗时 %d 分钟, 途径站台数(含起点) %d 个)\n", transfer_cnt, total_time, station_cnt);
    cout << "路径: ";
    for (const string& s : output_lines) {
        cout << s;
    }
    cout << endl;
}

void minTimePath3() {
    cout << "请输入起点站关键词: ";
    string startID = listStations();
    int start_idx = index[startID];

    cout << "请输入终点站关键词: ";
    string endID = listStations();
    int end_idx = index[endID];

    vector<vector<int>> dists(stations.size());
    vector<vector<pair<int, int>>> prevInfo(stations.size());
    vector<int> count(stations.size(), 0);

    priority_queue<tuple<int, int, int, int>,
        vector<tuple<int, int, int, int>>,
        greater<tuple<int, int, int, int>>> pq;
    pq.push(make_tuple(0, start_idx, -1, -1));

    while (!pq.empty()) {
        int time = get<0>(pq.top());
        int node_idx = get<1>(pq.top());
        int prev_node = get<2>(pq.top());
        int prev_path_index = get<3>(pq.top());
        pq.pop();

        if (count[node_idx] >= 3) continue;

        dists[node_idx].push_back(time);
        prevInfo[node_idx].push_back(make_pair(prev_node, prev_path_index));
        count[node_idx]++;

        for (const Edge& edge : graph[node_idx]) {
            int neighbor_idx = index[edge.stat_id];
            int new_time = time + edge.time;
            pq.push(make_tuple(new_time, neighbor_idx, node_idx, count[node_idx] - 1));
        }
    }

    if (count[end_idx] == 0) {
        cout << "无法到达目标站点" << endl;
        system("pause");
        system("cls");
        return;
    }

    vector<vector<int>> allPaths;
    for (int i = 0; i < count[end_idx]; i++) {
        vector<int> path;
        stack<int> s;
        int curr_idx = end_idx;
        int path_index = i;

        while (curr_idx != -1) {
            s.push(curr_idx);
            pair<int, int> prev_info = prevInfo[curr_idx][path_index];
            curr_idx = prev_info.first;
            path_index = prev_info.second;
        }

        while (!s.empty()) {
            path.push_back(s.top());
            s.pop();
        }
        allPaths.push_back(path);
    }

    for (int i = 0; i < allPaths.size(); i++) {
        cout << "路径" << i + 1 << ": ";
        printPath(allPaths[i], dists[end_idx][i]);
        if (i < allPaths.size() - 1) cout << endl;
    }

    system("pause");
    system("cls");
    getShortestTimePath();
}

void minTransferPath() {
    cout << "请输入起点站关键词: ";
    string startID = listStations();
    int start_idx = index[startID];

    cout << "请输入终点站关键词: ";
    string endID = listStations();
    int end_idx = index[endID];

    priority_queue<tuple<int, int, string>,
        vector<tuple<int, int, string>>,
        greater<tuple<int, int, string>>> pq;
    pq.push(make_tuple(0, 0, startID));

    vector<int> minTrans(stations.size(), INT_MAX);
    vector<int> minTime(stations.size(), INT_MAX);
    minTrans[start_idx] = 0;
    minTime[start_idx] = 0;

    vector<int> prev(stations.size(), -1);
    vector<string> prevLine(stations.size(), "");

    while (!pq.empty()) {
        auto [trans, time, currID] = pq.top();
        pq.pop();

        int curr_idx = index[currID];

        if (trans > minTrans[curr_idx] ||
            (trans == minTrans[curr_idx] && time > minTime[curr_idx])) {
            continue;
        }

        for (const Edge& toEdge : graph[curr_idx]) {
            int to_idx = index[toEdge.stat_id];
            int new_trans = trans;
            int new_time = time + toEdge.time;

            if (stations[curr_idx]->name == stations[to_idx]->name) {
                if (stations[curr_idx]->line != stations[to_idx]->line) {
                    new_trans += 1;
                }
            }

            if (new_trans < minTrans[to_idx] ||
                (new_trans == minTrans[to_idx] && new_time < minTime[to_idx])) {
                minTrans[to_idx] = new_trans;
                minTime[to_idx] = new_time;
                prev[to_idx] = curr_idx;
                prevLine[to_idx] = stations[curr_idx]->line;
                pq.push(make_tuple(new_trans, new_time, toEdge.stat_id));
            }
        }
    }

    if (minTrans[end_idx] == INT_MAX) {
        cout << "无法到达目标站点" << endl;
        system("pause");
        system("cls");
        return;
    }

    stack<int> path;
    int curr_idx = end_idx;
    while (curr_idx != -1) {
        path.push(curr_idx);
        curr_idx = prev[curr_idx];
    }

    vector<int> result;
    while (!path.empty()) {
        result.push_back(path.top());
        path.pop();
    }

    string prev_name = "";
    int station_cnt = 0;
    for (int idx : result) {
        if (prev_name != stations[idx]->name) {
            station_cnt++;
            prev_name = stations[idx]->name;
        }
    }

    cout << endl;
    printf("换乘 %d 次, 耗时 %d 分钟, 途径站台数(含起点) %d 个\n",
        minTrans[end_idx], minTime[end_idx], station_cnt);

    prev_name = "";
    string prev_line = "";
    for (int i = 0; i < result.size(); ++i) {
        int idx = result[i];

        if (prev_name != stations[idx]->name) {
            if (i > 0) cout << " -> ";
            printf("%s(%s)", stations[idx]->name.c_str(), stations[idx]->line.c_str());
        }
        else if (prev_line != stations[idx]->line) {
            printf(" 换乘至 [%s]", stations[idx]->line.c_str());
        }

        prev_name = stations[idx]->name;
        prev_line = stations[idx]->line;
    }
    cout << endl;

    system("pause");
    system("cls");
    getMinTransferPath();
}

void minTransferPath3() {
    cout << "请输入起点站关键词: ";
    string startID = listStations();
    int start_idx = index[startID];

    cout << "请输入终点站关键词: ";
    string endID = listStations();
    int end_idx = index[endID];

    vector<vector<pair<int, int>>> transRecords(stations.size());
    vector<vector<pair<int, int>>> prevRecords(stations.size());
    vector<int> pathCount(stations.size(), 0);

    priority_queue<tuple<int, int, string, int, int>,
        vector<tuple<int, int, string, int, int>>,
        greater<tuple<int, int, string, int, int>>> pq;
    pq.push(make_tuple(0, 0, startID, -1, -1));

    while (!pq.empty()) {
        auto [trans, time, currID, prev_node, path_index] = pq.top();
        pq.pop();

        int curr_idx = index[currID];

        bool keep = false;
        if (pathCount[curr_idx] < 3) {
            keep = true;
        }
        else {
            for (auto& record : transRecords[curr_idx]) {
                if (trans < record.first ||
                    (trans == record.first && time < record.second)) {
                    keep = true;
                    break;
                }
            }
        }

        if (!keep) continue;

        if (pathCount[curr_idx] < 3) {
            transRecords[curr_idx].push_back({ trans, time });
            prevRecords[curr_idx].push_back({ prev_node, path_index });
            pathCount[curr_idx]++;
        }
        else {
            int worst_index = 0;
            for (int i = 1; i < 3; i++) {
                auto& worst = transRecords[curr_idx][worst_index];
                auto& current = transRecords[curr_idx][i];

                if (current.first > worst.first ||
                    (current.first == worst.first && current.second > worst.second)) {
                    worst_index = i;
                }
            }

            transRecords[curr_idx][worst_index] = { trans, time };
            prevRecords[curr_idx][worst_index] = { prev_node, path_index };
        }

        for (const Edge& toEdge : graph[curr_idx]) {
            int to_idx = index[toEdge.stat_id];
            int new_trans = trans;
            int new_time = time + toEdge.time;

            if (stations[curr_idx]->name == stations[to_idx]->name) {
                if (stations[curr_idx]->line != stations[to_idx]->line) {
                    new_trans += 1;
                }
            }

            pq.push(make_tuple(
                new_trans,
                new_time,
                toEdge.stat_id,
                curr_idx,
                pathCount[curr_idx] - 1
            ));
        }
    }

    if (transRecords[end_idx].empty()) {
        cout << "无法到达目标站点" << endl;
        system("pause");
        system("cls");
        return;
    }

    vector<tuple<int, int, vector<int>>> allPaths;

    for (int i = 0; i < pathCount[end_idx]; i++) {
        vector<int> path;
        stack<int> s;
        int curr_idx = end_idx;
        int path_index = i;

        while (curr_idx != -1) {
            s.push(curr_idx);
            auto [prev_node, prev_index] = prevRecords[curr_idx][path_index];
            curr_idx = prev_node;
            path_index = prev_index;
        }

        while (!s.empty()) {
            path.push_back(s.top());
            s.pop();
        }

        allPaths.push_back(make_tuple(
            transRecords[end_idx][i].first,
            transRecords[end_idx][i].second,
            path
        ));
    }

    sort(allPaths.begin(), allPaths.end(), [](const auto& a, const auto& b) {
        if (get<0>(a) != get<0>(b)) return get<0>(a) < get<0>(b);
        return get<1>(a) < get<1>(b);
        });

    int count = min(3, (int)allPaths.size());
    for (int i = 0; i < count; i++) {
        auto& path = get<2>(allPaths[i]);
        int trans = get<0>(allPaths[i]);
        int time = get<1>(allPaths[i]);

        string prev_name = "";
        int station_cnt = 0;
        for (int idx : path) {
            if (prev_name != stations[idx]->name) {
                station_cnt++;
                prev_name = stations[idx]->name;
            }
        }

        cout << "路径 " << i + 1 << ": ";
        printf("(换乘 %d 次, 耗时 %d 分钟, 途径站台数(含起点) %d 个)\n", trans, time, station_cnt);

        string current_name = "";
        string current_line = "";
        for (int j = 0; j < path.size(); j++) {
            int idx = path[j];

            if (current_name != stations[idx]->name) {
                if (j > 0) cout << " -> ";
                printf("%s(%s)", stations[idx]->name.c_str(), stations[idx]->line.c_str());
                current_name = stations[idx]->name;
                current_line = stations[idx]->line;
            }
            else if (current_line != stations[idx]->line) {
                printf(" 换乘至 [%s]", stations[idx]->line.c_str());
                current_line = stations[idx]->line;
            }
        }
        cout << endl << endl;
    }

    system("pause");
    system("cls");
    getMinTransferPath();
}
