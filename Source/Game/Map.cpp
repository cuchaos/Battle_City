#include <vector>
#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "./MapItem.h"
#include "./Map.h"

using namespace game_framework;

Map::Map() {
}
void Map::OnInit(vector<vector<int>> stage) {
	_IfShowMap = false;
	_IfGetShovel = false;
	_IfShovelShine = false;
	_IfGrassInMap = false;
	_BlackGrid.LoadBitmapByString({ "resources/BlackGrid.bmp" }, RGB(105, 105, 105));
	_BlackGrid.SetTopLeft(100, 0);
	for (int i = 0; i < 26; i++) {
		vector<MapItem> temp;
		for (int j = 0; j < 26; j++) {
			temp.push_back(MapItem(stage[i][j]));
			if (stage[i][j] != 1 && stage[i][j] != 7) {
				temp[j].SetTopLeft(j * 32 + 100, i * 32);
			}
			if (stage[i][j] == 6) {
				_IfGrassInMap = true;
			}
		}
		_Stage.push_back(temp);
	}
	for (int i = 0; i < 20; i++) {
		CMovingBitmap temp;
		temp.LoadBitmapByString({ "resources/BattleMenuCount.bmp" }, RGB(105, 105, 105));
		_EnemySign.push_back(temp);
		if (i % 2 == 0) {
			_EnemySign[i].SetTopLeft(1010,i * 20);
		}
		else {
			_EnemySign[i].SetTopLeft(1080, _EnemySign[i-1].GetTop());
		}
	}
	CMovingBitmap temp1, temp2; // give to playersign
	_OnePlayerHealth.LoadBitmapByString({ "resources/BattleMenu1PHealth.bmp" }, RGB(105, 105, 105));
	_OnePlayerHealth.SetTopLeft(1010, 480);
	_SecondPlayerHealth.LoadBitmapByString({ "resources/BattleMenu2PHealth.bmp" }, RGB(105, 105, 105));
	_SecondPlayerHealth.SetTopLeft(1010, 580);
	_BattleMenuFlag.LoadBitmapByString({ "resources/BattleMenuFlag.bmp" }, RGB(105, 105, 105));
	_BattleMenuFlag.SetTopLeft(1000, 700);

	temp1.LoadBitmapByString({ "resources/BattleMenuHealthSign.bmp" }, RGB(105, 105, 105));
	temp2.LoadBitmapByString({ "resources/BattleMenuHealthSign.bmp" }, RGB(105, 105, 105));
	
	_PlayerSign.push_back(temp1);
	_PlayerSign.push_back(temp2);
	_PlayerSign[0].SetTopLeft(1010, 520);
	_PlayerSign[1].SetTopLeft(1010, 620);
}
void Map::ShootWall(int Direction, int TankLevel, int x, int y) {
	_Stage[x][y].ChangeGridState(Direction, TankLevel);
}

int Map::GetEnemySignNum() {
	return _EnemySign.size();
}
bool Map::GetMapItemInfo(int x, int y,InfoType GridAttribute) 
{
	switch (GridAttribute)
	{
	case CanWalk:
		return _Stage[x][y].GetIfWalk();
	case CanShoot:
		return _Stage[x][y].GetIfShoot();
	case CanBreak:
		return _Stage[x][y].GetIfBreak();
	}
	return true;
}
int Map::GetType(int x, int y) {
	return _Stage[x][y].GetType();
}
bool Map::GetIfShowMap() {
	return _IfShowMap;
}
bool Map::GetIfGrassInMap() {
	return _IfGrassInMap;
}
vector<vector<int>> Map::GetFrontGridsIndex(vector<vector<int>> GridXY) { // u should give the two of pixel xy in front of your object
	vector<vector<int>> FrontIndex = { {0,0},{0,0} };
	FrontIndex[0][0] = (GridXY[0][0] - 100)/ 32;
	FrontIndex[0][1] = GridXY[0][1] / 32;

	FrontIndex[1][0] = (GridXY[1][0] - 100) / 32;
	FrontIndex[1][1] = GridXY[1][1] / 32;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			if (FrontIndex[i][j] < 0) FrontIndex[i][j] = 0;
			else if (FrontIndex[i][j] > 25) FrontIndex[i][j] = 25;
		}
	}
	return FrontIndex;
}
bool Map::GetIfBoardEdge(int Nowx, int Nowy,int NowHeight,int NowWidth,int NowDirection) {
	if (NowDirection == Right) {
		if (Nowx + NowWidth >= 932) {
			return false;
		}
	}
	else if (NowDirection == Down) {
		if (Nowy + NowHeight>= 832) {
			return false;
		}
	}
	else if (NowDirection == Left) {
		if (Nowx <= 100) {
			return false;
		}
	}
	else if (NowDirection == Up) {
		if (Nowy <= 0) {
			return false;
		}
	}
	return true;
}

void Map::SetEnemySignPop() {
	_EnemySign.pop_back();
}
void Map::SetIfShowMap(bool Status) {
	_IfShowMap = Status;
}
void Map::SetGetShovel(int EffectTime) {
	int Type = 0;
	bool IfShine = false;
	if (EffectTime == 1) { // EffectTime >= 0 mean the effect is exist
		Type = 5;
	}
	else if (EffectTime == 0 ) {
		Type = 5;
		IfShine = true;
	}
	else if(EffectTime == -1){
		Type = 4;
	}
	_Stage[25][11].SetShovelChangeType(Type, IfShine);
	_Stage[24][11].SetShovelChangeType(Type, IfShine);
	_Stage[23][11].SetShovelChangeType(Type, IfShine);
	_Stage[23][12].SetShovelChangeType(Type, IfShine);
	_Stage[23][13].SetShovelChangeType(Type, IfShine);
	_Stage[23][14].SetShovelChangeType(Type, IfShine);
	_Stage[24][14].SetShovelChangeType(Type, IfShine);
	_Stage[25][14].SetShovelChangeType(Type, IfShine);
}

void Map::OnShowGrass() {
	if (_IfShowMap) {
		for (int i = 0; i < 26; i++) {
			for (int j = 0; j < 26; j++) {
				_Stage[i][j].GrassOnsShow();
			}
		}
	}
}
void Map::OnShow() {
	if (_IfShowMap) {
		_BlackGrid.ShowBitmap();
		for (int i = 0; i < 26; i++) {
			for (int j = 0; j < 26; j++) {
				_Stage[i][j].OnShow();
			}
		}
		for (int i = 0; i < (int)_EnemySign.size(); i++) {
			_EnemySign[i].ShowBitmap();
		}
		_OnePlayerHealth.ShowBitmap();
		_SecondPlayerHealth.ShowBitmap();
		for (int i = 0; i < 2; i++) {
			_PlayerSign[i].ShowBitmap();
		}
		_BattleMenuFlag.ShowBitmap();
	}
}
