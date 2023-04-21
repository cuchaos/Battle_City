#pragma once
#include "Map.h"
namespace game_framework {
	class GameProps { //地圖的物件(格子)
	public:
		GameProps();
		void OnInit();
		void OnShow();
		int count(int Type);
		int find(int Type);
		int IfEffectExit();

		void SetGameProps();
		void SetIfShow(bool IfShow);
		void SetIfExist(bool IfExist);
		void SetIfCountDown(bool IfCountDown);

		int GetType();
		bool GetIfShow();
		bool GetIfTouched();
		bool GetIfExist();
		vector<int> GetAllPropType();
		CMovingBitmap GetPropBitmap();
	private:
		CMovingBitmap _Props;
		clock_t _StartTime;

		static vector<int> _AllPropType;
		int _X, _Y; //top left x,top y
		int _Type; // 0 is chariot,1 is Star, 2 is handgrenade, 3 is clock,4 is steel helmet, 5 is shovel, 6 is pistol

		bool _IfShow; // 甚麼時候要出現
		bool _IfCountDown;
		bool _IfTouched;
		bool _IfExist;
		
		int _NowIndex;
	};
}