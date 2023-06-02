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

vector<bool> GameProps::_AllPropsIfEffect = {};
vector<clock_t> GameProps::_AllPropsStartTime = {};
GameProps::GameProps() {
}
void GameProps::OnInit() {
	_IfExist = false;
	vector<string> filename;
	for (int i = 0; i < 8; i++) {
		filename.push_back("resources/Prop" + to_string(i) + ".bmp");
	}
	_PropPicture.LoadBitmapByString( filename , RGB(0, 0, 0));
	for (int i = 0;i < 8;i++) {
		_AllPropsIfEffect.push_back(false);
		_AllPropsStartTime.push_back(clock());
	}
}
void GameProps::SetDropProp() {
	_IfExist = true;
	_Type = ItemType(rand() % 8);
	_PropPicture.SetFrameIndexOfBitmap((int)_Type);
	_X = 100 + rand() % 768;
	_Y = rand() % 768;
	_PropPicture.SetTopLeft(_X, _Y);
}
void GameProps::SetPropEffectStart(ItemType Type) {
	_AllPropsIfEffect[int(Type)] = true;
	_AllPropsStartTime[int(Type)] = clock();
}
void GameProps::SetPropIfEffect(ItemType Type, bool Status) {
	_AllPropsIfEffect[int(Type)] = Status;
}
int GameProps::IfEffectExit(ItemType Type) { // 1 is effect,-1 is no effect, 0 is 19second
	int EffectTime = 0;
	int TypeIndex = int(Type);
	if (Type == ItemType::Shovel) {
		EffectTime = 21000;
		if (EffectTime > clock() - _AllPropsStartTime[TypeIndex]
			&& clock() - _AllPropsStartTime[TypeIndex] >= EffectTime - 3000) {
			return 0;
		}
	}
	if (Type == ItemType::Clock || Type == ItemType::Steel_helmet) {
		EffectTime = 10000;
	}
	if (clock() - _AllPropsStartTime[TypeIndex] >= EffectTime) {
		_AllPropsIfEffect[TypeIndex] = false;
		return -1;
	}
	return 1;
}

void GameProps::SetIfExist(bool IfExist) {
	_IfExist = IfExist;
}
void GameProps::ReStartAllProp() {
	fill(_AllPropsIfEffect.begin(), _AllPropsIfEffect.end(), false);
}
vector<bool> GameProps::GetAllPropType() {
	return _AllPropsIfEffect;
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

bool GameProps::GetIfExist() {
	return _IfExist;
}
CMovingBitmap GameProps::GetPropBitmap() {
	return _PropPicture;
}

void GameProps::OnShow() {
	if (_IfExist) {
		_PropPicture.ShowBitmap();
	}
}
