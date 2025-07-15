#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "station.h"
#include "pathfinder.h"
using namespace std;

void getStationInfo();
void getShortestTimePath();
void getMinTransferPath();

void menu() {
	cout << "====地铁路径规划系统====" << endl;
	cout << "1. 线路站点信息/运营状态管理" << endl;
	cout << "2. 所需时间最短路径规划" << endl;
	cout << "3. 所需换乘次数最少路径规划" << endl;
	cout << "4. 退出系统" << endl;
	cout << "请输入选项编号: " << endl;

	string op;
	cin >> op;

	if (op == "1") {
		system("cls");
		getStationInfo();
	}
	else if (op == "2") {
		system("cls");
		getShortestTimePath();
	}
	else if (op == "3") {
		system("cls");
		getMinTransferPath();
	}
	else if (op == "4") {
		exit(0);
	}
	else {
		cout << "输入无效, 请输入数字选项 1、2、3、4" << endl;
		Sleep(1500);
		system("cls");
		menu();
	}

}

void getStationInfo() {
	cout << "-- 线路站点信息/运营状态管理 --" << endl;
	cout << "1. 从 CSV 文件批量更新站点开启/关闭状态" << endl;
	cout << "2. 手工更新站点开启/关闭状态" << endl;
	cout << "3. 显示当前关闭站点" << endl;
	cout << "4. 恢复所有站点初始状态" << endl;
	cout << "5. 显示线路站点信息" << endl;
	cout << "6. 返回上级菜单" << endl;
	cout << "请输入选项编号: " << endl;

	string op;
	cin >> op;

	if (op == "1") {
		system("cls");
		batchUpdateStationStatusFromCsv();
	}
	else if (op == "2") {
		system("cls");
		updateStationStatusManually();
	}
	else if (op == "3") {
		system("cls");
		showClosedStations();
	}
	else if (op == "4") {
		system("cls");
		resetAllStationsToInitialState();
	}
	else if (op == "5") {
		system("cls");
		showLineStationsInfo();
	}
	else if (op == "6") {
		system("cls");
		menu();
	}
	else {
		cout << "输入无效, 请输入数字选项 1、2、3、4、5、6" << endl;
		Sleep(1500);
		system("cls");
		getStationInfo();
	}
}

void getShortestTimePath() {
	cout << "-- 所需时间最短路径规划 --" << endl;
	cout << "1. 单条所需时间最短路径" << endl;
	cout << "2. 3条所需时间最短路径" << endl;
	cout << "3. 返回上级菜单" << endl;
	cout << "请输入选项编号: ";

	string op;
	cin >> op;

	if (op == "1") {
		minTimePath();
	}
	else if (op == "2") {
		minTimePath3();
	}
	else if (op == "3") {
		system("cls");
		menu();
	}
	else {
		cout << "输入无效, 请输入数字选项 1、2、3" << endl;
		Sleep(1500);
		system("cls");
		getShortestTimePath();
	}
}

void getMinTransferPath() {
	cout << "-- 所需换乘次数最少路径规划 --" << endl;
	cout << "1. 单条换乘次数最少路径" << endl;
	cout << "2. 3条换乘次数最少路径" << endl;
	cout << "3. 返回主菜单" << endl;
	cout << "请输入选项编号: " << endl;

	string op;
	cin >> op;

	if (op == "1") {
		minTransferPath();
	}
	else if (op == "2") {
		minTransferPath3();
	}
	else if (op == "3") {
		system("cls");
		menu();
	}
	else {
		cout << "输入无效, 请输入数字选项 1、2、3" << endl;
		Sleep(1500);
		system("cls");
		getMinTransferPath();
	}
}
