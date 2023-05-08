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
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnMove();									// 移動遊戲元素
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);
		Menu _Lobby;
		Event event;
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void OnShowText();
	private:
		int _MouseX;
		int _MouseY;
		CMovingBitmap logo;								// csie的logo
		CPlayer Player;
		
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
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
			Settlement
		};
		void OnShowText();
		int _MouseX;
		int _MouseY;
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
		InGameState state;
		void PlayerTankCollisionMap(CPlayer *tank);
		void EnemyTankCollisionMap(Enemy *tank);
		//bool PlayerTankCollision();
		bool EnemyTankCollision(CTank *tank);
		bool TankCollision(CTank *tank ,CTank *who);
		bool ShootCollision(CBullet Bullet,int TankLevel); 
		Event event;

		bool IfNoEnemy();
		void RandomSpawnTank(int num);
		void PlayerOnMove(CPlayer &tank);
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
		int _EnemyQuantity;
		CPlayer _PlayerTank;
		bool _isHoldUpKey, _isHoldDownKey, _isHoldRightKey, _isHoldLeftKey;
		bool _IfBattling; //the var that depend on Map and Gray Menu and tank and props show 
		bool _IfSettling;
		bool _IfSelecting;
		vector<int> _IfEatItem;
		int _OnIceCountDown; // If we on ice,we should go without any keydown,this is 
							// the go on counter 2 Grid,so is 64 pixel
		int tempIndex;
		int _HoldKey;
		int _PlayerTankFrontX ;
		int _PlayerTankFrontY ;
		vector<vector<int>> _tempcollision = { {0,0},{0,0} };
		//vector<vector<int>> _Tanktempcollision = { {0,0},{0,0} };
		//vector<vector<int>> _tempcollision;
		std::vector<Enemy> EnemyList = std::vector<Enemy>(4);
		std::vector<clock_t> EnemyReSpawnLastTime = std::vector<clock_t>(4);
		vector<int> EnemyTypeList = {0,0,0,0};
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

}