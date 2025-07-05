#include "CChess.h"
#include "CChessManager.h"
#include "pch.h"
#include "FiveAI.h"
#include <algorithm> 
#include <vector>
using vector = std::vector<std::vector<int>>;
FiveAI::FiveAI()
{
    this->boardSize = 15;
    mp.push_back(std::vector(18, -1));
    for (int i = 1; i <= 18; ++i) {
        mp.push_back(std::vector(18, -1));
    }
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            mp[i][j] = 0;
}
FiveAI::~FiveAI()
{
}
vector FiveAI::toVector(CChessManager& m_Manager)
{
    CChess* t;
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (t = m_Manager.GetQz(i, j)) {
                if (t->GetColor() == BLACK) {
                    this->mp[i][j] = 1;
                } else {
                    mp[i][j] = 2;
                }
            } else
                mp[i][j] = 0;
        }
    }
    return this->mp;
}
// �������̲������������λ��
std::pair<int, int> FiveAI::findBestMove(CChessManager& m_Manager, int playerColor)
{
    if (m_Manager.m_nChess == 0)
        return { 8, 8 };
    this->toVector(m_Manager);
    int bestScore = INT_MIN;
    std::pair<int, int> bestMove = { -1, -1 };

    // �򵥵��������п��ܵ�����λ��
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            // ���λ���Ƿ����
            if (mp[i][j] == 0) {
                // �����ڴ�λ�����ӣ����������
                // ��������滻Ϊ�����ӵ����ֺ������߼�
                mp[i][j] = playerColor;
                int score = evaluateBoard(playerColor, i, j);
                if (score > SCORE_FIVE)
                    return { i, j };
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = { i, j };
                }
                mp[i][j] = 0;
            }
        }
    }
    return bestMove;
}

// ����������ĳ�����ӵĵ÷�
int FiveAI::evaluateBoard(int player, int x, int y)
{
    int score = 0;
    mp[x][y] = player;
    // ��ÿ����������ֽ����ۼ�
    score += evaluateDirection(player, x, y, 1, 0); // ��ֱ
    score += evaluateDirection(player, x, y, 0, 1); // ˮƽ
    score += evaluateDirection(player, x, y, 1, 1); // �Խ���1
    score += evaluateDirection(player, x, y, 1, -1); // �Խ���2
    // ���ڶ��ֵ���в��������
    int oppoent = player == 1 ? 2 : 1;
    mp[x][y] = oppoent;
    score += evaluateDirection(oppoent, x, y, 1, 0) / 10 * 8; // ��ֱ
    score += evaluateDirection(oppoent, x, y, 0, 1) / 10 * 8; // ˮƽ
    score += evaluateDirection(oppoent, x, y, 1, 1) / 10 * 8; // �Խ���1
    score += evaluateDirection(oppoent, x, y, 1, -1) / 10 * 8; // �Խ���2
    mp[x][y] = 0;
    return score;
}

// ����ָ�������ϵ����Ͳ����ص÷�
int FiveAI::evaluateDirection(int player, int x, int y, int dx, int dy)
{
    int score = 0;
    if (dx == 0) {
        std::vector<int> temp;
        for (int i = 0; i < 15; i += dy)
            temp.push_back(mp[x][i]);
        score += countLine(player, temp);
    } else if (dy == 0) {
        std::vector<int> temp;
        for (int i = 0; i < 15; i += dx)
            temp.push_back(mp[i][y]);
        score += countLine(player, temp);
    } else if (dx == 1 && dy == 1) {
        std::vector<int> temp;
        if (x > y) {
            for (int i = x - y, j = 0; i < 15 && j < 15; ++i, ++j)
                temp.push_back(mp[i][j]);
        } else {
            for (int i = 0, j = y - x; i < 15 && j < 15; ++i, ++j)
                temp.push_back(mp[i][j]);
        }
        score += countLine(player, temp);
    } else if (dx == 1 && dy == -1) {
        std::vector<int> temp;
        if (x + y < 15) {
            for (int i = 0, j = x + y; i <= x + y && j >= 0; ++i, --j)
                temp.push_back(mp[i][j]);
        } else {
            for (int i = x + y - 14, j = 14; i < 15 && j >= 0; ++i, --j)
                temp.push_back(mp[i][j]);
        }
        score += countLine(player, temp);
    }
    return score;
}
bool containsArray(const std::vector<int>& container, const std::vector<int>& toFind)
{
    auto it = std::search(container.begin(), container.end(),
        toFind.begin(), toFind.end());
    return (it != container.end());
}
int FiveAI::countLine(int player, std::vector<int>& v)
{
    int score = 0;
    if (player == 1) {
        for (int i = 0; i < 13; ++i)
            if (containsArray(v, b_Modes[i].pattern))
                score += b_Modes[i].score;
    } else {
        for (int i = 0; i < 13; ++i)
            if (containsArray(v, w_Modes[i].pattern))
                score += w_Modes[i].score;
    }
    return score;
}