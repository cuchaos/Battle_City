/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/
#include "CEvent.h"
#include "Menu.h"
#include "GameProps.h"
#include "MapItem.h"
#include "Map.h"
#include "CPlayer.h"
#include "Enemy.h"
#include "../Library/audio.h"
namespace game_framework {
	enum AUDIO_ID {
		AUDIO_BGM = 16,
		AUDIO_Move = 17
	};
	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								
		void OnBeginState();							
		void OnMove();									
		void OnKeyUp(UINT, UINT, UINT); 				
		void OnLButtonDown(UINT nFlags, CPoint point);  
		void OnMouseMove(UINT nFlags, CPoint point);
		Menu _Lobby;
		Event event;
	protected:
		void OnShow();									
		void OnShowText();
	private:
		int _MouseX;
		int _MouseY;
		CMovingBitmap logo;								
		CPlayer Player;
		
	};

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							
		void OnInit();  								
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  
		void OnLButtonUp(UINT nFlags, CPoint point);	
		void OnMouseMove(UINT nFlags, CPoint point);	
		void OnRButtonDown(UINT nFlags, CPoint point);  
		void OnRButtonUp(UINT nFlags, CPoint point);	
		Map Stage1;
	protected:
		enum Direction {
			Right,
			Down,
			Left,
			Up,
		};
		enum BulletOrder {
			FirstBullet = 1,
			SecondBullet = 2
		};
		enum InGameState {
			SelectStage,
			PreBattle,
			Battle,
			Settlement,
			GameOver
		};
		void OnShowText();
		int _MouseX;
		int _MouseY;
		void OnMove();								
		void OnShow();									
		InGameState state;
		void PlayerTankCollisionMap(CPlayer *tank);
		void EnemyTankCollisionMap(Enemy *tank);
		bool EnemyTankCollision(CTank *tank);
		bool TankCollision(CTank *tank ,CTank *who);
		bool ShootCollision(CBullet Bullet,int TankLevel); 
		Event event;

		bool IfNoEnemy();
		void RandomSpawnTank(int num);
		void PlayerOnMove();
		void AllEnemyOnMove();

		void AllBulletFly();
		void AllBulletCollision();
		void PlayerBulletCollision(BulletOrder Order);
		void EnemyAllBulletCollision();
		bool BulletHitTank(CBullet CurrentBullet, CTank *BulletOwner, CTank *DetectTarget, BulletOrder Order);
		bool BulletHitBullet(CBullet CurrentBullet, CTank *BulletOwner, CTank *DetectTarget, BulletOrder Order);
		vector<CBullet*> _AllBullet; // index 0 and 1 is player's , index >= 2 is enemy's
		int _NowStage;
		int _EnemyNum;

		bool IfResetPropTime(int NowPropIndex,GameProps NowProp);
		void TrigAllProp();
		int _NowPropSize;  // the var that record Prop number
		vector<GameProps> _Prop;
		
		Menu _Menu;

		clock_t _TimerSpawn,_TimerFinish;
		vector<clock_t> _ScoreClock;
		int _EnemyQuantity;
		CPlayer _PlayerTank;
		bool _isHoldUpKey, _isHoldDownKey, _isHoldRightKey, _isHoldLeftKey;
		bool _IfBattling; //the var that depend on Map and Gray Menu and tank and props show 
		bool _IfSettling;
		bool _IfSelecting;
		vector<int> _IfEatItem;
		int _OnIceCountDown; // If we on ice,we should go without any keydown,this is 
							// the go on counter 2 Grid,so is 64 pixel
		bool _collision;
		int tempIndex;
		int _HoldKey;
		int _PlayerTankFrontX ;
		int _PlayerTankFrontY ;
		int _PlayerLife;
		vector<vector<int>> _tempcollision = { {0,0},{0,0} };
		//vector<vector<int>> _Tanktempcollision = { {0,0},{0,0} };
		//vector<vector<int>> _tempcollision;
		std::vector<Enemy> EnemyList = std::vector<Enemy>(4);
		std::vector<clock_t> EnemyReSpawnLastTime = std::vector<clock_t>(4);
		clock_t GameOverClock;
		vector<int> EnemyTypeList = {0,0,0,0};
		CMovingBitmap _GameOverSign;

		CAudio* audio = CAudio::Instance();
		vector<bool> _AllAudioIfPlaying;
	};

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							
		void OnInit();
	protected:
		void OnMove();									
		void OnShow();									
	private:
		int counter;	
	};

}