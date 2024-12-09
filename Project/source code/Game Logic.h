#pragma once
#include<algorithm>
#include<utility>
#include<vector>
struct MOUSEMSG;
class Search_AI;
// ������������
class Five_Chess {
public:
	// ����
	std::vector<std::vector<char>> chessboard;
	// �Ƿ�Ϊ��һ��������ڴ�
	bool is_index_0_player_playing;
	Five_Chess() : chessboard(15, std::vector<char>(15, ' ')), is_index_0_player_playing(false) {}
	Five_Chess(const Five_Chess &other) : chessboard(other.chessboard), is_index_0_player_playing(other.is_index_0_player_playing) {}
	Five_Chess &operator=(const Five_Chess &other) {
		if(this != &other) {
			chessboard = other.chessboard;
			is_index_0_player_playing = other.is_index_0_player_playing;
		}
		return *this;
	}
	// ����
	void putchess(int x, int y);
	// �Ƴ�����
	void rmchess(int x, int y);
	// �ж���Ϸ�Ƿ������ch�����Ի�ȡʤ�����
	bool has_ended(char &ch);
	// �������п��ܵ����Ӵ�
	std::vector<std::pair<int, int>> generate_possible_moves() const;
	// ��������������
	bool player_playing() const { return is_index_0_player_playing; }
};