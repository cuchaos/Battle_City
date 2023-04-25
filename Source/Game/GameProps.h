#pragma once
#include "Map.h"
namespace game_framework {
	class GameProps { //地圖的物件(格子)
	public:
		enum class ItemType
		{
			Empty = -1,

			Chariot = 0,
			Star = 1,
			Handgrenade = 2,
			Clock = 3,
			Steel_helmet = 4,
			Shovel = 5,
			Pistol = 6,
			Ship = 7
		};

		GameProps();
		void OnInit();
		void OnShow();
		int count(ItemType Type);
		int find(ItemType Type);
		int IfEffectExit();

		void SetGameProps();
		void SetIfShow(bool IfShow);
		void SetIfExist(bool IfExist);
		void SetIfCountDown(bool IfCountDown);
		void ReStartAllProp();

		int GetX();
		int GetY();
		ItemType GetType();
		bool GetIfShow();
		bool GetIfTouched();
		bool GetIfExist();
		vector<ItemType> GetAllPropType();
		CMovingBitmap GetPropBitmap();
	private:
		CMovingBitmap _Props;
		clock_t _StartTime;

		static vector<ItemType> _AllPropType;
		int _X, _Y; //top left x,top y
		ItemType _Type; // 0 is chariot,1 is Star, 2 is handgrenade, 3 is clock,4 is steel helmet, 5 is shovel, 6 is pistol

		bool _IfShow; // 甚麼時候要出現
		bool _IfCountDown;
		bool _IfTouched;
		bool _IfExist;
		
		int _NowIndex;
	};
}