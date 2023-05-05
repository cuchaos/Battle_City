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
		static void TrigLobbyMenu(Menu& LobbyMenu); // If u lose your 3 health,or first time login,you will go lobby (include select)
		static void TrigSelectingStage(Menu& GrayMenu); //use this func when choosing stage,and battle 
		static void TrigSetBattleMap(vector<vector<int>>& Stage,Map& StageMap,int& EnemyNum, Menu& BattleMenu, CPlayer& Player, vector<GameProps>& Props);
		static void TrigUpDateMap(Map& StageMap);
		static void TrigSettlement(Menu& SettlementMenu, vector<int>& StageEnemy, int& NowScore, int& TheHighestScore,int& NowStage);
		static void TrigNextStage(Map& StageMap, Menu& BattleMenu, int& EnemyNum, int& NowStage);
		static void TrigSetProps(vector<GameProps>& Props,int NowPropndex);
		static void TrigReSetProps(vector<GameProps>& Props);
		static void TrigGetProps(GameProps& Props, Map& StageMap, CPlayer& Player,vector<Enemy>& AllEnemy); // lost a enemy class

	private:
		bool _IfStart; // the attribute that can decide the Battle start or not
	};
}
