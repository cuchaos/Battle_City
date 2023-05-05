#pragma once

#include "CTank.h"

namespace game_framework {
	class Enemy : public CTank
	{
	public:
		enum EnemyType {
			LightTank,
			QuickTank,
			ArmorTank,
			HeavyTank,
		};
		Enemy();
		void LoadBitmap();
		int GetEnemyScore();
		bool GetIfBattle();
		bool SuccessMove(int direction);

		void SetEnemyHaveItem(bool has);		// 設定有道具
		void SetEnemyType(int _num);			// 設定敵人類型
		void SetEnemyInit();					// 設定初始化
		void SetOriginAngle(int _direction);	// 設定轉向方向 
		void EnemyRandomDirection();			// 隨機設定前進方向
		
		// Prop
		void SetGetTimeStop(int Status);
		void SetIfGetTimeStop(bool IfGetTimeStop);
		bool isEnemyHaveItem();					// 是否有道具
		// bullet
		void FireBullet(int BulletOrder) override;
		void SetBulletStatus(int BulletOrder,bool Status) override;
		void SetIfFire(int FireOrder, bool Status) override;
		bool GetIfFire(int FireOrder) override;
		int GetEnemyType();
		bool GetEnemySetInit();
		bool GetIfGetTimeStop();
		CMovingBitmap GetEnemyBitmap();
		void SetFaceDirection() override;
		void OnShow() override;
		void TankbeHit() override;
		void Animation() override;
		void OnShowScore(CDC *pDC, CFont* &fp);
	private:
		clock_t _TimeFinish, _TimeStart, _SpawnClock;		// 計時器 (結束-開始 = 經過時間)
		int _EnemyType;
		int _EnemyScore;
		bool _EnemyHaveItem;					// 道具

		int _RandomDirection,_RandomMoveTime;	// 隨機轉向方向,隨機移動時間
		bool _IfGetTimeStop;
		int DownDistance;
		int LeftDistance;
		int RightDistance;
	};

}
