#include "Maze.h"
#include <random>
#include <fstream>
#include <algorithm>
Maze::Maze() {
	 m = 0;		 //行
	 n = 0;		 //列
	 start_X = 0;
	 start_Y = 0;
	 end_X = 0;
	 end_Y = 0;
	 grid.clear();
}
void Maze::inputInfo_Maze() {
	 int row, col;
	 cout << "pls enter the row and col of maze:" << endl;
	 cout << "row: ";
	 cin >> row;
	 cout << "col: ";
	 cin >> col;
	 if (row <= 0 || col <= 0) {
		  cout << "invalid input! pls enter again.\n"
				<< "size must be positive number" << endl;
		  return;
	 }
	 row = (row - 1) / 2;
	 col = (col - 1) / 2;
	 int k = rand() % 10;		  //随机数生成通路
	 generat_algorithm(row, col, k);
	 cout << "successful create maze.\n"
		  << "maze size: " << row * 2 + 1 << " x " << col * 2 + 1 << endl
		  << "start point: (" << start_X << "," << start_Y << ")" << endl
		  << "end point: (" << end_X << "," << end_Y << ")" << endl;
}
void Maze::generat_algorithm(int cellRows, int cellCols, int extraOpening) {
	 int height = cellRows * 2 + 1;
	 int width = cellCols * 2 + 1;
	 iniGrid(height, width);
	 vector<vector<int>> visited(cellRows, vector<int>(cellCols, 0));
	 createMaze(0, 0, visited, cellRows, cellCols);		  //生成迷宫
	 //设置起点和终点
	 start_X = 1;
	 start_Y = 1;
	 end_X = 2 * cellRows - 1;
	 end_Y = 2 * cellCols - 1;
	 //建立多条通路
	 if (extraOpening > 0) {
		  auto road = listWalls();			 //建立回路,获得可以被打破墙的位置
		  //打乱迷宫墙位置
		  if (!road.empty()) {
				for (size_t i = road.size()-1;i > 0;i--) {
					 size_t j = rand() % (i + 1);
					 swap(road[i], road[j]);
				}
		  }
		  //找到合适的墙打通
		  int tobreak = min((int)road.size(), extraOpening);		 //在确保不越界的情况下打通墙
		  for (int i = 0;i < tobreak;i++) {
				int r = road[i].first;
				int c = road[i].second;
				grid[r][c] = '.';
		  }
	 }
}
void Maze::createMaze(int cellR, int cellC, vector<vector<int>>& visited, int cellRows, int cellCols) {
	 visited[cellR][cellC] = 1;		//该单元已被访问
	 int charR = cellR * 2 + 1;	 //对应迷宫的行
	 int charC = cellC * 2 + 1;	 //对应迷宫的列
	 grid[charR][charC] = '.';	  //将该单元设为路（核心代码）
	 int arah[4] = { 0,1,2,3 };		//方向（0:上(-1,0), 1:下(1,0), 2:左(0,-1), 3:右(0,1)
	 //随机数打乱方向
	 for (int i = 3;i > 0;--i) {
		  int j = rand() % (i+1);
		  swap(arah[i], arah[j]);
	 }
	 //判断打乱后的方向
	 for (int i = 0;i < 4;i++) {
		  int d = arah[i];
		  int nr = cellR + (d == 0 ? -1 : (d == 1 ? 1 : 0));		 //判断上下方向的x（行）是1还是-1
		  int nc = cellC + (d == 2 ? -1 : (d == 3 ? 1 : 0));		 //判断左右方向的y（列）是1还是-1
		  //判断是否能走通
		  if (nr >= 0 && nr < cellRows && nc >= 0 && nc < cellCols && visited[nr][nc] == 0) {
				//打通墙的坐标位置
				int wallR = (charR + (2 * nr + 1)) / 2;
				int wallC = (charC + (2 * nc + 1)) / 2;
				grid[wallR][wallC] = '.';		  //打通墙,使其变成路
				createMaze(nr, nc, visited, cellRows, cellCols);		  //递归
		  }
	 }
}
vector<pair<int, int>> Maze::listWalls() {
	 vector<pair<int, int>> result;		 //存放所有可以被打通的墙（行，列）
	 for (int i = 1;i + 1 < m;i++) {
		  for (int j = 1;j + 1 < n;j++) {
				if (grid[i][j] != '|') {
					 continue;
				}
				//判断墙是左右还是上下
				if ((i % 2 == 1 && j % 2 == 0)) {		 //墙在左右
					 if (grid[i][j - 1] == '.' && grid[i][j + 1] == '.') {		 //如果墙的左右两边都是路，则可以打通
						  result.push_back({ i,j });
					 }
				}
				if ((i % 2 == 0 && j % 2 == 1)) {		 //墙在上下
					 if (grid[i - 1][j] == '.' && grid[i + 1][j] == '.') {
						  result.push_back({ i,j });
					 }
				}
		  }
	 }
	 return result;
}
void Maze::iniGrid(int a, int b) {		 //初始化迷宫
	 m = a;
	 n = b;
	 grid.assign(m, string(n, '|'));
}
void Maze::print() {			 //打印迷宫
	 vector<string> g = grid;
	 if (inBounds(start_X, start_Y)) g[start_X][start_Y] = 'S';
	 if (inBounds(end_X, end_Y)) g[end_X][end_Y] = 'E';
	 cout << "Maze Layout:" << endl;
	 for (auto& row : g) {
		  cout << row << endl;
	 }
}
bool Maze::inBounds(int x, int y) const {
	 return x >= 0 && x < m && y >= 0 && y < n;
}
bool Maze::isPass(int x, int y) {
	 if (!inBounds(x, y)) {
		  return false;
	 }
	 return grid[x][y] == '.';
}
bool Maze::print_Maze_ToFile(const string& filename) const {
	 ofstream outFile(filename);
	 if (!outFile.is_open()) {
		  cerr << "Error: Could not open file " << filename << " for writing." << endl;
		  return false;
	 }
	 vector<string> g = grid;
	 //打印迷宫
	 if (inBounds(start_X, start_Y)) g[start_X][start_Y] = 'S';
	 if (inBounds(end_X, end_Y)) g[end_X][end_Y] = 'E';
	 outFile << "Maze Layout" << endl;
	 for (auto& row : g) {
		  outFile << row << endl;
	 }
	 outFile.close();
	 return true;
}
int Maze::get_m() const {
	 return m;
}
int Maze::get_n() const {
	 return n;
}
vector<string> Maze::get_grid() const {
	 return grid;
}
int Maze::get_startX() const {
	 return start_X;
}
int Maze::get_startY() const {
	 return start_Y;
}
int Maze::get_endX() const {
	 return end_X;
}
int Maze::get_endY() const {
	 return end_Y;
}
void Maze::read_maze(const string& filename) {
	 ifstream in(filename);
	 if (!in.is_open()) {
		  cout << "Error: Could not open file " << filename << " for reading." << endl;
		  return;
	 }
	 int n, m;
	 in >> m >> n;
	 this->n = n;
	 this->m = m;
	 grid.resize(m, string(n, ' '));
	 for (int i = 0;i < m;i++) {
		  for (int j = 0;j < n;j++) {
				in >> grid[i][j];
		  }
	 }
	 int sx, sy, ex, ey;
	 in >> sx >> sy >> ex >> ey;
	 start_X = sx;
	 start_Y = sy;
	 end_X = ex;
	 end_Y = ey;
	 in.close();
}