#include "Solver.h"
#include <thread>
#include <chrono>
#include <queue>
#include <fstream>
//东南西北
static int dx[4] = { 0,1,0,-1 };
static int dy[4] = { 1,0,-1,0 };
Solver::Solver(Maze& m):maze(m) {
	 DFS_allroads.clear();
	 DFS_currentroad.clear();
	 BFS_road.clear();
}
void Solver::Display_roads(const vector<pair<int, int>>& path) const {
	 for (int i = 0;i < 5;i++) {
		  cout << endl;
	 }
	 vector<string> g = maze.get_grid();
	 for (auto& p : path) {
		  int x = p.first;
		  int y = p.second;
		  if (maze.inBounds(x, y) && g[x][y] == '.') {
				g[x][y] = '$';
		  }
	 }
	 if (maze.inBounds(maze.get_startX(), maze.get_startY())) {
		  g[maze.get_startX()][maze.get_startY()] = 'S';
	 }
	 if (maze.inBounds(maze.get_endX(), maze.get_endY())) {
		  g[maze.get_endX()][maze.get_endY()] = 'E';
	 }
	 cout << "Maze Layout:" << endl;
	 for (auto& row : g) {
		  cout << row << endl;
	 }
	 cout.flush();
}
void Solver::DFS(bool animate, int step) {
	 DFS_allroads.clear();
	 DFS_currentroad.clear();
	 if (!maze.isPass(maze.get_startX(), maze.get_startY()) || !maze.isPass(maze.get_endX(), maze.get_endY())) {
		  cout << "No valid start or end point." << endl;
		  return;
	 }
	 int height = maze.get_m();
	 int width = maze.get_n();
	 vector<vector<int>> visited(maze.get_m(), vector<int>(maze.get_n(), 0));			//记录节点是否被已访问
	 vector<pair<int, int>> path;			 //记录走过路径
	 DFS_solve(maze.get_startX(), maze.get_startY(), visited, path, animate, step);
	 if (!DFS_allroads.empty()) {
		  DFS_currentroad = DFS_allroads[0];
	 }
}
void Solver::DFS_solve(int x, int y, vector<vector<int>>& visited, vector<pair<int, int>>& path, bool animate, int step) {
	 if (!maze.inBounds(x, y) || !maze.isPass(x, y) || visited[x][y]) {
		  return;
	 }
	 visited[x][y] = 1;		 //标记当前节点已被访问
	 path.push_back({ x, y });			//存入当前节点的路径
	 if (animate) {
		  Display_roads(path);
		  this_thread::sleep_for(chrono::milliseconds(step));
	 }
	 if (x == maze.get_endX() && y == maze.get_endY()) {			 //如果到达终点
		  DFS_allroads.push_back(path);
	 }
	 else {
		  for (int i = 0;i < 4;i++) {		 //历遍四个方向
				int nx = x + dx[i];
				int ny = y + dy[i];
				DFS_solve(nx, ny, visited, path, animate, step);
		  }
	 }
	 //节点回溯到分岔路
	 path.pop_back();			 //出栈
	 visited[x][y] = 0;		 //还原访问记录
	 if (animate) {
		  Display_roads(path);
		  this_thread::sleep_for(chrono::milliseconds(step));
	 }
	 return;
}
void Solver::Print_allroads() {
	 if (DFS_allroads.empty()) {
		  cout << "No paths found." << endl;
		  return;
	 }
	 cout << "Total found path have " << DFS_allroads.size() << " roads." << endl;
	 for (size_t i = 0;i < DFS_allroads.size();i++) {
		  cout << "Path" << (i + 1) << ":" << endl;
		  for (size_t j = 0;j < DFS_allroads[i].size();j++) {
				auto p = DFS_allroads[i][j];
				cout << "(" << p.first << "," << p.second << ")";
				if (j < DFS_allroads[i].size()-1) {
					 cout << " -> ";
				}
		  }
		  cout << endl;
	 }
}
void Solver::BFS_solve(bool animate, int step) {
	 BFS_road.clear();
	 if (!maze.isPass(maze.get_startX(), maze.get_startY()) || !maze.isPass(maze.get_endX(), maze.get_endY())) {
		  cout << "No valid start or end point." << endl;
		  return;
	 }
	 int height = maze.get_m();
	 int width = maze.get_n();
	 vector<vector<int>> visited(height, vector<int>(width, 0));
	 vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width, { -1,-1 }));
	 queue<pair<int, int>> q;			//队列
	 q.push({ maze.get_startX(), maze.get_startY() });			//起点入列
	 visited[maze.get_startX()][maze.get_startY()] = 1;
	 bool found = false;
	 vector<pair<int, int>> visitedPath;	  //动画展示
	 while (!q.empty()) {
		  auto real = q.front();
		  q.pop();
		  int x = real.first;
		  int y = real.second;
		  visitedPath.push_back(real);
		  if (animate) {
				vector<string> gg = maze.get_grid();
				for (auto& p : visitedPath) {
					 if (maze.inBounds(p.first, p.second) && gg[p.first][p.second] == '.') {
						  gg[p.first][p.second] = '$';		  //标记访问路线
					 }
				}
				if (maze.inBounds(maze.get_startX(), maze.get_startY())) {
					 gg[maze.get_startX()][maze.get_startY()] = 'S';
				}
				if (maze.inBounds(maze.get_endX(), maze.get_endY())) {
					 gg[maze.get_endX()][maze.get_endY()] = 'E';
				}
				cout << "\nMaze Layout:" << endl;
				for (auto& row : gg) {
					 cout << row << endl;
				}
				cout.flush();
				this_thread::sleep_for(chrono::milliseconds(step));
		  }
		  if (x == maze.get_endX() && y == maze.get_endY()) {
				found = true;
				break;
		  }
		  for (int i = 0;i < 4;i++) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if (!maze.inBounds(nx, ny)) {
					 continue;
				}
				if (!maze.isPass(nx, ny)) {
					 continue;
				}
				if (visited[nx][ny]) {
					 continue;
				}
				visited[nx][ny] = 1;
				parent[nx][ny] = { x, y };
				q.push({ nx, ny });
		  }
	 }
	 if (!found) {
		  cout << "No path found from start to end." << endl;
		  return;
	 }
	 pair<int, int> current = { maze.get_endX(), maze.get_endY() };
	 while (current.first != -1) {
		  BFS_road.push_back(current);
		  current = parent[current.first][current.second];
	 }
	 reverse(BFS_road.begin(), BFS_road.end());
	 cout << "BFS solve successful, path length: " << BFS_road.size() << endl;
	 size_t i = 0;
	 for (auto& p : BFS_road) {
		  cout << "(" << p.first << "," << p.second << ") ";
		  if (i< BFS_road.size()-1) {
				cout << "->";
				i++;
		  }
	 }
	 cout << endl;
	 BFS_print();
}
void Solver::BFS_print() {
	 if (BFS_road.empty()) {
		  cout << "No BFS path found to display." << endl;
		  return;
	 }
	 vector<string> g = maze.get_grid();
	 for (auto& p : BFS_road) {
		  int x = p.first;
		  int y = p.second;
		  if (maze.inBounds(x, y) && g[x][y] == '.') {
				g[x][y] = '$';
		  }
	 }
	 if (maze.inBounds(maze.get_startX(), maze.get_startY())) {
		  g[maze.get_startX()][maze.get_startY()] = 'S';
	 }
	 if (maze.inBounds(maze.get_endX(), maze.get_endY())) {
		  g[maze.get_endX()][maze.get_endY()] = 'E';
	 }
	 cout << "Maze Layout with BFS Path:" << endl;
	 for (auto& row : g) {
		  cout << row << endl;
	 }
}
void Solver::Print_grid(const vector<pair<int, int>>& path) const {
	 vector<string> g = maze.get_grid();
	 for (auto& p : path) {
		  int x = p.first;
		  int y = p.second;
		  if (maze.inBounds(x, y) && g[x][y] == '.') {
				g[x][y] = '$';
		  }
	 }
	 for (auto& row : g) {
		  cout << row << endl;
	 }
}
bool Solver::print_BFS_Tofile(const string& filename) const {
	 ofstream outFile(filename, ios::app);
	 if (!outFile.is_open()) {
		  cerr << "Error: Could not open file " << filename << " for writing." << endl;
		  return false;
	 }
	 outFile << "Maze Layout with BFS Path:" << endl;
	 vector<string> g = maze.get_grid();
	 for (auto& p : BFS_road) {
		  int x = p.first;
		  int y = p.second;
		  if (maze.inBounds(x, y) && g[x][y] == '.') {
				g[x][y] = '$';
		  }
	 }
	 if (maze.inBounds(maze.get_startX(), maze.get_startY())) {
		  g[maze.get_startX()][maze.get_startY()] = 'S';
	 }
	 if (maze.inBounds(maze.get_endX(), maze.get_endY())) {
		  g[maze.get_endX()][maze.get_endY()] = 'E';
	 }
	 for (auto& row : g) {
		  outFile << row << endl;
	 }
	 size_t i = 0;
	 for (auto& bfs : BFS_road) {
		  outFile << "(" << bfs.first << "," << bfs.second << ") ";
		  if (i<BFS_road.size()-1) {
				outFile << " -> ";
				i++;
		  }
	 }
	 outFile << endl;
	 outFile.close();
	 return true;
}
bool Solver::print_DFS_Tofile(const string& filename) const {
	 fstream outFile(filename, ios::app);
	 if (!outFile.is_open()) {
		  cerr << "Error: Could not open file " << filename << " for writing." << endl;
		  return false;
	 }
	 outFile << "Maze Layout with DFS:" << endl;
	 outFile << "Total found path have " << DFS_allroads.size() << " roads." << endl;
	 for (size_t i = 0;i < DFS_allroads.size();i++) {
		  outFile << "Path" << (i + 1) << ":" << endl;
		  for (size_t j = 0;j < DFS_allroads[i].size();j++) {
				auto p = DFS_allroads[i][j];
				outFile << "(" << p.first << "," << p.second << ")";
				if (j < DFS_allroads[i].size()-1) {
					 outFile << " -> ";
				}
		  }
		  outFile << endl;
	 }
	 outFile << endl;
	 outFile.close();
	 return true;
}
AStar_Node::AStar_Node(int a, int b, int c, int d, int e, AStar_Node* node) {
	 x = a;
	 y = b;
	 g = c;
	 h = d;
	 f = e;
	 parent = node;
}
bool Solver::AStar_solve(bool animate, int step) {
	 AStar_road.clear();
	 int Start_X = maze.get_startX();
	 int Start_Y = maze.get_startY();
	 int End_X = maze.get_endX();
	 int End_Y = maze.get_endY();
	 vector<AStar_Node*> openList;		 //存放待考察节点
	 vector<vector<int>> visited(maze.get_m(), vector<int>(maze.get_n(), 0));			//标记已访问节点
	 int h = heuristic(Start_X, Start_Y);
	 openList.push_back(new AStar_Node(Start_X, Start_Y, 0, h, h, nullptr));	  //记录起点
	 AStar_Node* endNode = nullptr;
	 while (!openList.empty()) {
		  //找到最小f
		  int minIndex = 0;
		  for (size_t i = 1;i < openList.size();i++) {
				if (openList[i]->f < openList[minIndex]->f) {
					 minIndex = i;
				}
		  }
		  //最小节点
		  AStar_Node* currentNode = openList[minIndex];
		  //删除最小节点，并标记访问
		  openList.erase(openList.begin() + minIndex);
		  if (visited[currentNode->x][currentNode->y] == 1) {
				continue;
		  }
		  visited[currentNode->x][currentNode->y] = 1;
		  if (animate) {
				auto path = getAStarPath(currentNode);
				Display_roads(path);
				this_thread::sleep_for(chrono::milliseconds(step));
		  }
		  if (currentNode->x == End_X && currentNode->y == End_Y) {
				endNode = currentNode;
				break;
		  }
		  //循环四个方向
		  for (int i = 0;i < 4;i++) {
				int newx = currentNode->x + dx[i];
				int newy = currentNode->y + dy[i];
				if (!maze.isPass(newx, newy) || visited[newx][newy]) {
					 continue;
				}
				int newg = currentNode->g + 1;		//代价+1
				int newh = heuristic(newx, newy);	//计算
				int newf = newg + newh;
				openList.push_back(new AStar_Node(newx, newy, newg, newh, newf, currentNode));
		  }
	 }
	 if (endNode == nullptr) {
		  cout << "No path found from start to end using A*." << endl;
		  return false;
	 }
	 while (endNode) {
		  AStar_road.push_back({ endNode->x, endNode->y });
		  endNode = endNode->parent;
	 }
	 reverse(AStar_road.begin(), AStar_road.end());
	 return true;
	 /*要拿当前节点减去终点节点
	 更新每一次的父节点
	 走的路就看面临的分叉路口循环for4个方向*/
}
int Solver::heuristic(int x, int y) const {
	 return abs(x - maze.get_endX()) + abs(y - maze.get_endY());
}
vector<pair<int, int>> Solver::getAStarPath(AStar_Node* node) const {
	 vector<pair<int, int>> path;
	 while (node!=nullptr) {
		  path.push_back({ node->x, node->y });
		  node = node->parent;
	 }
	 reverse(path.begin(), path.end());
	 return path;
}
void Solver::AStar_print() {
	 if (AStar_road.empty()) {
		  cout << "No A* path found to display." << endl;
		  return;
	 }
	 vector<string> g = maze.get_grid();
	 for (auto& p : AStar_road) {
		  int x = p.first;
		  int y = p.second;
		  if (maze.inBounds(x, y) && g[x][y] == '.') {
				g[x][y] = '$';
		  }
	 }
	 if (maze.inBounds(maze.get_startX(), maze.get_startY())) {
		  g[maze.get_startX()][maze.get_startY()] = 'S';
	 }
	 if (maze.inBounds(maze.get_endX(), maze.get_endY())) {
		  g[maze.get_endX()][maze.get_endY()] = 'E';
	 }
	 cout << "A* Path length:" << AStar_road.size() << endl;
	 cout << "Maze Layout with A* Path:" << endl;
	 for (auto& row : g) {
		  cout << row << endl;
	 }
	 size_t i = 0;
	 for (auto& as : AStar_road) {
		  cout << "(" << as.first << "," << as.second << ") ";
		  if (i<AStar_road.size()-1) {
				cout << " -> ";
				i++;
		  }
	 }
	 cout << endl;
}
bool Solver::print_AStar_Tofile(const string& filename) const {
	 ofstream Outfile(filename, ios::app);
	 if (!Outfile.is_open()) {
		  cerr << "Error: Could not open file " << filename << " for writing." << endl;
		  return false;
	 }
	 vector<string> g = maze.get_grid();
	 Outfile << "Maze Layout with A* Path:" << endl;
	 for (auto& as : AStar_road) {
		  int x = as.first;
		  int y = as.second;
		  if (maze.inBounds(x, y) && g[x][y] == '.') {
				g[x][y] = '$';
		  }
	 }
	 if (maze.inBounds(maze.get_startX(), maze.get_startY())) {
		  g[maze.get_startX()][maze.get_startY()] = 'S';
	 }
	 if (maze.inBounds(maze.get_endX(), maze.get_endY())) {
		  g[maze.get_endX()][maze.get_endY()] = 'E';
	 }
	 for (auto& row : g) {
		  Outfile << row << endl;
	 }
	 size_t i = 0;
	 for (auto& as : AStar_road) {
		  Outfile << "(" << as.first << "," << as.second << ") ";
		  if (i<AStar_road.size()-1) {
				Outfile << " -> ";
				i++;
		  }
	 }
	 Outfile << endl;
	 Outfile.close();
}