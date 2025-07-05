#pragma once
#include <vector>
#include "CChessManager.h"
using vector = std::vector<std::vector<int>>;
struct Mode {
	int score;
	std::vector<int> pattern;
};
class FiveAI {
	const int SCORE_FIVE = 10000000;       // 成五
	const int SCORE_ALIVE_FOUR = 100000;  // 活四
	const int SCORE_RUSH_FOUR = 1000;    // 冲四
	const int SCORE_ALIVE_THREE = 1000;  // 活三
	const int SCORE_SLEEP_THREE = 100;   // 眠三
	const int SCORE_ALIVE_TWO = 100;     // 活二
	const int SCORE_SLEEP_TWO = 10;      // 眠二
	const int SCORE_ONE = 1;
	Mode b_Modes[13] = {
		{1000000000, { 1,1,1,1,1 }},
		{100000, {0,1,1,1,1,0}},
		{10000,{0,1,1,1,1}},
		{10000,{1,1,1,1,0}},
		{1000,{0,1,1,1,0}},
		{100,{1,1,1,0}},
		{100,{0,1,1,1}},
		{100,{0,1,1,0}},
		{10,{0,1,1}},
		{10,{1,1,0}},
		{10,{0,1,0}},
		{1,{0,1}},
		{1,{1,0}}
	};
	Mode w_Modes[13] = {
		{1000000000, { 2,2,2,2,2 }},
		{100000, {0,2,2,2,2,0}},
		{10000,{0,2,2,2,2}},
		{10000,{2,2,2,2,0}},
		{1000,{0,2,2,2,0}},
		{100,{2,2,2,0}},
		{100,{0,2,2,2}},
		{100,{0,2,2,0}},
		{10,{0,2,2}},
		{10,{2,2,0}},
		{10,{0,2,0}},
		{1,{0,2}},
		{1,{2,0}}
	};
public:
	int boardSize;
	vector mp;
	FiveAI();
	~FiveAI();
	vector toVector(CChessManager& m_Manager);
	std::pair<int, int> findBestMove(CChessManager& m_Manager, int playerColor);
	int evaluateBoard(int player, int x, int y);
	int countLine(int player, std::vector<int>& v);
	int evaluateDirection(int player, int x, int y, int dx, int dy);
};