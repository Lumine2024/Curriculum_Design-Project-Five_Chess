#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include<algorithm>
#include<utility>
#include<vector>
struct MOUSEMSG;
class Search_AI;
class Five_Chess {
	friend void printboard(const Five_Chess &game, const MOUSEMSG &msg);
	friend class Search_AI;
private:
	std::vector<std::vector<char>> chessboard;
	std::vector<char> player;
	bool is_index_0_player_playing;
public:
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
};

#endif // GAME_LOGIC_H