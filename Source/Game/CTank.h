﻿#pragma once
namespace game_framework {
	class CTank
	{
	public:
		CTank();
		int GetX1();
		int GetY1();
		int GetFrontX();
		int GetFrontY();
		int GetOriginAngle();
		bool isBreak();
		void TankFront(int grid);
		void Animation();					
		//void AnimationOnce();			
		//void Attacke();						//attack
		void LevelUP();						//升級
		void LocationPoint(int x,int y);	//位置校正
		void Move();				//移動
		void OnShow();						//SHOW
		void SetXY(int x, int y);			//座標設定
		void TurnFace(UINT nChar);			//調整圖片方向
		//CMovingBitmap GetTankBitmap();
	protected:
		CMovingBitmap _Tank;
		int Width,Height;					//OneGrid寬,高
		int _X,_Y;							//地圖座標								
		int _FrameTime,_Frameindex,_FrameSecond;			//動畫計時器,動畫初始禎,計時秒數
		int _OriginAngle,_TurnAngle;		//原始角度,轉向角度
		int _Level,_Life;					//坦克等級&生命
		int _OffsetX, _OffsetY;				//偏移長度
		int _PointX, _PointY;				//紀錄定位點
		int _MovementSpeed;					//移動速度
		int _LocationDistance;				//偏移長度上限
		int _FrontX, _FrontY;				//前方的X和Y 
		bool _AttackSpeedUP;				//功速提升		 level >= 2
		bool _CanBreakIron;					//可破壞鐵牆		 level >= 4
		bool _DoubleAttack;					//可發射兩發子彈   level >= 3
	};

}
