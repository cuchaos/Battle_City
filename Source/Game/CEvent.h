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

		static void TriggerSetBattleMap(vector<vector<int>>& Stage,Map& StageMap, Menu& BattleMenu, CPlayer& Player, vector<GameProps>& Props, vector<Enemy>& AllEnemy);
		static void TriggerUpdateMap(Map& StageMap);
		static void TriggerNextStage(Map& StageMap, Menu& BattleMenu, int& EnemyNum, int& NowStage, vector<Enemy>& EnemyList);
		static void TriggerSetProps(vector<GameProps>& Props,int NowPropndex);
		static void TriggerReSetProps(vector<GameProps>& Props);
		static void TriggerGetProps(GameProps& Props, Map& StageMap, CPlayer& Player,vector<Enemy>& AllEnemy); // lost a enemy class

	private:
		bool _IfStart; // the attribute that can decide the Battle start or not
	};
}
