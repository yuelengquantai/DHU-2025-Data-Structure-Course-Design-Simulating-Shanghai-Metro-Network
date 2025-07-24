#include "menu.h"
#include "station.h"
#include "graph.h"
using namespace std;

int main() {
	initStation(); // 初始化站点数据
	buildIndex();  // 构建站点索引
	buildGraph();  // 构建图
	menu();        // 菜单

	releaseStationMemory(); // 释放站点内存
	return 0;
}