#include "Search_AI.h"
#include<unordered_map>
using namespace std;
using score_t = unsigned long long;
// �������Ͷ�Ӧ�ĵ÷ֹ�ֵ����
score_t ScoreRefToCnt(int cnt) {
	switch(cnt) {
		case 0:return 1;
		case 1:return 10;
		case 2:return 10000;
		case 3:return 1000000ULL;
		case 4:return 100000000000ULL;
		default:__assume(false); // ÿ����ֻɨ4��cnt�����ܱ�4�󣬸������ܱ�0С
	}
}
void Search_AI::judge(const Five_Chess& fc) {
	judgement = vector<vector<score_t>>(15, vector<score_t>(15, 0)); // ��ʼ���ж�
	max_of_judgement = 0; // ��ʼ�����ֵ
	auto spaces = fc.generate_possible_moves(); // ��ȡ���еĿո�
	for(auto &[x, y] : spaces) { // ��ÿһ���ո���й�ֵ
		// ��ˮƽ����ֱ��б�����ϡ�б�������ĸ�����ķ������м��㣬��������
		for(int i = 0; i < 4; ++i) {
			int dx = i % 3 - 1, dy = i / 3 - 1; // ���㷽��
			int leftcnt = 1, rightcnt = 1, lefthasblock = 0, righthasblock = 0; // �����������ӵĸ����Լ��Ƿ��ڵ�
			if(x + dx < 0 || y + dy < 0 || x + dx > 14 || y + dy > 14) rightcnt = 0; // ������ߣ������һ��Ϊ0
			else if(fc.chessboard[x + dx][y + dy] == ' ') rightcnt = 0; // ������һ���ǿյ�
			else for(int j = 1; j < 5; ++j) { // ɨ5�����е�5���Ŀ���ǿ������Ƿ�Ϊ�з������ӷ�·
				int nx = x + dx * j, ny = y + dy * j; // ����ƫ����
				if(nx >= 0 && nx <= 14 && ny >= 0 && ny <= 14 &&
					nx + dx >= 0 && nx + dx <= 14 && ny + dy >= 0 && ny + dy <= 14) { // ��֤��Խ��
					if(fc.chessboard[nx][ny] == fc.chessboard[nx + dx][ny + dy] && fc.chessboard[nx][ny] != ' ') {
						++rightcnt; // ��ͬ��˵������
					}
					else if(fc.chessboard[nx + dx][ny + dy] != ' ') {
						righthasblock = 1; // ����ͬ��˵����·
						break;
					}
					else break; // �գ��˳�
				}
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
			judgement[x][y] += 
				(leftcnt == 0 ? 1 : 
					(lefthasblock ? 0.4 : 1) * ScoreRefToCnt(leftcnt)) *
				(rightcnt == 0 ? 1 :
					(righthasblock ? 0.4 : 1) * ScoreRefToCnt(rightcnt));
		}
		// �������ֵ
		max_of_judgement = max(max_of_judgement, judgement[x][y]);
	}
}

unsigned long long Search_AI::put(Five_Chess &fc, int depth) {
	// ���ж�
	judge(fc);
	unordered_map<int, int> ht;
	// �ݹ鵽�����ֱ�ӷ���
	if(depth >= maxdepth) {
		score_t best_score = 0ULL;
		for(int i = 0; i < 15; ++i) {
			for(int j = 0; j < 15; ++j) {
				if(judgement[i][j] == max_of_judgement) {
					fc.putchess(i, j);
					judge(fc);
					best_score = max(best_score, max_of_judgement);
					fc.rmchess(i, j);
				}
			}
		}
		return best_score;
	}
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
		judge(fc);
		auto ret = max_of_judgement;
		if(depth != 0) fc.rmchess(i, j); // ֻ�е���Ȳ�Ϊ0ʱ����Ҫ�Ƴ�������
		return ret;
	}
	int _x = -1, _y = -1;
	unsigned long long maxn = 0;
	// ����ÿһ�����ĵ㣬�ݹ������ڼ������ӵ�����£���Ӧ�����ĵ�
	for(auto hashes : ht) {
		auto [i, j] = hashes;
		fc.putchess(i, j);
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
		return static_cast<unsigned long long>(-1);
	}
	else {
		return maxn;
	}
}