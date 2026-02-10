#include "Maze.h"
#include "Solver.h"
int main() {
	 Maze maze;
	 Solver solver(maze);
	 while (true) {
		  cout << "===== Maze Game =====\n"
				<< "1. Input Maze\n"
				<< "2. Print Maze\n"
				<< "3. BFS solve\n"
				<< "4. DFS solve\n"
				<< "5. A* solve\n"
				<< "6. Output in file\n"
				<< "Q. Exit\n"
				<< "Choose an option: ";
		  string choice;
		  bool success_dfs = false;
		  bool success_bfs = false;
		  cin >> choice;
		  if (choice == "Q" || choice == "q") {		 //done
				cout << "End the Maze Game...\n" << endl;
				break;
		  }
		  if (choice == "1") {
				cout << "want to read maze with file enter 'Yes' else 'No'" << endl;
				string temp;
				cin >> temp;
				if(temp=="No"||temp=="no") {
					 maze.inputInfo_Maze();
				}
				else if (temp == "Yes" || temp == "yes") {
					 maze.read_maze("model.txt");
					 cout << "Maze successfully read from model.txt" << endl;
				}
				else {
					 cout << "Invalid input. Please enter 'Yes' or 'No'." << endl;
				}
		  }
		  else if (choice == "2") {		 //done
				if (maze.get_grid().empty()) {
					 cout << "No maze available. Please input a maze first."<<endl;
					 continue;
				}
				maze.print();
		  }
		  else if (choice == "3") {		 //done
				if (maze.get_grid().empty()) {
					 cout << "No maze available. Please input a maze first." << endl;
					 continue;
				}
				bool animate = false;
				string temp;
				cout << "Do you want to animate the solving process? (yes/no): ";
				cin >> temp;
				if (temp == "yes") {
					 animate = true;
				}
				int ms = 50;
				solver.BFS_solve(animate,ms);
		  }
		  else if (choice == "4") {		 //done
				if (maze.get_grid().empty()) {
					 cout << "No maze available. Please input a maze first." << endl;
					 continue;
				}
				bool animate = false;
				string temp;
				cout << "Do you want to animate the solving process? (yes/no): ";
				cin >> temp;
				if (temp == "yes") {
					 animate = true;
				}
				int ms = 50;
				solver.DFS(animate, ms);
				solver.Print_allroads();
		  }
		  else if (choice == "5") {		 //done
				if (maze.get_grid().empty()) {
					 cout << "No maze available. Please input a maze first." << endl;
					 continue;
				}
				bool animate = false;
				string temp;
				cout << "Do you want to animate the solving process? (yes/no): ";
				cin >> temp;
				if (temp == "yes") {
					 animate = true;
				}
				int ms = 50;
				solver.AStar_solve(animate, ms);
				solver.AStar_print();
		  }
		  else if (choice == "6") {		 //done
				string filename = "result.txt";
				if (maze.print_Maze_ToFile(filename)) {
					 cout << "Maze successfully written to " << filename << endl;
				}
				else {
					 cout << "Failed to write maze to file." << endl;
				}
				if (solver.print_BFS_Tofile(filename)) {
					 cout << "BFS solver for Maze successfully written to " << filename << endl;
				}
				else {
					 cout << "Failed to write BFS to file." << endl;
				}
				if (solver.print_DFS_Tofile(filename)) {
					 cout << "DFS solver for Maze successfully written to " << filename << endl;
				}
				else {
					 cout << "Failed to write DFS to file." << endl;
				}
				if (solver.print_AStar_Tofile(filename)) {
					 cout << "A* solver for Maze successfully written to " << filename << endl;
				}
				else {
					 cout << "Failed to write A* to file." << endl;
				}
		  }
		  else {
				cout << "Invalid choice. Please try again." << endl;
		  }
	 }
	 return 0;
}