#pragma once

#include "Bullet.h"

namespace game_framework {
	class CTank
	{
	public:
		CTank();
		enum Direction {
			Right,
			Down,
			Left,
			Up,
		};
		enum TankState {
			Spawn,
			Alive,
			Death,
		};
		~CTank();
		void Move();
		void LoadBitmap();					//Load重生動畫
		virtual void OnShow() = 0;				//SHOW
		void ShowSpawnAnimation();			//Show重生動畫
		virtual void Animation() = 0;
		
		int GetX1(); //Get top left 
		int GetY1(); //Get top left 
		int GetWidth();
		int GetHeight();
		int GetOriginAngle();
		int GetLife();
		bool GetIfGetShip();
		int GetLevel();
		int GetTankState();
		CMovingBitmap GetTankBitmap();

		void SetIfBattle(bool Battle);
		void SetXY(int x, int y);			//座標設定
		void SetLife(int num);

		// location
		void TankFront();					//坦克前方的格子做定位
		void LocationPoint();				//位置校正
		void TurnFace(UINT 📥);			//調整圖片方向
		
		vector<vector<int>> GetTankFront();

		// virtual Direction (LoadBitmapIndex)
		virtual void SetFaceDirection() = 0;
		virtual void TankbeHit() = 0;

		// bullet
		CBullet _Bullet;
		virtual void SetBulletStatus(int BulletOrder, bool Status) = 0;
		virtual void SetIfFire(int FireOrder, bool Status) = 0;
		virtual void FireBullet(int BulletOrder) = 0;
		virtual bool GetIfFire(int FireOrder=1) = 0;
		int GetBulletOwner();
		CMovingBitmap GetBulletBitmap();
		void SetBulletOwner(int Who);
	protected:
		int _X, _Y;									//地圖座標
		int Width, Height;							//OneGrid寬,高

		clock_t _Last_time; // last time that bullet move; (we can consider it as delay)

		CMovingBitmap _Tank;
		CMovingBitmap _SpawnAnimation;
		CMovingBitmap _TankBrokenAnimation;
		
		bool _IfBattle; //the var that decide onshow
		int _Level, _Life;							//坦克等級&生命
		int _MovementSpeed;							//移動速度
		bool _Setinit;
		int _TankState;

		bool _IfGetShip;
		bool _AttackSpeedUP;						//功速提升		 level >= 2
		bool _CanBreakIron;							//可破壞鐵牆		 level >= 4
		bool _DoubleAttack;							//可發射兩發子彈   level >= 3

		bool _IfFire;
		int _BulletFlySpeed;
					
		int _FrameTime,_Frameindex,_FrameSecond;	//動畫計時器,動畫初始禎,計時秒數
		int _OriginAngle,_TurnAngle;				//原始角度,轉向角度
		int _LocationDistance;						//偏移距離上限
		vector<vector<int>> _FrontXY;				//前方的X和Y 
		vector<int> _NowGrid;						//現在的格子
		vector<int> _OffsetXY;						//偏移距離
	};
}
