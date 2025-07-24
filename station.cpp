#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <string>
#include "station.h"
#include "graph.h"

std::vector<Station*> stations;

void initStation() {
    std::ifstream test_fs("./data/Station.csv");
    if (!test_fs.good()) {
        std::cout << "Station.csv 文件不存在" << std::endl;
        system("pause");
    }

    std::ifstream fs("./data/Station.csv", std::ios::in); // 读取csv文件

    if (!fs.is_open()) {
        std::cout << "Station.csv 文件打开失败" << std::endl;
        system("pause");
    }

    std::string line;
    std::getline(fs, line); // 跳过表头

    stations.clear();
    while (std::getline(fs, line)) {
        std::stringstream ss(line);
        std::string stat_id, name, line_name, open_status;

        std::getline(ss, stat_id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, line_name, ',');
        std::getline(ss, open_status, ',');

        bool open = false;
        if (open_status == "开启") open = true;

        Station* s = new Station(line_name, stat_id, name, open);
        stations.push_back(s);
    }

    fs.close();
}

void batchUpdateStationStatusFromCsv() {
    system("cls");
    initStation(); // 当然, 我偷懒了

    std::cout << stations.size() << "个站点更新成功" << std::endl;
    system("pause");
    return;

}

std::vector<Station*> revisedStations;

void updateStationStatusManually() {
    while (true) {
        system("cls");
        std::cout << "请输入待修改站点关键词(exit 退出): ";
        std::string name;
        std::cin >> name;

        if (name == "exit") {
            if (revisedStations.size()) {
                std::cout << "修改站点: " << std::endl;
                for (int i = 0; i < revisedStations.size(); ++i) {
                    printf("%s(%s)->状态: ", revisedStations[i]->name.c_str(), revisedStations[i]->line.c_str());
                    if (revisedStations[i]->open) std::cout << "开启";
                    else std::cout << "关闭";
                    std::cout << std::endl;
                }
                std::cout << std::endl;
                std::cout << revisedStations.size() << "个站点的状态修改完成" << std::endl;
                system("pause");
            }
            revisedStations.clear();
            return;
        }

        std::vector<Station*> list;
        for (int i = 0; i < stations.size(); ++i) {
            if (stations[i]->name.find(name) != std::string::npos) list.push_back(stations[i]);
        }

        if (list.size()) {
            std::cout << "匹配的站点如下: " << std::endl;
            for (int i = 0; i < list.size(); ++i) {
                printf("%d. %s (%s)\n", i + 1, list[i]->name.c_str(), list[i]->line.c_str());
            }

            std::cout << "请输入对应编号选择站点: ";
            std::string op;
            std::cin >> op;
            bool error = true;
            for (int i = 0; i < list.size(); ++i) {
                if (std::to_string(i + 1) == op) {
                    error = false;
                    
                    std::cout << list[i]->name << ", " << list[i]->line << ", ";
                    if (list[i]->open) std::cout << "开启";
                    else std::cout << "关闭";
                    std::cout << std::endl;

                    std::cout << "请输入站点状态(开启/关闭) >";
                    std::string stat_status;
                    std::cin >> stat_status;
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
                        std::cout << "输入无效, 请输入开启、关闭" << std::endl;
                        Sleep(1000);
                    }
                    continue;
                }
            }
            if (error) {
                std::cout << "输入无效, 请输入对应编号选择站点";
                Sleep(1200);
                continue;
            }
        }
        else {
            std::cout << "没有找到匹配的站点" << std::endl;
            Sleep(1200);
            continue;
        }
    }
}

std::vector<Station*> closedStations;
void showClosedStations() {
    system("cls");
    closedStations.clear();
    for (int i = 0; i < stations.size(); ++i) {
        if (!stations[i]->open) closedStations.push_back(stations[i]);
    }

    if (closedStations.empty()) std::cout << "所有站点均处于开放状态" << std::endl;
    else {
        std::cout << "当前关闭的站点列表: " << std::endl;
        for (int i = 0; i < closedStations.size(); ++i) {
            printf("关闭站点%d: %s, %s\n", i + 1, closedStations[i]->name.c_str(), closedStations[i]->line.c_str());
        }
    }

    system("pause");
    return;
}

void resetAllStationsToInitialState() {
    while (true) {
        system("cls");
        std::cout << "您确定要恢复所有站点的初始状态? (Y/N)" << std::endl;
        std::string op;
        std::cin >> op;
        if (op != "Y") {
            if (op == "N") return;
            else {
                std::cout << "输入无效, 请输入(Y/N)" << std::endl;
                Sleep(1200);
                continue;
            }
        }
        break;
    }

    for (int i = 0; i < closedStations.size(); ++i) {
        closedStations[i]->open = true;
    }
    closedStations.clear();

    system("cls");
    std::cout << "全部站点恢复为\"开启\"状态" << std::endl;
    buildGraph();
    Sleep(1200);
    return;
}

void showLineStationsInfo() {
    while (true) {
        system("cls");
        std::cout << "线路编号列表 (1 - 20)" << std::endl;
        std::cout << " 1 - 1号线\t 2 - 2号线" << std::endl;
        std::cout << " 3 - 3号线\t 4 - 4号线" << std::endl;
        std::cout << " 5 - 5号线\t 6 - 6号线" << std::endl;
        std::cout << " 7 - 7号线\t 8 - 8号线" << std::endl;
        std::cout << " 9 - 9号线\t10 - 10号线" << std::endl;
        std::cout << "11 - 11号线\t12 - 12号线" << std::endl;
        std::cout << "13 - 13号线\t14 - 14号线" << std::endl;
        std::cout << "15 - 15号线\t16 - 16号线" << std::endl;
        std::cout << "17 - 17号线\t18 - 18号线" << std::endl;
        std::cout << "19 - 浦江线\t20 - 机场市域线" << std::endl;

        std::cout << "请输入要查询的线路编号 (1 - 20) : ";
        std::string line;
        std::cin >> line;
        bool match = false;
        for (int i = 1; i <= 20; ++i) {
            if (std::to_string(i) == line) {
                match = true;

                std::string query_line = std::to_string(i) + "号线";
                if (i == 19) query_line = "浦江线";
                else if (i == 20) query_line = "市域机场线";

                std::vector<Station*> query_stations;
                bool enter = true;
                for (int j = 0; j < stations.size(); ++j) {
                    if (stations[j]->line == query_line) {
                        enter = false;
                        query_stations.push_back(stations[j]);
                    }
                    if (!enter && stations[j]->line != query_line) break;
                }

                std::cout << query_line << "共有" << query_stations.size() << "个站点" << std::endl;
                std::cout << std::endl;

                for (int j = 0; j < query_stations.size(); ++j) {
                    if (j) std::cout << "|" << std::endl;
                    std::cout << "○ " << query_stations[j]->name << std::endl;
                }
                std::cout << std::endl;

                system("pause");
                return;
            }
        }
        if (!match) {
            std::cout << "输入无效, 请输入数字选项 (1 - 20)";
            Sleep(1000);
            continue;
        }
    }
}

void releaseStationMemory() {
    for (int i = 0; i < stations.size(); ++i) delete stations[i];
}