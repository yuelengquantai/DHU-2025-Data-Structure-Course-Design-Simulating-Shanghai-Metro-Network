#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <filesystem>
#include <string>
#include "menu.h"
#include "station.h"
#include "graph.h"
using namespace std;

vector<Station*> stations;

void initStation() {
    if (!filesystem::exists("./data/Station.csv")) {
        cout << "Station.csv 文件不存在" << endl;
        system("pause");
    }

    ifstream fs("./data/Station.csv", ios::in); // 读取csv文件

    if (!fs.is_open()) {
        cout << "Station.csv 文件打开失败" << endl;
        system("pause");
    }

    string line;
    getline(fs, line); // 跳过表头

    stations.clear();
    while (getline(fs, line)) {
        stringstream ss(line);
        string stat_id, name, line_name, open_status;

        getline(ss, stat_id, ',');
        getline(ss, name, ',');
        getline(ss, line_name, ',');
        getline(ss, open_status, ',');

        bool open = false;
        if (open_status == "开启") open = true;

        Station* s = new Station(line_name, stat_id, name, open);
        stations.push_back(s);
    }

    fs.close();
}

void batchUpdateStationStatusFromCsv() {
    initStation(); // 当然, 我偷懒了

    cout << stations.size() << "个站点更新成功" << endl;
    system("pause");
    system("cls");
    getStationInfo();

}

vector<Station*> revisedStations;

void updateStationStatusManually() {
    cout << "请输入待修改站点关键词(exit 退出): ";
    string name;
    cin >> name;

    if (name == "exit") {
        if (revisedStations.size()) {
            cout << "修改站点: " << endl;
            for (int i = 0; i < revisedStations.size(); ++i) {
                printf("%s(%s)->状态: ", revisedStations[i]->name.c_str(), revisedStations[i]->line.c_str());
                if (revisedStations[i]->open) cout << "开启";
                else cout << "关闭";
                cout << endl;
            }
            cout << endl;
            cout << revisedStations.size() << "个站点的状态修改完成" << endl;
            system("pause");
        }
        revisedStations.clear();
        system("cls");
        getStationInfo();
    }

    vector<Station*> list;
    for (int i = 0; i < stations.size(); ++i) {
        if (stations[i]->name.find(name) != string::npos) list.push_back(stations[i]);
    }

    if (list.size()) {
        cout << "匹配的站点如下: " << endl;
        for (int i = 0; i < list.size(); ++i) {
            printf("%d. %s (%s)\n", i + 1, list[i]->name.c_str(), list[i]->line.c_str());
        }

        cout << "请输入对应编号选择站点: ";
        string op;
        cin >> op;
        bool error = true;
        for (int i = 0; i < list.size(); ++i) {
            if (to_string(i + 1) == op) {
                error = false;
                
                cout << list[i]->name << ", " << list[i]->line << ", ";
                if (list[i]->open) cout << "开启";
                else cout << "关闭";
                cout << endl;

                cout << "请输入站点状态(开启/关闭) >";
                string stat_status;
                cin >> stat_status;
                if (stat_status == "开启") {
                    list[i]->open = true;
                    revisedStations.push_back(list[i]);
                    buildGraph();
                }
                else if (stat_status == "关闭") {
                    list[i]->open = false;
                    revisedStations.push_back(list[i]);
                    buildGraph();
                }
                else {
                    cout << "输入无效, 请输入开启、关闭" << endl;
                    Sleep(1000);
                }
                
                system("cls");
                updateStationStatusManually();

                break;
            }
        }
        if (error) {
            cout << "输入无效, 请输入对应编号选择站点";
            Sleep(1200);
            updateStationStatusManually();
        }
    }
    else {
        cout << "没有找到匹配的站点" << endl;
        Sleep(1200);
        system("cls");
        updateStationStatusManually();
    }
    
}

vector<Station*> closedStations;
void showClosedStations() {
    closedStations.clear();
    for (int i = 0; i < stations.size(); ++i) {
        if (!stations[i]->open) closedStations.push_back(stations[i]);
    }

    if (closedStations.empty()) cout << "所有站点均处于开放状态" << endl;
    else {
        cout << "当前关闭的站点列表: " << endl;
        for (int i = 0; i < closedStations.size(); ++i) {
            printf("关闭站点%d: %s, %s\n", i + 1, stations[i]->name.c_str(), stations[i]->line.c_str());
        }
    }

    system("pause");
    system("cls");
    getStationInfo();
}

void resetAllStationsToInitialState() {
    cout << "您确定要恢复所有站点的初始状态? (Y/N)" << endl;
    string op;
    cin >> op;
    if (op != "Y") {
        if (op == "N") {
            system("cls");
            getStationInfo();
        }
        else {
            cout << "输入无效, 请输入(Y/N)" << endl;
            Sleep(1200);
            system("cls");
            resetAllStationsToInitialState();
        }
    }

    for (int i = 0; i < closedStations.size(); ++i) {
        closedStations[i]->open = true;
    }
    closedStations.clear();

    system("cls");
    cout << "全部站点恢复为\"开启\"状态" << endl;
    buildGraph();
    Sleep(1200);
    system("cls");
    getStationInfo();
}

void showLineStationsInfo() {
    cout << "线路编号列表 (1 - 20)" << endl;
    cout << " 1 - 1号线\t 2 - 2号线" << endl;
    cout << " 3 - 3号线\t 4 - 4号线" << endl;
    cout << " 5 - 5号线\t 6 - 6号线" << endl;
    cout << " 7 - 7号线\t 8 - 8号线" << endl;
    cout << " 9 - 9号线\t10 - 10号线" << endl;
    cout << "11 - 11号线\t12 - 12号线" << endl;
    cout << "13 - 13号线\t14 - 14号线" << endl;
    cout << "15 - 15号线\t16 - 16号线" << endl;
    cout << "17 - 17号线\t18 - 18号线" << endl;
    cout << "19 - 浦江线\t20 - 机场市域线" << endl;

    cout << "请输入要查询的线路编号 (1 - 20) : ";
    string line;
    cin >> line;
    bool match = false;
    for (int i = 1; i <= 20; ++i) {
        if (to_string(i) == line) {
            match = true;

            string query_line = to_string(i) + "号线";
            if (i == 19) query_line = "江浦线";
            else if (i == 20) query_line = "市域机场线";

            vector<Station*> query_stations;
            bool enter = true;
            for (int j = 0; j < stations.size(); ++j) {
                if (stations[j]->line == query_line) {
                    enter = false;
                    query_stations.push_back(stations[j]);
                }
                if (!enter && stations[j]->line != query_line) break;
            }

            cout << i << "号线共有" << query_stations.size() << "个站点" << endl;
            cout << endl;

            for (int j = 0; j < query_stations.size(); ++j) {
                if (j) cout << "|" << endl;
                cout << "○ " << query_stations[j]->name << endl;
            }

            system("pause");
            system("cls");
            getStationInfo();
        }
    }
    if (!match) {
        cout << "输入无效, 请输入数字选项 (1 - 20)";
        Sleep(1000);
        system("cls");
        showLineStationsInfo();
    }
}

void releaseStationMemory() {
    for (int i = 0; i < stations.size(); ++i) delete stations[i];
}