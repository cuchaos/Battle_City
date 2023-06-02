#pragma once

#include "Menu.h"
#include "Map.h"
#include "CPlayer.h"
#include "GameProps.h"
#include "Enemy.h"
namespace game_framework {
	class Event {
	public:
		Event();
		static void TriggerLobbyMenu(Menu& LobbyMenu); // If u lose your 3 health,or first time login,you will go lobby (include select)
		static void TriggerSelectingStage(Menu& GrayMenu); //use this func when choosing stage,and battle 
		static void TriggerSettlement(Menu& SettlementMenu, vector<int>& StageEnemy, int& NowScore, int& TheHighestScore, int& NowStage);
		static void TriggerGameOver(CMovingBitmap& GameOverSign);
		static void TriggerSetBattleMap(vector<vector<int>>& Stage,Map& StageMap, Menu& BattleMenu, CPlayer& Player, GameProps& Props, vector<Enemy>& AllEnemy);
		static void TriggerUpdateMap(Map& StageMap);
		static void TriggerNextStage(Map& StageMap, Menu& BattleMenu, int& EnemyNum, int& NowStage, vector<Enemy>& EnemyList);
		static void TriggerSetDropProp(GameProps& Prop);
		static void TriggerCancelPropOnMap(GameProps& Prop);
		static void TriggerPropsEffect(GameProps& Props, Map& StageMap, CPlayer& Player,vector<Enemy>& AllEnemy, int& EnemyNum , int& PlayerRespawnTimes,vector<int>& DeadEnemylist); // lost a enemy class
	};
}
