#include "Search_AI.h"
#include<unordered_map>
#ifdef _DEBUG
#include <iostream>
#include <iomanip>
// �ָ���
constexpr char separator[] = "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
#endif
using namespace std;
#pragma warning(disable: 4244) // doubleתscore_t���ܻ��о�����ʧ���������ﲻ��Ҫ����
// �������Ͷ�Ӧ�ĵ÷ֹ�ֵ����
inline Search_AI::score_t ScoreRefToCnt(int cnt) {
	switch(cnt) {
		case 0:return 1;
		case 1:return 100;
		case 2:return 100000;
		case 3:return 100000000ULL;
		case 4:return 10000000000000ULL;
		default:__assume(false); // ÿ����ֻɨ4��cnt�����ܱ�4�󣬸������ܱ�0С
	}
}

void Search_AI::judge(const Five_Chess &fc) noexcept {
	judgement = vector<vector<score_t>>(15, vector<score_t>(15, 0)); // ��ʼ���ж�
	max_of_judgement = 0; // ��ʼ�����ֵ
	auto spaces = fc.generate_possible_moves(); // ��ȡ���еĿո�
	for(auto &[x, y] : spaces) { // ��ÿһ���ո���й�ֵ
		// ��ˮƽ����ֱ��б�����ϡ�б�������ĸ�����ķ������м��㣬��������
		for(int i = 0; i < 4; ++i) {
			int dx = i % 3 - 1, dy = i / 3 - 1; // ���㷽��
			int leftcnt = 1, rightcnt = 1, lefthasblock = 0, righthasblock = 0; // �����������ӵĸ����Լ��Ƿ��ڵ�
			char leftrecord = ' ', rightrecord = ' '; // ��¼�������ߵ�����
			if(x + dx < 0 || y + dy < 0 || x + dx > 14 || y + dy > 14) rightcnt = 0; // ������ߣ������һ��Ϊ0
			else if(fc.chessboard[x + dx][y + dy] == ' ') rightcnt = 0; // ������һ���ǿյ�
			else for(int j = 1; j < 5; ++j) { // ɨ4��
				int nx = x + dx * j, ny = y + dy * j; // ����ƫ����
				if(nx >= 0 && nx <= 14 && ny >= 0 && ny <= 14 &&
					nx + dx >= 0 && nx + dx <= 14 && ny + dy >= 0 && ny + dy <= 14) { // ��֤��Խ��
					if(fc.chessboard[nx][ny] == fc.chessboard[nx + dx][ny + dy] && fc.chessboard[nx][ny] != ' ') {
						rightrecord = fc.chessboard[nx][ny];
						++rightcnt; // ��ͬ��˵������
					}
					else if(fc.chessboard[nx + dx][ny + dy] != ' ') {
						righthasblock = 1; // ����ͬ��˵����·
						break;
					}
					else break; // �գ��˳�
				}
				// ���Խ�磬�������²��˵�
				else {
					righthasblock = 1;
					break;
				}
			}
			// ͬ���߼�
			if(x - dx < 0 || y - dy < 0 || x - dx > 14 || y - dy > 14) leftcnt = 0;
			else if(fc.chessboard[x - dx][y - dy] == ' ') leftcnt = 0;
			else for(int j = 1; j < 5; ++j) {
				int nx2 = x - dx * j, ny2 = y - dy * j;
				if(nx2 > 0 && nx2 < 15 && ny2 > 0 && ny2 < 15 &&
					nx2 - dx > 0 && nx2 - dx < 15 && ny2 - dy > 0 && ny2 - dy < 15) {
					if(fc.chessboard[nx2][ny2] == fc.chessboard[nx2 - dx][ny2 - dy] &&
						fc.chessboard[nx2][ny2] != ' ') {
						leftrecord = fc.chessboard[nx2][ny2];
						++leftcnt;
					}
					else if(fc.chessboard[nx2 - dx][ny2 - dy] != ' ') {
						lefthasblock = 1;
						break;
					}
					else break;
				}
				else {
					lefthasblock = 1;
					break;
				}
			}
			// ����õ�ĵ÷�
			// ������߶��з�·������С��4�����ﲻ���������������飬ֱ������
			if(lefthasblock && righthasblock && leftcnt + rightcnt < 4) {
				continue;
			}
			// ������߾���Ϊ���Ҳ���ȣ���ÿһ�߾�С��4�����ﲻ���������������飬ֱ������
			else if(leftrecord != ' ' && rightrecord != ' ' &&
				leftrecord != rightrecord && leftcnt < 4 && rightcnt < 4) {
				continue;
			}
			else if(leftcnt + rightcnt >= 4 && leftrecord == rightrecord) {
				judgement[x][y] += 1000000000000000ULL; // ��һ�����ӵ��˴���ʤ
			}
			else {
				judgement[x][y] +=
					(lefthasblock ? 0.1 : 1.) * ScoreRefToCnt(leftcnt) *
					(righthasblock ? 0.1 : 1.) * ScoreRefToCnt(rightcnt);
			}
		}
		// �������ֵ
		max_of_judgement = max(max_of_judgement, judgement[x][y]);
	}
}

Search_AI::score_t Search_AI::put(Five_Chess &fc, int depth) noexcept {
	// ���ж�
	judge(fc);
	unordered_map<int, int> ht;
	// �ݹ鵽�����ֱ�ӷ���
	if(depth >= maxdepth) {
		return max_of_judgement;
	}
	// �����ֵΪ0ʱ��˵����Ϸ��Ϊƽ��
	if(max_of_judgement == 0) {
		auto [x, y] = fc.generate_possible_moves()[0];
		fc.putchess(x, y);
		return 0ULL;
	}
	else {
		// ��ȡ�����������е�
		for(int i = 0; i < 15; ++i) {
			for(int j = 0; j < 15; ++j) {
				if(judgement[i][j] == max_of_judgement) {
					ht[i] = j;
				}
			}
		}
		// ������ĵ�ֻ��һ����ֱ�ӷ���
		if(ht.size() == 1) {
			auto [i, j] = *(ht.begin());
			fc.putchess(i, j);
			// ���У���Ϸ�Ƿ����
			char ch;
			if(fc.has_ended(ch)) {
				if(depth != 0) fc.rmchess(i, j); // ֻ�е���Ȳ�Ϊ0ʱ����Ҫ�Ƴ�������
				return static_cast<score_t>(-1);
			}
			judge(fc);
			auto ret = max_of_judgement;
			if(depth != 0) fc.rmchess(i, j);
			else return static_cast<score_t>(-1);
			return ret;
		}
		int _x = -1, _y = -1;
		unsigned long long maxn = 0;
		// ����ÿһ�����ĵ㣬�ݹ������ڼ������ӵ�����£���Ӧ�����ĵ�
		for(auto hashes : ht) {
			auto [i, j] = hashes;
			fc.putchess(i, j);
			char ch;
			if(fc.has_ended(ch)) {
				if(depth != 0) {
					fc.rmchess(i, j);
				}
				return static_cast<score_t>(-1);
			}
			judge(fc);
			auto retthis = put(fc, depth + 1);
			if(maxn < retthis) {
				maxn = retthis;
				_x = i, _y = j;
			}
			fc.rmchess(i, j);
		}
		// ������Ϊ0������������
		if(depth == 0) {
			fc.putchess(_x, _y);
			return static_cast<score_t>(-1);
		}
		else {
			return maxn;
		}
	}
}

void Search_AI::put(Five_Chess &fc) noexcept {
	put(fc, 0);
#ifdef _DEBUG
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			cout << setw(16) << judgement[j][i] << "\t";
		}
		cout << "\n\n";
	}
	cout << separator;
#endif
}