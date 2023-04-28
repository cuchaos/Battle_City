#pragma once

namespace game_framework {
	class MapItem { //地圖的物件(格子)
	public:
		enum GridType {
			NoneLoad = 0,
			BlackGrid = 1,
			Water = 2,
			Ice = 3,
			BrickGrid = 4,
			SteelGrid = 5,
			GrassGrid = 6,
			FlagHome = 7
		};
		MapItem();
		MapItem(int Type);
		void SetTopLeft(int x, int y);
		void OnShow();
		void GrassOnsShow();
		void ChangeGridState(int Direction,int Attack); // 子彈方向(磚牆)

		bool GetIfShoot();
		bool GetIfBreak();
		bool GetIfWalk();
		bool GetIfShine();
		int GetType();
		CMovingBitmap GetMapItmeBitmap();

		void SetShovelChangeType(GridType type,bool IfShine);
	private:
		CMovingBitmap _OneGrid;
		CMovingBitmap _ShovelAnimaetion;
		bool _IfGetShovel;
		bool _IfShine;

		int _Health;
		GridType _Type;
		bool _IfShoot;  //是否可以射擊 false 不行,true可以
		bool _IfBreak;  //是否可以破壞 false 不行,true可以
		bool _IfWalk;   //是否可以走上去(格子) false 不行,true可以
	};
}