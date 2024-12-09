#include"Game Logic.h"
#include"Graphics Logic.h"
using namespace std;
extern HWND game;
void Five_Chess::putchess(int x, int y) {
	// 为空，落子
	if(chessboard[x][y] == ' ') {
		chessboard[x][y] = is_index_0_player_playing ? '1' : '2';
		is_index_0_player_playing = !is_index_0_player_playing;
	}
	// 被占据，抛异常
	else {
		throw Occupied();
	}
}

void Five_Chess::rmchess(int x, int y) {
	// 不检测了
	chessboard[x][y] = ' ';
	is_index_0_player_playing = !is_index_0_player_playing;
}

bool Five_Chess::has_ended(char &ch) {
	// 棋盘占满，游戏结束
	if(generate_possible_moves().empty()) {
		ch = '3';
		return true;
	}
	// 五子连珠，游戏结束
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[i][j] == chessboard[i][j + 1] &&
				chessboard[i][j + 1] == chessboard[i][j + 2] &&
				chessboard[i][j + 2] == chessboard[i][j + 3] &&
				chessboard[i][j + 3] == chessboard[i][j + 4] &&
				chessboard[i][j] != ' ') {
				ch = chessboard[i][j];
				return true;
			}
		}
	}
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[j][i] == chessboard[j + 1][i] &&
				chessboard[j + 1][i] == chessboard[j + 2][i] &&
				chessboard[j + 2][i] == chessboard[j + 3][i] &&
				chessboard[j + 3][i] == chessboard[j + 4][i] &&
				chessboard[j][i] != ' ') {
				ch = chessboard[j][i];
				return true;
			}
		}
	}
	for(int i = 0; i < 11; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[i][j] == chessboard[i + 1][j + 1] &&
				chessboard[i + 1][j + 1] == chessboard[i + 2][j + 2] &&
				chessboard[i + 2][j + 2] == chessboard[i + 3][j + 3] &&
				chessboard[i + 3][j + 3] == chessboard[i + 4][j + 4] &&
				chessboard[i][j] != ' ') {
				ch = chessboard[i][j];
				return true;
			}
		}
	}
	for(int i = 4; i < 15; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[i][j] == chessboard[i - 1][j + 1] &&
				chessboard[i - 1][j + 1] == chessboard[i - 2][j + 2] &&
				chessboard[i - 2][j + 2] == chessboard[i - 3][j + 3] &&
				chessboard[i - 3][j + 3] == chessboard[i - 4][j + 4] &&
				chessboard[i][j] != ' ') {
				ch = chessboard[i][j];
				return true;
			}
		}
	}
	// 未结束，返回假
	ch = ' ';
	return false;
}

vector<pair<int, int>> Five_Chess::generate_possible_moves() const {
	// 返回所有空的坐标
	vector<pair<int, int>> moves;
	for(int i = 0; i < chessboard.size(); ++i) {
		for(int j = 0; j < chessboard[i].size(); ++j) {
			if(chessboard[i][j] == ' ') {
				moves.emplace_back(i, j);
			}
		}
	}
	return moves;
}