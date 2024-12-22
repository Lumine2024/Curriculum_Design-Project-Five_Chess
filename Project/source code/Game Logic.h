#pragma once
#include "Common.h"
#include "Player_Base.h"
#include <algorithm>
#include <utility>
#include <vector>
#pragma warning(disable: 5030) // 无法识别属性：msvc::always_inline
// 五子棋棋盘类
class Five_Chess {
public:
	// 棋盘
	std::vector<std::vector<char>> chessboard;
	// 是否为白子正在操作
	bool white_player_playing;
	Five_Chess() :
		chessboard(15, std::vector<char>(15, ' ')),
		white_player_playing(false) {}
	Five_Chess(const Five_Chess &other) :
		chessboard(other.chessboard),
		white_player_playing(other.white_player_playing) {}
	Five_Chess &operator=(const Five_Chess &other) {
		if(this != &other) {
			chessboard = other.chessboard;
			white_player_playing = other.white_player_playing;
		}
		return *this;
	}
	// 落子
	RELEASE_INLINE void putchess(int x, int y);
	// 移除棋子
	RELEASE_INLINE void rmchess(int x, int y) noexcept;
	// 判定游戏是否结束，ch传入以获取胜利玩家
	RELEASE_INLINE bool has_ended(char &ch) const noexcept;
	// 生成所有可能的落子处
	RELEASE_INLINE std::vector<std::pair<int, int>> generate_possible_moves() const noexcept;
	// 返回正在玩的玩家，true为白子
	RELEASE_INLINE bool player_playing() const noexcept {
		return white_player_playing;
	}
};

// 进行游戏的函数
// 第四个参数为落子后是否等待500ms，这是为了防止AI自我对弈时落子过快无法看清
void playgame(Five_Chess &fc,
	Player_Base *player1, Player_Base *player2,
	bool dosleep = false) noexcept;

// 人类操作
class Human : public Player_Base {
public:
	Human() noexcept = default;
	// 人类的判断可由人类自行完成，故提供空实现
	void judge(const Five_Chess &fc) noexcept override {}
	// 获取鼠标信息，点击时落子
	void put(Five_Chess &fc) noexcept override;
};