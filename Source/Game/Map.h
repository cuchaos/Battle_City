#pragma once
#include "MapItem.h"
namespace game_framework {
	class Map { //地圖的物件(格子)
	public:
		Map();
		vector<vector<MapItem>> _Stage; //每一個stage
		void OnInit(vector<vector<int>> Stage); //要傳某一個stage的每個格子 type
		int _BoardX; //棋盤的x
		int _BoardY; //棋盤的y
		bool GetMapItemInfo(int _x, int _y, int gettype); // 獲取地圖某一格格子的資訊 0:walk ,1:shoot ,2:break
		int GetType(int _x, int _y);
		bool GetIfShowMap();
		bool GetIfGrassInMap();
		void OnShow();
		void OnShowGrass();
		vector<vector<int>> GetFrontGridsIndex(vector<vector<int>> Grid1XY); // you should put the two of pixel xy here
		bool GetIfBoardEdge(int Nowx, int Nowy, int NowHeight, int NowWidth,int NowDirection); // wil return a bool that If u touch the edge
		//CMovingBitmap GetMapBitmap(int _x,int _y);

		void SetIfShowMap(bool Status);

		void ShootWall(int Direction, int Attack, int x, int y); //Iron wall or wall was shot

		void SetGetShovel(int Type);
	private:
		enum ObjectDirection {
			Right, Down, Left, Up
		};
		
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