#pragma once

#include "Bullet.h"

namespace game_framework {
	class CTank
	{
	public:
		CTank();
		int GetX1(); //Get top left 
		int GetY1(); //Get top left 
		int GetWidth();
		int GetHeight();
		int GetOriginAngle();
		int GetLife();
		bool GetEnemyisNeedRespawn();
		void SetLife(int num);
		bool isBreak();
		void TankFront();					//坦克前方的格子做定位
		void Animation();							
		void LocationPoint();				//位置校正
		void Move();						//移動
		void OnShow();						//SHOW
		void SetXY(int x, int y);			//座標設定
		void TurnFace(UINT nChar);			//調整圖片方向
		void SetFaceDirection();
		void TankbeHit();
		vector<vector<int>> GetTankFront();

		// bullet
		void FireBullet();
		void SetBulletStatus(bool Status);
		bool GetIfFire();
		void SetIfFire(bool Status);
		CBullet _Bullet;

		void SetIfBattle(bool Battle);

		void LoadBitmap();					//Load重生動畫
		void ShowSpawnAnimation();			//Show重生動畫
		bool GetSpawnAnimationDone();		//Get重生動畫播放完畢
		
		CMovingBitmap GetTankBulletBitmap();
		CMovingBitmap GetTankBitmap();
	protected:
		enum Direction {
			Right,
			Down,
			Left,
			Up,
		};
		enum EnemyType {
			LightTank,
			QuickTank,
			ArmorTank,
			HeavyTank,
		};
		bool _IfBattle; //the var that decide onshow
		bool _IfFire;
		clock_t _Last_time; // last time that bullet move; (we can consider it as delay)
		int _BulletFlySpeed;
		bool _isNeedRespawn;
		CMovingBitmap _Tank;
		CMovingBitmap _SpawnAnimation;
		CMovingBitmap _TankBrokenAnimation;
		bool _SpawnAnimationDone;
		bool _isTankBrokenAnimationDone;
		int Width,Height;							//OneGrid寬,高
		int _X,_Y;									//地圖座標								
		int _FrameTime,_Frameindex,_FrameSecond;	//動畫計時器,動畫初始禎,計時秒數
		int _OriginAngle,_TurnAngle;				//原始角度,轉向角度
		int _Level,_Life;							//坦克等級&生命
		int _MovementSpeed;							//移動速度
		int _LocationDistance;						//偏移距離上限
		vector<vector<int>> _FrontXY;				//前方的X和Y 
		vector<int> _NowGrid;						//現在的格子
		vector<int> _OffsetXY;						//偏移距離
		bool _AttackSpeedUP;						//功速提升		 level >= 2
		bool _CanBreakIron;							//可破壞鐵牆		 level >= 4
		bool _DoubleAttack;							//可發射兩發子彈   level >= 3
	};
}
