#pragma once

namespace game_framework {
	class CBullet
	{
	public:
		CBullet();
		void LoadBitmap();
		void SetBulletFire(int TankX, int TankY, int TankDirect, int BulletSpeed);
		void OnShow();
		void SetBulletAlreadyFire(bool AlreadyFire);
		bool GetAlreadyFire();
		bool GetIfBoom();
		void SetIfBoom(bool Status);
		int GetDirection();
		static vector<vector<int>> _Move;
		vector<vector<int>> GetNowBackPlace();
		vector<vector<int>> GetNowFrontPlace();
		int GetHeight();
		int GetWidth();
		CMovingBitmap GetBulletBitmap();
	private:
		enum Move {
			Right, Down, Left, Up
		};
		bool _AlreadyFire;
		bool _IfBoom;
		vector<vector<int>> _NowBackPlace; //the place that stand on your left and right , but doesnt plus width and height
		vector<vector<int>> _NowFrontPlace; //plus width and height
		int _Direction;
		vector<vector<int>> _NextMove;
		CMovingBitmap _Bulletimage; //all bullet image in here ,index 0 is Go to east Bullet image,1 is south,2 is west,3 is north
		CMovingBitmap _Boom; // the Animation of Boom (Bullet touch the wall or sth)
	};

}