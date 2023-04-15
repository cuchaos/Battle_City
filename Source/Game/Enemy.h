#pragma once

#include "CTank.h"

namespace game_framework {
	class Enemy : public CTank
	{
	public:
		Enemy();
		int GetEnemyScore();
		bool isEnemyHaveItem();					// 是否有道具
		
		void SetEnemyHaveItem(bool has);				// 設定有道具
		void SetEnemyType(int _num);			// 設定敵人類型
		void SetEnemyInit();					// 設定初始化
		void SetGetTimeStop(int Status);
		void SetIfGetTimeStop(bool IfGetTimeStop);
		void LoadBitmap();						
		void SetOriginAngle(int _direction);	// 設定轉向方向 
		void EnemyRandomDirection();			// 隨機設定前進方向
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
	private:
		int _EnemyType;
		int _EnemyScore;
		int _RandomDirection,_RandomMoveTime;	// 隨機轉向方向,隨機移動時間
		clock_t _TimeFinish, _TimeStart;		// 計時器 (結束-開始 = 經過時間)
		bool _EnemyHaveItem;					// 道具
		
		bool _IfGetTimeStop;
	};

}
