#pragma once
#include "Map.h"
namespace game_framework {
	class GameProps { //地圖的物件(格子)
	public:
		GameProps();
		void OnInit();
		void SetGameProps();
		void SetIfShow(bool IfShow);
		void SetIfExist(bool IfExist);
		void SetIfCountDown(bool IfCountDown);

		int count(int Type);
		int find(int Type);
		int GetType();
		bool GetIfShow();
		bool GetIfTouched();
		bool GetIfExist();
		int GetX();
		int GetY();
		vector<int> GetAllPropType();

		CMovingBitmap GetPropBitmap();
		int IfEffectExit();
		void OnShow();
		//void OnShowScore(CDC *pDC, CFont* &fp);
	private:
		CMovingBitmap _Props;

		clock_t _StartTime; 
		bool _IfCountDown;
		bool _IfTouched;
		bool _IfExist;
		static vector<int> _AllPropType;
		int _X, _Y; //top left x,top y
		bool _IfShow; // 甚麼時候要出現
		int _Type; // 0 is chariot,1 is Star, 2 is handgrenade, 3 is clock,4 is steel helmet, 5 is shovel, 6 is pistol
		int _NowIndex;
	};
}