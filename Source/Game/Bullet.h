#pragma once
namespace game_framework {
	class CBullet
	{
	public:
		enum Move {
			Right, Down, Left, Up
		};
		static vector<vector<int>> _Move;
		CBullet();
		void LoadBitmap();
		void OnShow();
		void BulletFly();

		void SetOwner(int who);
		void SetBulletAlreadyFire(bool AlreadyFire);
		void SetBulletFire(int TankX, int TankY, int TankDirect, int BulletSpeed);
		void SetIfBoom(bool Status);

		int GetOwner();
		int GetHeight();
		int GetWidth();
		bool GetAlreadyFire();
		int GetDirection();
		bool GetIfBoom();
		vector<vector<int>> GetNowBackPlace();
		vector<vector<int>> GetNowFrontPlace();

		CMovingBitmap GetBitmap();
	private:
		CMovingBitmap _Bulletimage; //all bullet image in here ,index 0 is Go to east Bullet image,1 is south,2 is west,3 is north
		CMovingBitmap _Boom; // the Animation of Boom (Bullet touch the wall or sth)

		bool _AlreadyFire;
		int _Direction;
		int _BulletSpeed;
		int _Owner;					// player =1, enemy =0
		bool _IfBoom;

		vector<vector<int>> _NowBackPlace; //the place that stand on your left and right , but doesnt plus width and height
		vector<vector<int>> _NowFrontPlace; //plus width and height
		vector<vector<int>> _NextMove;
	
	};

}