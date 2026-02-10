#ifndef SOLVER_H
#define SOLVER_H
#include "Maze.h"
struct AStar_Node {		//A*算法结构体
	 int x, y;
	 int g, h, f;			//f = g+h ; g = abs(x-endX) ; h = abs(y-endY)
	 AStar_Node* parent;
	 AStar_Node(int a, int b, int c, int d, int e, AStar_Node* node = nullptr);
};
class Solver{
private:
	 Maze& maze;
	 vector<vector<pair<int, int>>> DFS_allroads;		  //深度算法-所有通路
	 vector<pair<int, int>> DFS_currentroad;	  //深度算法-当前路径
	 vector<pair<int, int>> BFS_road;		  //广度算法-最优路径
	 vector<pair<int, int>> AStar_road;		  //A*算法-最优路径
public:
	 Solver(Maze& m);
	 void DFS(bool animate = false, int step = 50);		  //深度算法
	 void DFS_solve(int x, int y, vector<vector<int>>& visited, vector<pair<int, int>>& path, bool animate, int step);		  //深度算法求解全部路径
	 void BFS_solve(bool animate = false, int step = 50);		  //广度算法求解迷宫
	 void BFS_print();		  //广度算法打印路径
	 void Print_allroads();		  //打印所有路径
	 void Display_roads(const vector<pair<int,int>>& path) const;		  //动态显示探索过程
	 void Print_grid(const vector<pair<int,int>>& path) const;		  //输出迷宫模型
	 bool print_BFS_Tofile(const string& filename) const;			 //输出BFS结果到文件
	 bool print_DFS_Tofile(const string& filename) const;			 //输出DFS结果到文件
	 bool AStar_solve(bool animate = false, int step = 50);		 //A*算法求解迷宫
	 int heuristic(int x, int y) const;		 //启发函数
	 void AStar_print();		 //A*算法打印路径
	 vector<pair<int, int>> getAStarPath(AStar_Node* node) const;			//用于动态显示算法的探索过程
	 bool print_AStar_Tofile(const string& filename) const;		 //输出A*结果到文件
};
#endif
