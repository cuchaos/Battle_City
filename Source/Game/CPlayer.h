#pragma once

#include "CTank.h"
#include "Bullet.h"
namespace game_framework {
	class CPlayer :public CTank 
	{
	public:
		CPlayer();
		void KillEnemy(int type);
		void LoadBitmap();
		void SetIfInvicible(bool Status);
		void PlusPlayerScore(int score);
		void LevelUP();
		void PlayerInit();
		int GetPlayerScore();

		bool GetIfInvicible();

		void SetIfGetShip(bool Status);
		void SetMoveOnIce(bool IfOnIce);

		void SetBulletStatus(int BulletOrder, bool Status) override;
		void SetIfFire(int FireOrder, bool Status) override;
		bool GetIfFire(int FireOrder) override;
		void FireBullet(int BulletOrder) override;
		CBullet _SecondBullet;
		void SetFaceDirection() override;
		void OnShow() override;
		void TankbeHit() override;
	private:
		bool _IfSecondFire;

		int _IfInvicible;
		int _PlayerScore;
		vector<int> _KillEnemyList;

	};
}
