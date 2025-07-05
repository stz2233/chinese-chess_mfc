#include "pch.h"
#include "CChessManager.h"
#include <afxwin.h>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
CChessManager::CChessManager() {
}
CChessManager::~CChessManager() {
}
bool CChessManager::Xy2Xy(int x0, int y0, int& x1, int& y1) { //x0,y0为鼠标点击的坐标，x1,y1为棋盘上的坐标
	int x, y;
	for (int i = 0;i < 15;i++)
		for (int j = 0; j < 15; j++) {
			x = CChess::m_dx + i * CChess::m_d + CChess::m_d * 0.5;
			y = CChess::m_dy + j * CChess::m_d + CChess::m_d * 0.5;
			if (sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0)) < 15) {
				x1 = i, y1 = j;
				return true;
			}
		}
	return false;
}
int CChessManager::Add(int x, int y) {	// x,y为鼠标点击的坐标
	int x1, y1;
	if (!Xy2Xy(x, y, x1, y1))
		return 1;
	for (int i = 0; i < m_nChess; i++)
		if (x1 == m_aChess[i].GetX() && y1 == m_aChess[i].GetY())
			return 2;
	m_aChess[m_nChess].Set(m_nChess, x1, y1, m_Color);	// m_Color为当前落子的颜色
	m_nChess++;
	m_Color = (m_Color == WHITE ? BLACK : WHITE);
	return 0;
}
COLOR CChessManager::GetCurrentColor() {
	return m_Color;
}
int CChessManager::Add_Org(int x, int y) {
	int x1 = x, y1 = y;
	for (int i = 0; i < m_nChess; i++)
		if (x1 == m_aChess[i].GetX() && y1 == m_aChess[i].GetY())
			return 2;
	m_aChess[m_nChess].Set(m_nChess, x1, y1, m_Color);
	m_nChess++;
	m_Color = (m_Color == WHITE ? BLACK : WHITE);
	return 0;
}
bool CChessManager::LoadArchives(CString FilePath = _T(""))	//	加载存档
{
	try
	{
        std::ifstream i(FilePath.GetString());
		json j;
        j = json::parse(i);
		m_nChess = j["m_nChess"].get<int>();
		m_Color = j["m_Color"].get<COLOR>();
		org_Color = j["org_Color"].get<COLOR>();
		for (int i = 0; i < m_nChess; i++) {
			m_aChess[i].Set(j["m_aChess"][i]["m_nIndex"], 
			j["m_aChess"][i]["m_x"], j["m_aChess"][i]["m_y"], 
			j["m_aChess"][i]["m_Color"]);
		}
		return true;
	}
    catch (const std::exception& e)
    {
        std::string str = e.what();
        MessageBoxA(NULL, str.c_str(), "Error", MB_OK);
        return false;
    }
}
bool CChessManager::SaveArchives(CString FilePath /*= _T("")*/)
{
	try
	{
		json j;
		j["m_nChess"] = m_nChess;
		j["m_Color"] = m_Color;
		j["org_Color"] = org_Color;
		for (int i = 0; i < m_nChess; i++) {
			j["m_aChess"][i]["m_nIndex"] = i;
			j["m_aChess"][i]["m_x"] = m_aChess[i].GetX();
			j["m_aChess"][i]["m_y"] = m_aChess[i].GetY();
			j["m_aChess"][i]["m_Color"] = m_aChess[i].GetColor();
		}
		std::ofstream o(FilePath.GetString());
        o << j.dump(4);
		return true;
	}
	catch (const std::exception& e)
	{
		std::string str = e.what();
		MessageBoxA(NULL, str.c_str(), "Error", MB_OK);
		return false;
	}
}
void CChessManager::Repentance() {
	if (m_nChess > 0) {
		m_nChess--;
		m_Color = (m_Color == WHITE ? BLACK : WHITE);
	}
}
void CChessManager::Show(CDC* pDC) {
	for (int i = 0; i < m_nChess; i++)
		m_aChess[i].Show(pDC);
}
bool CChessManager::GameOver() {
	if (CheckRows())
		return true;
	if (CheckCols())
		return true;
	if (CheckLSlash())
		return true;
	if (CheckRSlash())
		return true;
	return false;
}
CChess* CChessManager::GetQz(int x, int y) {
	for (int i = 0; i < m_nChess; i++)
		if (m_aChess[i].GetX() == x && m_aChess[i].GetY() == y)
			return &m_aChess[i];
	return nullptr;
}
bool CChessManager::CheckRows() {
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0;i < MAX_ROWS;i++)
	{
		iCount = 0;
		for (int j = 0; j < MAX_COLS; j++)
			if (pQz = GetQz(j, i)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}
bool CChessManager::CheckCols() {
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0; i < MAX_COLS; i++) {
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i, j)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}
bool CChessManager::CheckLSlash() {
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = -14; i < MAX_COLS; i++) {
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i + j, j)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}
bool CChessManager::CheckRSlash()
{
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0; i < MAX_COLS + 14; i++) {
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i - j, j)) {
				if (iCount == 0) {
					color = pQz->GetColor();
					iCount++;
				}
				else if (color == pQz->GetColor()) {
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else {
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}