#pragma once
#include<algorithm>
#include<utility>
#include<vector>
struct MOUSEMSG;
class Search_AI;
class AlphaBetaAI;
class Five_Chess {
public:
	std::vector<std::vector<char>> chessboard;
	std::vector<char> player;
	bool is_index_0_player_playing;
	Five_Chess() : chessboard(15, std::vector<char>(15, ' ')), player({ '1','2' }), is_index_0_player_playing(false) {}
	Five_Chess(const Five_Chess &other) : chessboard(other.chessboard), player(other.player), is_index_0_player_playing(other.is_index_0_player_playing) {}
	Five_Chess &operator=(const Five_Chess &other) {
		if(this != &other) {
			chessboard = other.chessboard;
			player = other.player;
			is_index_0_player_playing = other.is_index_0_player_playing;
		}
		return *this;
	}
	void putchess(int x, int y);
	void rmchess(int x, int y);
	bool has_ended(char &ch);
	char the_player_who_wins();
	std::vector<std::pair<int, int>> generate_possible_moves() const;
	bool player_playing() const { return is_index_0_player_playing; }
};