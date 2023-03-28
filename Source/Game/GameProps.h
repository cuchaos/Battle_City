﻿#pragma once
#include "Map.h"
namespace game_framework {
	class GameProps { //地圖的物件(格子)
	public:
		GameProps();
		void OnInit();
		void SetGameProps();
		void OnShow();

		int GetType();
		void SetIfShow(bool IfShow);
		bool GetIfShow();
		bool GetIfTouched();

		CMovingBitmap GetPropBitmap();

		int IfEffectExit();
	private:
		CMovingBitmap _Props;

		clock_t _StartTime; 
		bool _IfCountDown;
		bool _IfTouched;

		int _X, _Y; //top left x,top y
		bool _IfShow; // 甚麼時候要出現
		int _Type; // 0 is chariot,1 is Star, 2 is handgrenade, 3 is clock,4 is steel helmet, 5 is shovel, 6 is pistol
		
	};
}