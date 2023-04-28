﻿#include <vector>
#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include "CEvent.h"

// 地圖物件發生事件
using namespace game_framework;

Event::Event() {
	_IfStart = false;
}

void Event::TrigLobbyMenu(Menu& LobbyMenu) {
	if (LobbyMenu.GetMenuY(LobbyMenu.LobbyMenu) > 0) {
		LobbyMenu.SetIfAnimation(true);
		LobbyMenu.SetMenuRaise(LobbyMenu.LobbyMenu);
	}
	else {
		LobbyMenu.SetSelecting(true);
		LobbyMenu.SetIfAnimation(false);
	}
}
void Event::TrigSelectingStage(Menu& GrayScreen) {
	if (GrayScreen.GetMenuY(GrayScreen.ChooseStageMenu) < 0) {
		GrayScreen.SetMenuRaise(GrayScreen.ChooseStageMenu);
	}
	else {
		GrayScreen.SetSelecting(true);
		GrayScreen.SetIfAnimation(false);
	}
}
void Event::TrigUpDateMap(Map& StageMap, int& EnemyNum) {
	if (StageMap.GetEnemySignNum() > EnemyNum) {
		StageMap.SetEnemySignPop();
	}
}
void Event::TrigSetBattleMap(vector<vector<int>>& Stage,Map& StageMap,int& EnemyNum, Menu& BattleMenu, CPlayer& Player, vector<GameProps>& Props) {
	if (Props.size() != 0) {
		Props[0].ReStartAllProp();
	}
	StageMap.OnInit(Stage);
	StageMap.SetIfShowMap(true);
	BattleMenu.SetMenuType(BattleMenu.BattleMenu);
	EnemyNum = 20;
	Player.PlayerInit();
	Player.SetIfBattle(true);
}
void Event::TrigSettlement(Menu& SettlementMenu, vector<int>& StageEnemy, int& NowScore,int& TheHighestScore,int& NowStage) {
	vector<int> EnemyScore = { 100,200,300,400 };
	SettlementMenu.SetMenuType(SettlementMenu.SettleMenu);
	SettlementMenu.SetSettlement(StageEnemy, EnemyScore, NowScore, TheHighestScore,NowStage);
}
void Event::TrigReSetProps(vector<GameProps>& Props) {
	int _LastProps = Props[0].GetAllPropType().size() - 1;
	if (_LastProps+1 >= 1) {
		if (!Props[_LastProps].GetIfTouched() && Props[_LastProps].GetIfExist()) {
			Props[_LastProps].SetIfExist(false);
		}
	}
}
void Event::TrigNextStage(Map& StageMap, Menu& BattleMenu, int& EnemyNum, int& NowStage) {

}
void Event::TrigSetProps(vector<GameProps>& Props,int NowPropIndex) {
	Props[NowPropIndex].SetGameProps();
}
void Event::TrigGetProps(GameProps& Props,Map& StageMap,CPlayer& Player,vector<Enemy>& AllEnemy) {
	Props.SetIfShow(false);
	GameProps::ItemType type = Props.GetType();
	switch (type)
	{
	case GameProps::ItemType::Chariot:
		Player.SetLife(Player.GetLife() + 1);
		Props.SetIfExist(false);
		break;
	case GameProps::ItemType::Star:
		Player.LevelUP();
		Props.SetIfExist(false);
		break;
	case GameProps::ItemType::Handgrenade:
		for (int i = 0; i < 4; i++) {
			AllEnemy[i].SetLife(0);
		}
		Props.SetIfExist(false);
		break;
	case GameProps::ItemType::Clock:
		for (int i = 0; i < 4; i++) {
			AllEnemy[i].SetIfGetTimeStop(true);
			AllEnemy[i].SetGetTimeStop(Props.IfEffectExit());
		}
		break;
	case GameProps::ItemType::Steel_helmet:
		if (Props.IfEffectExit() == 1) {
			Player.SetIfInvicible(true);
		}
		else {
			Player.SetIfInvicible(false);
		}
		break;
	case GameProps::ItemType::Shovel:
		StageMap.SetGetShovel(Props.IfEffectExit());
		break;
	case GameProps::ItemType::Pistol:
		for (int i = Player.GetLevel(); i < 5; i++) {
			Player.LevelUP();
		}
		Props.SetIfExist(false);
		break;
	case GameProps::ItemType::Ship:
		Player.SetIfGetShip(true);
		break;
	}
}