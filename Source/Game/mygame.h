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
		AUDIO_HitHeavyTank = 1,
		AUDIO_HitIronOrBound = 2,
		AUDIO_HitBrickWall = 4,
		AUDIO_FireBullet = 5,
		AUDIO_Explosion = 6,
		AUDIO_HitHomeOrPlayer = 10,
		AUDIO_ItemApear = 11,
		AUDIO_GetItem = 13,
		AUDIO_BGM = 15,
		AUDIO_Move = 16
	};
	enum BulletOwner {
		Eenemy,
		Player
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
		void OnMove();
		void OnMouseMove(UINT nFlags, CPoint point);	
		void OnShow();
		void OnShowText();
		void OnShowScore(CDC* pDC, CFont*& fp);
		Map Stage1;
		bool _isHoldUpKey, _isHoldDownKey, _isHoldRightKey, _isHoldLeftKey;
		int _HoldKey;
		int _MouseX;
		int _MouseY;
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
		// Collision
		vector<vector<int>> _tempcollision = { {0,0},{0,0} };
		// Tank collision with Map
		bool ShootCollision(CBullet Bullet, int TankLevel, int who);
		void PlayerTankCollisionMap(CPlayer* tank);
		void EnemyTankCollisionMap(Enemy* tank);
		// Tank collision with tank
		bool IfFrontHaveTank(CTank* tank, CTank* who);
		bool TankCollision(CTank* tank);
		// Bullet collision
		// Bullet collision with tank and another bullet
		bool BulletHitTank(CBullet CurrentBullet, CTank* BulletOwner, CTank* DetectTarget, BulletOrder Order);
		bool BulletHitBullet(CBullet CurrentBullet, CTank* BulletOwner, CTank* DetectTarget, BulletOrder Order);
		void PlayerBulletCollision(BulletOrder Order);
		void EnemyAllBulletCollision();
		// All Bullet collision
		void AllBulletCollision();
		// OnMove
		void PlayerOnMove();
		void AllEnemyOnMove();
		void AllBulletFly();
		void IfTouchPropOnMap();
		bool IfResetPropTime(int NowPropIndex, GameProps NowProp);
		void RandomSpawnTank(int num);
		// GameState transformation
		InGameState state;
		void IfGameOver();
		void SetGameOverAndGotoSettlement();
		bool IfHaveEnemy();
		// Audio
		CAudio* audio = CAudio::Instance();
		vector<bool> _AllAudioIfPlaying;
		void PlayAudio(AUDIO_ID Id, bool IfRepeat);
		// else
		Event event;
		Menu _Menu;
		CPlayer _PlayerTank;
		vector<Enemy> EnemyList = vector<Enemy>(4);
		vector<int> EnemyTypeList = { 0,0,0,0 };
		vector<int> DeadEnemyList = { 0,0,0,0 };
		vector<CBullet*> _AllBullet; // index 0 and 1 is player's , index >= 2 is enemy's
		GameProps _Prop;
		bool _IfBattling, _IfSettling, _IfSelecting;
		int _NowStage;
		int _PlayerRespawnTimes;
		int _EnemyExistNum;
		int _OnIceCountDown; // If we on ice,we should go without any keydown,this is 
		// the go on counter 2 Grid,so is 64 pixel
		bool _IfGotoNextStage, _IfGameOver;
		bool _IfNeedShowPropScore;
		clock_t GotoNextStageDelay, GameOverClock;
		clock_t _EnemyReSpawnLastTime, _TimerFinish;
		vector<clock_t> _ScoreClock;
		CMovingBitmap _GameOverSign;
		GameProps cheatprop;
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