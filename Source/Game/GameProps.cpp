#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "./GameProps.h"
#include <string>
#include <random>

using namespace game_framework;

vector<GameProps::ItemType> GameProps::_AllPropType = {};
GameProps::GameProps() {
}
void GameProps::OnInit() {
	_IfShow = false;
	_IfExist = false;
	_IfCountDown = false;
	_IfTouched = false;
	_Type = ItemType::Empty;
	vector<string> filename;
	for (int i = 0; i < 8; i++) {
		filename.push_back("resources/Prop" + to_string(i) + ".bmp");
	}
	_Props.LoadBitmapByString( filename , RGB(0, 0, 0));
}
void GameProps::SetGameProps() {
	_IfShow = true;
	_IfExist = true;
	_IfCountDown = false;
	_IfTouched = false;
	_Type = ItemType(rand() % 8);
	_Props.SetFrameIndexOfBitmap((int)_Type);
	_X = 100 + rand() % 768;
	_Y = rand() % 768;
	_Props.SetTopLeft(_X,_Y);
	_AllPropType.push_back(_Type);
	_NowIndex = _AllPropType.size() - 1;
}
int GameProps::count(ItemType Type) {
	int sum = 0;
	for (int i = 0; i < (int)_AllPropType.size(); i++) {
		if (_AllPropType[i] == Type) {
			sum += 1;
		}
	}
	return sum;
}
int GameProps::find(ItemType Type) {
	for (int i = 0; i < (int)_AllPropType.size(); i++) {
		if (_AllPropType[i] == Type) {
			return i;
		}
	}
	return -1;
}
int GameProps::IfEffectExit() { // 1 is effect,-1 is no effect, 0 is 19second
	int EffectTime = 0;
	if (!_IfCountDown) {
		_IfCountDown = true;
		_IfTouched = true;
		_StartTime = clock();
	}
	if (_Type == ItemType::Shovel) {
		EffectTime = 21000;
	}
	if (_Type == ItemType::Clock || _Type == ItemType::Steel_helmet) {
		EffectTime = 10000;
	}
	if (_Type == ItemType::Shovel && EffectTime > clock() - _StartTime && clock() - _StartTime >= EffectTime - 3000) {
		return 0;
	}
	if (clock() - _StartTime >= EffectTime) {
		_IfCountDown = false;
		_IfTouched = false;
		_IfExist = false;
		_Type = ItemType::Empty;
		_AllPropType[_NowIndex] = ItemType::Empty;
		return -1;
	}
	return 1;
}

void GameProps::SetIfExist(bool IfExist) {
	_IfExist = IfExist;
	if (_IfExist == false) {
		_Type = ItemType::Empty;
		_AllPropType[_NowIndex] = ItemType::Empty;
	}
}
void GameProps::SetIfShow(bool Status) {
	_IfShow = Status;
}
void GameProps::SetIfCountDown(bool State) {
	_IfCountDown = State;
}
void GameProps::ReStartAllProp() {
	_AllPropType.clear();
}

vector<GameProps::ItemType> GameProps::GetAllPropType() {
	return _AllPropType;
}
bool GameProps::GetIfTouched() {
	return _IfTouched;
}
GameProps::ItemType GameProps::GetType() {
	return _Type;
}
int GameProps::GetX() {
	return _X;
}
int GameProps::GetY() {
	return _Y;
}

bool GameProps::GetIfShow() {
	return _IfShow;
}

bool GameProps::GetIfExist() {
	return _IfExist;
}
CMovingBitmap GameProps::GetPropBitmap() {
	return _Props;
}

void GameProps::OnShow() {
	if (_IfShow && _IfExist) {
		_Props.ShowBitmap();
	}
}

