#include "stdafx.h"
#include "../Core/Resource.h"
#include <string>
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "./Menu.h"
#include <string>

using namespace game_framework;

Menu::Menu() {
	tempselect = 0;
	finalselect = 0;
	_MenuType = LobbyMenu;
	_RaiseDelay = 10;
	_RaiseSpeed = 10;
	_IfLobbyMenuing = false;
	_IfSelecting = false;
	_IfChoosingStage = false;
	_IfAnimation = false;
	_IfSettlement = false;
	_Dialog = { {},{},{0},{0},{0} };
}
void Menu::SetMenuType(MenuType Type) {
	switch (Type)
	{
	case LobbyMenu:
		_MenuType = LobbyMenu;
		return;
	case ChooseStageMenu:
		_MenuType = ChooseStageMenu;
		return;
	case BattleMenu:
		_MenuType = BattleMenu;
		return;
	case SettleMenu:
		_MenuType = SettleMenu;
		return;
	}
}
void Menu::SetSelecting(bool Status) {
	_IfSelecting = Status;
}
void Menu::SetIfLobbyMenuing(bool Status) {
	_IfLobbyMenuing = Status;
}
void Menu::SetIfChoosingStage(bool Status) {
	_IfChoosingStage = Status;
}
void Menu::SetIfAnimation(bool Status) {
	_IfAnimation = Status;
}
void Menu::SetSettlement(vector<int>& EnemyNum, vector<int>& EnemyScore, int& NowScore, int& THeHighestScore,int& NowStage) {
	_IfSettlement = true;
	_IfAnimation = true;
	_CountNumber = {0,0,0,0};
	_NowCountTank = 0;
	_Dialog[0] = EnemyNum;
	_Dialog[1] = EnemyScore;
	_Dialog[2] = { NowStage };
	_Dialog[3] = { NowScore };
	_Dialog[4] = { THeHighestScore };
}
void Menu::SetMenuRaise(MenuType Type) {
	_MenuType = Type;
	if (clock() - _Last_time >= _RaiseDelay) {
		if (_MenuType == LobbyMenu && _IfAnimation) {
			_IfLobbyMenuing = true;
			_Menu.SetTopLeft(100, _Menu.GetTop() - _RaiseSpeed);
			_Last_time = clock();
		}
		else if (_MenuType == ChooseStageMenu) {
			_IfChoosingStage = true;
			_ChooseStageMenuTop.SetTopLeft(0, _ChooseStageMenuTop.GetTop() + _RaiseSpeed + 15);
			_ChooseStageMenuDown.SetTopLeft(0, _ChooseStageMenuDown.GetTop() - _RaiseSpeed - 15);
			_Last_time = clock();
		}
	}
}

bool Menu::GetIfSelecting() {
	return _IfSelecting;
}
bool Menu::GetIfLobbyMenuing() {
	return _IfLobbyMenuing;
}
bool Menu::GetChoosingStage() {
	return _IfChoosingStage;
}
bool Menu::GetIfAnimation() {
	return _IfAnimation;
}
int Menu::GetMenuY(MenuType Type) {
	if (Type == LobbyMenu) {
		return _Menu.GetTop();
	}
	else if (Type == ChooseStageMenu) {
		return _ChooseStageMenuTop.GetTop();
	}
	return 0;
}



int Menu::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	const char KEY_UP = 0x26;
	const char KEY_DOWN = 0x28;
	const char KEY_ENTER = 0x0D;
	const char Key_Z = 0x5A;
	const char Key_X = 0x58;

	if (_IfAnimation && _MenuType == LobbyMenu) { // return -1 is still same,not change.
		_Menu.SetTopLeft(100, 0);
		_IfAnimation = false;
		return -1;
	}
	if (_IfSelecting && _MenuType == LobbyMenu) { // we select the stage in there.
		if (nChar == KEY_DOWN) {
			tempselect += 1;
			tempselect %= 2;
		}
		else if (nChar == KEY_UP) {
			tempselect -= 1;
			tempselect += 2;
			tempselect %= 2;
		}
		else if (nChar == KEY_ENTER) {
			_ChooseStageMenuTop.SetTopLeft(0, -450);
			_ChooseStageMenuDown.SetTopLeft(0, 900);
			_IfSelecting = false;
			_IfLobbyMenuing = false;
			_IfAnimation = false;
			_Menu.SetTopLeft(100, 900);
			finalselect = tempselect;
			tempselect = 0;
			return finalselect;
		}
		_Arrow.SetTopLeft(380, 500 + 70 * tempselect);
		return -1; // return -1 is still same.
	}
	else if (_IfSelecting && _MenuType == ChooseStageMenu) { // IfSelecting is after in graymenu animation.
		if (nChar == Key_Z) {
			tempselect -= 1;
			if (tempselect < 0) {
				tempselect += 1;
			}
		}
		else if (nChar == Key_X) {
			tempselect += 1;
			if (tempselect > 34) {
				tempselect -= 1;
			}
		}
		else if (nChar == KEY_ENTER) {
			_IfChoosingStage = false;
			_IfAnimation = false;
			_IfSelecting = false;
			finalselect = tempselect + 1;
			return finalselect;
		}
		return -1; // return -1 is still same.
	}
	else if (_IfSettlement && _MenuType == SettleMenu && !_IfAnimation) {
		if (nChar == KEY_ENTER) {
			_IfSettlement = false;
			return 1;
		}
		return -1;
	}
	return -1;
}

void Menu::LoadBitMap() {
	_Menu.LoadBitmapByString({ "resources/LobbyMenu.bmp" });
	_Menu.SetTopLeft(100, 900);
	_Arrow.LoadBitmapByString({ "resources/LobbyArrow.bmp" });
	_Arrow.SetTopLeft(380, 500);
	_BattleMenuGray.LoadBitmapByString({ "resources/GrayScreen_Background.bmp" });
	_BattleMenuGray.SetTopLeft(0, 0);
	_ChooseStageMenuTop.LoadBitmapByString({ "resources/GrayScreen.bmp" });
	_ChooseStageMenuTop.SetTopLeft(0, -450);
	_ChooseStageMenuDown.LoadBitmapByString({ "resources/GrayScreen.bmp" });
	_ChooseStageMenuDown.SetTopLeft(0, 900);
	_Settlementdivided.LoadBitmapByString({ "resources/Settlementdivided.bmp" }, RGB(0, 0, 0));
	_Settlementdivided.SetTopLeft(440, 680);

	for (int i = 0; i < 4; i++) {
		CMovingBitmap temp1,temp2;
		string path;
		temp1.LoadBitmapByString({ "resources/SettleArrow.bmp" }, RGB(0, 0, 0));
		_SettleArrow.push_back(temp1);
		_SettleArrow[i].SetTopLeft(520, 275 + i * 100);
		if (i == 0) {
			path = "resources/Enemy_LightTank_Top2.bmp";
		}
		else if (i == 1) {
			path = "resources/Enemy_ArmorTank_Top2.bmp";
		}
		else if (i == 2) {
			path = "resources/Enemy_QuickTank_Top2.bmp";
		}
		else {
			path = "resources/Enemy_HeavyTank_Top2.bmp";
		}
		temp2.LoadBitmapByString({ path }, RGB(0, 0, 0));
		_SettlementMenuEnemy.push_back(temp2);
		_SettlementMenuEnemy[i].SetTopLeft(640, 300 + i * 100);
	}
	
}
void Menu::OnShow() {

	if (_MenuType == LobbyMenu) {
		if (_IfLobbyMenuing) {
			_Menu.ShowBitmap();
		}
		if (_IfSelecting) {
			_Arrow.ShowBitmap();
		}
	}
	if (_MenuType == ChooseStageMenu) {
		if (_IfChoosingStage) {
			_ChooseStageMenuTop.ShowBitmap();
			_ChooseStageMenuDown.ShowBitmap();
		}
	}
	if (_MenuType == BattleMenu) {
		_BattleMenuGray.ShowBitmap();
	}
	if (_MenuType == SettleMenu) {
		for (int i = 0; i < 4; i++) {
			_SettleArrow[i].ShowBitmap();
			_SettlementMenuEnemy[i].ShowBitmap();
			_Settlementdivided.ShowBitmap();
		}
	}
}
void Menu::OnShowText(CDC *pDC, CFont* &fp) {

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(180, 180, 180));
	CTextDraw::ChangeFontLog(pDC, 40, "STZhongsong", RGB(180, 180, 180));
	if (_IfLobbyMenuing && _IfSelecting) {
		CTextDraw::Print(pDC, 200, 60, to_string(_Dialog[2][0]).c_str());
	}
	if (_IfChoosingStage && _IfSelecting) {
		CTextDraw::Print(pDC, 600, 400, string("Stage") + to_string(tempselect+1).c_str());
	}
	
	if (_IfSettlement) {
		if (_NowCountTank <= 3) {
			if (_CountNumber[_NowCountTank] < _Dialog[0][_NowCountTank]) {
				if (clock() - _Last_time >= 250) {
					_CountNumber[_NowCountTank]++;
					_Last_time = clock();
				}
			}
			else {
				if (clock() - _Last_time >= 250 && _NowCountTank != 3) {
					_NowCountTank++;
					_Last_time = clock();
				}
				if (_IfAnimation == true) {
					_IfAnimation = false;
				}
			}
		}
		for (int i = 0; i <= _NowCountTank; i++) {
			pDC->SetTextColor(RGB(161, 59, 6));
			CTextDraw::Print(pDC, 400, 60, string("HI-Score    ")); // turn to color red
			CTextDraw::Print(pDC, 180, 160, string("I-Player"));

			pDC->SetTextColor(RGB(184, 120, 5));
			CTextDraw::Print(pDC, 655, 60, to_string(_Dialog[4][0])); // turn to color orange
			CTextDraw::Print(pDC, 180, 210, to_string(_Dialog[3][0]));

			pDC->SetTextColor(RGB(180, 180, 180));
			CTextDraw::Print(pDC, 500, 110, string("Stage")); //turn to color white
			CTextDraw::Print(pDC, 655, 110, to_string(_Dialog[2][0])); 
			CTextDraw::Print(pDC, 440, 300 + i * 100, to_string(_CountNumber[i]));
			CTextDraw::Print(pDC, 180, 300 + i * 100, to_string(_Dialog[1][i] * _CountNumber[i]));
			CTextDraw::Print(pDC, 310, 300 + i * 100, string("PTS"));
			CTextDraw::Print(pDC, 320, 720, string("TOTAL"));
		}
	}
}
