#include <vector>
#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include "CEvent.h"

using namespace game_framework;

Event::Event() {
	
}

void Event::TriggerLobbyMenu(Menu& LobbyMenu) {
	if (LobbyMenu.GetMenuY(LobbyMenu.LobbyMenu) > 0) {
		LobbyMenu.SetIfAnimation(true);
		LobbyMenu.SetMenuRaise(LobbyMenu.LobbyMenu);
	}
	else {
		LobbyMenu.SetSelecting(true);
		LobbyMenu.SetIfAnimation(false);
	}
}
void Event::TriggerSelectingStage(Menu& GrayScreen) {
	if (GrayScreen.GetMenuY(GrayScreen.ChooseStageMenu) < 0) {
		GrayScreen.SetMenuRaise(GrayScreen.ChooseStageMenu);
	}
	else {
		GrayScreen.SetSelecting(true);
		GrayScreen.SetIfAnimation(false);
	}
}
void Event::TriggerUpdateMap(Map& StageMap) {
	StageMap.SetEnemySignPop();
}
void Event::TriggerSetBattleMap(vector<vector<int>>& Stage,Map& StageMap, Menu& BattleMenu, CPlayer& Player, GameProps& Props, vector<Enemy>& AllEnemy) {
	Props.ReStartAllProp();
	StageMap.OnInit(Stage);
	StageMap.SetIfShowMap(true);
	BattleMenu.SetMenuType(BattleMenu.BattleMenu);
	Player.SetPlayerReSpawn();
	Player.SetIfBattle(true);
	for (auto& enemy : AllEnemy) {
		if (enemy.GetTankState() == Enemy::Alive) {
			enemy.SetLife(0);
			enemy._Bullet.SetBulletAlreadyFire(false);
			enemy._Bullet.SetIfBoom(false);
			enemy.SetEnemyIfExplision(true);
		}
		enemy.SetIfGetTimeStop(false);
		enemy.SetIfBattle(true);
	}
}
void Event::TriggerSettlement(Menu& SettlementMenu, vector<int>& StageEnemy, int& NowScore,int& TheHighestScore,int& NowStage) {
	vector<int> EnemyScore = { 100,200,300,400 };
	SettlementMenu.SetMenuType(SettlementMenu.SettleMenu);
	SettlementMenu.SetSettlement(StageEnemy, EnemyScore, NowScore, TheHighestScore,NowStage);
}
void Event::TriggerCancelPropOnMap(GameProps& Prop) {
	if (Prop.GetIfExist()) {
		Prop.SetIfExist(false);
	}
}
void Event::TriggerNextStage(Map& StageMap, Menu& BattleMenu, int& EnemyNum, int& NowStage, vector<Enemy>& EnemyList) {

	for (auto& enemy : EnemyList) {
		enemy.SetIfBattle(false);
	}
	StageMap.SetIfShowMap(false);
	EnemyNum = 20;
}
void Event::TriggerSetDropProp(GameProps& Prop) {
	Prop.SetDropProp();
}
void Event::TriggerPropsEffect(GameProps& Props,Map& StageMap,CPlayer& Player,vector<Enemy>& AllEnemy,int& EnemyNum,int& PlayerRespawnTimes,vector<int>& DeadEnemyList) {
	vector<bool> PropsIfEffect = Props.GetAllPropIfEffect();
	for (int i = 0;i < 8;i++) {
		if (!PropsIfEffect[i]) {
			continue;
		}
		GameProps::ItemType type = GameProps::ItemType(i);
		switch (type)
		{
		case GameProps::ItemType::Chariot:
			PlayerRespawnTimes += 1;
			Props.SetPropIfEffect(type, false);
			break;
		case GameProps::ItemType::Star:
			Player.LevelUP();
			Props.SetPropIfEffect(type, false);
			break;
		case GameProps::ItemType::Handgrenade:
			for (int i = 0; i < 4; i++) {
				if (AllEnemy[i].GetTankState() == CTank::Alive) {
					AllEnemy[i].SetLife(0);
					DeadEnemyList[AllEnemy[i].GetEnemyType()] += 1;
					EnemyNum -= 1;
				}
			}
			Props.SetPropIfEffect(type, false);
			break;
		case GameProps::ItemType::Clock:
			for (int i = 0; i < 4; i++) {
				AllEnemy[i].SetIfGetTimeStop(true);
				AllEnemy[i].SetGetTimeStop(Props.IfEffectExit(type));
			}
			break;
		case GameProps::ItemType::Steel_helmet:
			if (Props.IfEffectExit(type) == 1) {
				Player.SetIfInvicible(true);
			}
			else {
				Player.SetIfInvicible(false);
				Props.SetPropIfEffect(type, false);
			}
			break;
		case GameProps::ItemType::Shovel:
			StageMap.SetGetShovel(Props.IfEffectExit(type));
			break;
		case GameProps::ItemType::Pistol:
			for (int i = Player.GetLevel(); i < 5; i++) {
				Player.LevelUP();
			}
			Props.SetPropIfEffect(type, false);
			break;
		case GameProps::ItemType::Ship:
			Player.SetIfGetShip(true);
			Props.SetPropIfEffect(type, false);
			break;
		}
	}
}

void Event::TriggerGameOver(CMovingBitmap& GameOverSign) {
	if (GameOverSign.GetTop() >= 450) {
		GameOverSign.SetTopLeft(516, GameOverSign.GetTop() - 20);
	}
}