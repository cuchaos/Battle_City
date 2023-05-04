#pragma once

#include "CTank.h"
#include "Bullet.h"
namespace game_framework {
	class CPlayer :public CTank 
	{
	public:
		CPlayer();
		CBullet _SecondBullet;
		void PlayerInit();
		void LoadBitmap();
		void InvicibleAnimation();
		void OnShow() override;
		void FireBullet(int BulletOrder) override;
		void TankExpolsion() override;
		void Animation() override;
		void LevelUP();

		bool GetIfInvicible();
		bool GetIfFire(int FireOrder) override;

		void SetFaceDirection() override;
		void SetIfInvicible(bool Status);
		void SetIfGetShip(bool Status);
		void SetIfFire(int FireOrder, bool Status) override;
		void SetBulletStatus(int BulletOrder, bool Status) override;

	private:
		CMovingBitmap _Invicible,_Ship;
		clock_t _InvicibleClock;
		bool _IfSecondFire;
		int _IfInvicible;
		int _PlayerScore;
		vector<int> _KillEnemyList;

	};
}
