#include"Game Logic.h"
#include"Graphics Logic.h"
using namespace std;
extern HWND game;
void Five_Chess::putchess(int x, int y) {
	if(chessboard[x][y] == ' ') {
		chessboard[x][y] = is_index_0_player_playing ? player[0] : player[1];
		is_index_0_player_playing = !is_index_0_player_playing;
	}
	else {
		throw Occupied();
	}
}

void Five_Chess::rmchess(int x, int y) {
	chessboard[x][y] = ' ';
	is_index_0_player_playing = !is_index_0_player_playing;
}

bool Five_Chess::has_ended(char &ch) {
	if(generate_possible_moves().empty()) {
		ch = '3';
		return true;
	}
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[i][j] == chessboard[i][j + 1] && chessboard[i][j + 1] == chessboard[i][j + 2] && chessboard[i][j + 2] == chessboard[i][j + 3] && chessboard[i][j + 3] == chessboard[i][j + 4] && chessboard[i][j] != ' ') {
				ch = chessboard[i][j];
				return true;
			}
		}
	}
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[j][i] == chessboard[j + 1][i] && chessboard[j + 1][i] == chessboard[j + 2][i] && chessboard[j + 2][i] == chessboard[j + 3][i] && chessboard[j + 3][i] == chessboard[j + 4][i] && chessboard[j][i] != ' ') {
				ch = chessboard[j][i];
				return true;
			}
		}
	}
	for(int i = 0; i < 11; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[i][j] == chessboard[i + 1][j + 1] && chessboard[i + 1][j + 1] == chessboard[i + 2][j + 2] && chessboard[i + 2][j + 2] == chessboard[i + 3][j + 3] && chessboard[i + 3][j + 3] == chessboard[i + 4][j + 4] && chessboard[i][j] != ' ') {
				ch = chessboard[i][j];
				return true;
			}
		}
	}
	for(int i = 4; i < 15; ++i) {
		for(int j = 0; j < 11; ++j) {
			if(chessboard[i][j] == chessboard[i - 1][j + 1] && chessboard[i - 1][j + 1] == chessboard[i - 2][j + 2] && chessboard[i - 2][j + 2] == chessboard[i - 3][j + 3] && chessboard[i - 3][j + 3] == chessboard[i - 4][j + 4] && chessboard[i][j] != ' ') {
				ch = chessboard[i][j];
				return true;
			}
		}
	}
	ch = ' ';
	return false;
}
char Five_Chess::the_player_who_wins() {
	char ch;
	has_ended(ch);
	return ch;
}

vector<pair<int, int>> Five_Chess::generate_possible_moves() const {
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