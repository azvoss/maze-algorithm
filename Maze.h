#ifndef MAZE_H
#define MAZE_H
#include <vector>
#include <iostream>
using namespace std;
class Maze{
private:
	 int n;		 //列
	 int m;		 //行
	 int start_X;			//起点x
	 int start_Y;			//起点y
	 int end_X;			//终点x
	 int end_Y;			//终点y
	 vector<string> grid;		  //迷宫
public:
	 Maze();
	 void inputInfo_Maze();		  //输入迷宫参数
	 void print();
	 bool inBounds(int x, int y) const;		  //判断坐标是否合法
	 bool isPass(int x, int y);		  //判断该位置是否为通路
	 void generat_algorithm(int cellRows, int cellCols, int extraOpening);		  //生成迷宫的算法
	 void iniGrid(int a, int b);		//初始化迷宫（先建立全是墙的迷宫）用DFS
	 void createMaze(int cellR, int cellC, vector<vector<int>>& visited, int cellRows, int cellCols);
	 vector<pair<int, int>> listWalls();	  //多迷宫（基于回溯算法打通多个通道）
	 bool print_Maze_ToFile(const string& filename) const;		  //输出到txt文件
	 void read_maze(const string& filename);		  //从txt文件读取迷宫
	 int get_m() const;
	 int get_n() const;
	 vector<string> get_grid() const;
	 int get_startX() const;
	 int get_startY() const;
	 int get_endX() const;
	 int get_endY() const;
};
#endif
//核心代码：createMaze函数