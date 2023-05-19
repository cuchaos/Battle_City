#pragma once
#include "MapItem.h"
namespace game_framework {
	class Map { //地圖的物件(格子)
	public:
		enum InfoType{
			CanWalk,
			CanShoot,
			CanBreak,
		};
		enum ObjectDirection {
			Right, Down, Left, Up
		};
		Map();
		vector<vector<MapItem>> _Stage; //每一個stage
		void OnInit(vector<vector<int>> Stage); //要傳某一個stage的每個格子 type
		void OnShow();
		void OnShowGrass();
		void ShootWall(int Direction, int Attack, int x, int y); //Iron wall or wall was shot
		void SetHomeBreak();

		void SetGetShovel(int Type);
		void SetIfShowMap(bool Status);
		void SetEnemySignPop();

		int GetGridIndexX(int NowX);
		int GetGridIndexY(int NowY);
		int GetEnemySignNum();
		bool GetMapItemInfo(int _x, int _y, InfoType GridAttribute); // 獲取地圖某一格格子的資訊 0:walk ,1:shoot ,2:break
		int GetType(int _x, int _y);
		bool GetIfShowMap();
		bool GetIfGrassInMap();
		bool GetIfBoardEdge(int Nowx, int Nowy, int NowHeight, int NowWidth, int NowDirection); // wil return a bool that If u touch the edge
		vector<vector<int>> GetFrontGridsIndex(vector<vector<int>> Grid1XY); // you should put the two of pixel xy here
		
	private:
		clock_t _StartTime; // last time to get that we have get item shovel
		bool _IfGetShovel; 
		bool _IfShovelShine; // last 3 sec will shine

		bool _IfShowMap;
		bool _IfGrassInMap;
		CMovingBitmap _BlackGrid;
		vector<CMovingBitmap> _EnemySign;
		CMovingBitmap _OnePlayerHealth;
		CMovingBitmap _SecondPlayerHealth;
		CMovingBitmap _BattleMenuFlag;
		vector<CMovingBitmap> _PlayerSign;
	};
}