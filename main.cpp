#include <algorithm>  // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;

// TODO: Add kStart and kFinish enumerators to the State enum.
enum class State {kStart, kFinish, kEmpty, kBlocked, kClosed, kPath};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};


vector<State> ParseLine(string line) {
    istringstream line_stream(line);
    int n;
    char c;
    vector<State> row;
    while (line_stream >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kBlocked);
      }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};

  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}


bool Compare(const vector<int> first, const vector<int> second) {
  int f1 = first[2] + first[3]; // f1 = g1 + h1
  int f2 = second[2] + second[3]; // f2 = g2 + h2
  return f1 > f2; 
}


void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}


int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}


bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  bool in_x = (x >= 0 && x < grid.size());
  bool in_y = (y >= 0 && y < grid[0].size());
  if (in_x && in_y)
    return grid[x][y] == State::kEmpty;
  return false;
}


void AddToOpen(int x, int y, int g, int h,
               vector<vector<int>> &open, vector<vector<State>> &grid) {
  // Add node to open vector, and mark grid cell as closed.
  open.push_back(vector<int>{x, y, g, h});
  grid[x][y] = State::kClosed;
}


void ExpandNeighbors(const vector<int> &current, int goal[2],
                     vector<vector<int>> &open, vector<vector<State>> &grid) {
  int x = current[0];
  int y = current[1];
  int g = current[2];

  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    if (CheckValidCell(x2, y2, grid)) {
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, open, grid);
    }
  }
}


vector<vector<State>> Search(vector<vector<State>> grid,
                             int init[2], int goal[2]) {
  vector<vector<int>> open {};
  
  int x = init[0];
  int y = init[1];
  int g = 0;
  int h = Heuristic(x, y, goal[0],goal[1]);
  AddToOpen(x, y, g, h, open, grid);

  while (open.size() > 0) {
    CellSort(&open);
    auto current = open.back();
    open.pop_back();
    x = current[0];
    y = current[1];
    grid[x][y] = State::kPath;

    if (x == goal[0] && y == goal[1]) {
      grid[init[0]][init[1]] = State::kStart;
      grid[x][y] = State::kFinish; 
      return grid;
    }
    
    ExpandNeighbors(current, goal, open, grid);
  }
  cout << "No path found!" << "\n";
  return vector<vector<State>>{};
}


string CellString(State cell) {
  switch(cell) {
    case State::kBlocked: return "⛰️   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    case State::kPath: return "🚗   ";
    default: return " 0   "; 
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}


int main() {
  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("grid.board");
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
}
