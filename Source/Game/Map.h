﻿#pragma once
#include "MapItem.h"
namespace game_framework {
	class Map { //地圖的物件(格子)
	public:
		Map();
		vector<vector<MapItem>> _Stage; //每一個stage
		void OnInit(vector<vector<int>> Stage); //要傳某一個stage的每個格子 type
		int _BoardX; //棋盤的x
		int _BoardY; //棋盤的y
		void Tankdetect(int TankX, int TankY);
		void OnShow();
	private:
		vector<CMovingBitmap> _EnemySign;
		CMovingBitmap _OnePlayerHealth;
		CMovingBitmap _SecondPlayerHealth;
		CMovingBitmap _BattleMenuFlag;
		vector<CMovingBitmap> _PlayerSign;
	};
}