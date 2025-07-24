#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "station.h"
#include "pathfinder.h"

void getStationInfo();
void getShortestTimePath();
void getMinTransferPath();

void menu() {
    while (true) {
        system("cls");
        std::cout << "====地铁路径规划系统====" << std::endl;
        std::cout << "1. 线路站点信息/运营状态管理" << std::endl;
        std::cout << "2. 所需时间最短路径规划" << std::endl;
        std::cout << "3. 所需换乘次数最少路径规划" << std::endl;
        std::cout << "4. 退出系统" << std::endl;
        std::cout << "请输入选项编号: " << std::endl;

        std::string op;
        std::cin >> op;

        if (op == "1") {
            getStationInfo();
			continue;
        } else if (op == "2") {
            getShortestTimePath();
			continue;
        } else if (op == "3") {
            getMinTransferPath();
			continue;
        } else if (op == "4") {
            exit(0);
        } else {
            std::cout << "输入无效, 请输入数字选项 1、2、3、4" << std::endl;
            Sleep(1500);
        }
    }
}

void getStationInfo() {
    while (true) {
        system("cls");
        std::cout << "-- 线路站点信息/运营状态管理 --" << std::endl;
        std::cout << "1. 从 CSV 文件批量更新站点开启/关闭状态" << std::endl;
        std::cout << "2. 手工更新站点开启/关闭状态" << std::endl;
        std::cout << "3. 显示当前关闭站点" << std::endl;
        std::cout << "4. 恢复所有站点初始状态" << std::endl;
        std::cout << "5. 显示线路站点信息" << std::endl;
        std::cout << "6. 返回上级菜单" << std::endl;
        std::cout << "请输入选项编号: " << std::endl;
        std::string op;
        std::cin >> op;
        if (op == "1") {
            batchUpdateStationStatusFromCsv();
			continue;
        } else if (op == "2") {
            updateStationStatusManually();
			continue;
        } else if (op == "3") {
            showClosedStations();
			continue;
        } else if (op == "4") {
            resetAllStationsToInitialState();
			continue;
        } else if (op == "5") {
            showLineStationsInfo();
			continue;
        } else if (op == "6") {
            return;
        } else {
            std::cout << "输入无效, 请输入数字选项 1、2、3、4、5、6" << std::endl;
            Sleep(1500);
        }
    }
}

void getShortestTimePath() {
    while (true) {
        system("cls");
        std::cout << "-- 所需时间最短路径规划 --" << std::endl;
        std::cout << "1. 单条所需时间最短路径" << std::endl;
        std::cout << "2. 3条所需时间最短路径" << std::endl;
        std::cout << "3. 返回上级菜单" << std::endl;
        std::cout << "请输入选项编号: ";
        std::string op;
        std::cin >> op;
        if (op == "1") {
            minTimePath();
			continue;
        } else if (op == "2") {
            minTimePath3();
			continue;
        } else if (op == "3") {
            break;
			continue;
        } else {
            std::cout << "输入无效, 请输入数字选项 1、2、3" << std::endl;
            Sleep(1500);
        }
    }
}

void getMinTransferPath() {
    while (true) {
        system("cls");
        std::cout << "-- 所需换乘次数最少路径规划 --" << std::endl;
        std::cout << "1. 单条换乘次数最少路径" << std::endl;
        std::cout << "2. 3条所需换乘次数最少路径" << std::endl;
        std::cout << "3. 返回主菜单" << std::endl;
        std::cout << "请输入选项编号: " << std::endl;
        std::string op;
        std::cin >> op;
        if (op == "1") {
            minTransferPath();
			continue;
        } else if (op == "2") {
            minTransferPath3();
			continue;
        } else if (op == "3") {
            break;
        } else {
            std::cout << "输入无效, 请输入数字选项 1、2、3" << std::endl;
            Sleep(1500);
        }
    }
}
