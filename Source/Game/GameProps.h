#pragma once
#include "Map.h"
namespace game_framework {
	class GameProps { //地圖的物件(格子)
	public:
		enum class ItemType
		{
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
		int IfEffectExit(ItemType Type);

		void SetDropProp();
		void SetPropEffectStart(ItemType Type);
		void SetPropIfEffect(ItemType Type,bool Status);
		void SetIfExist(bool IfExist);
		void ReStartAllProp();

		int GetX();
		int GetY();
		ItemType GetType();
		bool GetIfExist();
		vector<bool> GetAllPropType();
		CMovingBitmap GetPropBitmap();
	private:
		CMovingBitmap _PropPicture;
		static vector<clock_t> _AllPropsStartTime;
		static vector<bool> _AllPropsIfEffect;

		int _X, _Y; //top left x,top y
		ItemType _Type; // 0 is chariot,1 is Star, 2 is handgrenade, 3 is clock,4 is steel helmet, 5 is shovel, 6 is pistol

		bool _IfExist;
	};
}