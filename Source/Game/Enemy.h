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
		void OnShow() override;
		void TankExpolsion() override;
		void EnemyRandomDirection();			
		void Animation() override;
		
		int GetEnemyScore();
		bool GetIfBattle();
		bool GetIfFire(int FireOrder) override;

		void SetFaceDirection() override;
		void SetEnemyHaveItem(bool has);		// 設定有道具
		void SetEnemyType(int _num);			// 設定敵人類型
		void SetEnemyInit();					// 設定初始化
		void SetOriginAngle(int _direction);	// 設定轉向方向 
		void SetEnemyReSpawn();
		
		int GetEnemyType();
		bool GetEnemySetInit();
		bool GetIfGetTimeStop();
		// Prop
		void SetGetTimeStop(int Status);
		void SetIfGetTimeStop(bool IfGetTimeStop);
		
		bool GetEnemyHaveItem();					
		// bullet
		void FireBullet(int BulletOrder) override;
		void SetBulletStatus(int BulletOrder,bool Status) override;
		void SetIfFire(int FireOrder, bool Status) override;

		CMovingBitmap GetEnemyBitmap();
		void OnShowScore(CDC *pDC, CFont* &fp);
	private:
		clock_t _TimeFinish, _TimeStart, _SpawnClock,_FireClock;		// 計時器 (結束-開始 = 經過時間)
		int _EnemyType;
		int _EnemyScore;
		bool _EnemyHaveItem;					// 道具

		int _RandomDirection,_RandomMoveTime;	// 隨機轉向方向,隨機移動時間
		bool _IfGetTimeStop;
	};

}
