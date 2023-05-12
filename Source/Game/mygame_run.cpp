#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace game_framework;


CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{

	state = SelectStage;
	_NowStage = -1;
	_IfBattling = false;
	_IfSettling = false;
	_isHoldDownKey = _isHoldUpKey = _isHoldLeftKey = _isHoldRightKey = false;
	_IfEatItem = {0,0,0,0};
	_ScoreClock = { 0,0,0,0 };
	_MouseX = 0;
	_MouseY = 0;
	_OnIceCountDown = 0;
	_PlayerTankFrontX = 0;
	_PlayerTankFrontY = 0;
	_PlayerLife = 2;
	_NowPropSize = 0;

	_TimerSpawn = clock();

}



void CGameStateRun::OnMove()                            
{
	switch(state) {
		case SelectStage:
			event.TriggerSelectingStage(_Menu);
			break;
		case PreBattle:
			_PlayerLife += 1;
			event.TriggerSetBattleMap(_AllStage[_NowStage-1],Stage1,_Menu,_PlayerTank,_Prop,EnemyList);
			EnemyTypeList.assign(_AllStageEnemy[_NowStage - 1].begin(), _AllStageEnemy[_NowStage - 1].end());
			_IfBattling = true;
			_IfSettling = false;
			_NowPropSize = 0;
			_EnemyNum = 16;
			break;
		case Battle:
			TrigAllProp();
			PlayerOnMove(_PlayerTank);
			AllEnemyOnMove();
			AllBulletCollision();
			AllBulletFly();
			if (_PlayerLife == 0) {
				GotoGameState(GAME_STATE_INIT);
			}
			_TimerFinish = clock();
			break;
		case Settlement:
			break;
	}
	// state machine transformation
	switch (state) {
	case SelectStage:
		if (_NowStage != -1 && !_IfBattling && !_IfSettling) state = PreBattle;
		break;
	case PreBattle:
		state = Battle;
		break;
	case Battle:
		if (IfNoEnemy()) {
			_IfBattling = false;
			_IfSettling = true;
			Stage1.SetIfShowMap(false);
			event.TriggerSettlement(_Menu, _AllStageEnemy[_NowStage - 1], _NowTotalScore, _TheHighestScore, _NowStage);
			state = Settlement;
		}
		break;
	case Settlement:
		if (!_IfSettling) {
			state = PreBattle;
		}
	}
}
void CGameStateRun::OnInit()                                  
{
	srand((unsigned)time(NULL));
	_Menu.LoadBitMap();
	_PlayerTank.LoadBitmap();
	for (int i = 0; i < 5; i++) {
		_Prop.push_back(GameProps());
		_Prop[i].OnInit();
	}
	for (int i=0; i<4; ++i) {
		EnemyList[i].LoadBitmap();
		EnemyReSpawnLastTime[i] = clock();
	}
	for (int i = 0; i < 6; i++) {
		if (i == 0) {
			_AllBullet.push_back(&_PlayerTank._Bullet);
		}
		else if (i == 1) {
			_AllBullet.push_back(&_PlayerTank._SecondBullet);
		}
		else if (i >= 2) {
			_AllBullet.push_back(&EnemyList[i - 2]._Bullet);
		}
	}
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (_IfBattling && _PlayerTank.GetTankState() == CPlayer::Alive) {
		if (nChar == 0x5A || nChar == 0x58) {
			if (_PlayerTank.GetLevel() > 3 && _PlayerTank.GetIfFire(1) == true) {
				_PlayerTank.FireBullet(2);
			}
			_PlayerTank.FireBullet(1);
		}
		if (nChar == VK_DOWN)	_isHoldDownKey = true;
		if (nChar == VK_UP)		_isHoldUpKey = true;
		if (nChar == VK_LEFT)	_isHoldLeftKey = true;
		if (nChar == VK_RIGHT)	_isHoldRightKey = true;
		if (nChar == VK_DOWN || nChar == VK_RIGHT || nChar == VK_LEFT || nChar == VK_UP) _HoldKey = nChar;
		if (nChar == 'A') {
			_IfBattling = false;
			_IfSettling = true;
			Stage1.SetIfShowMap(false);
			event.TriggerSettlement(_Menu, _AllStageEnemy[_NowStage - 1], _NowTotalScore, _TheHighestScore,_NowStage);
			state = Settlement;
		}
	}	
	else {
		if (!_IfSettling && state == SelectStage) {
			_NowStage = _Menu.OnKeyDown(nChar, nRepCnt, nFlags);
		}
		else {
			if (_Menu.OnKeyDown(nChar, nRepCnt, nFlags) == 1) {
				_NowStage += 1;
				_IfSettling = false;
			}
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (_IfBattling) {
		if (nChar == VK_DOWN)	_isHoldDownKey = false;
		if (nChar == VK_UP)		_isHoldUpKey = false;
		if (nChar == VK_LEFT)	_isHoldLeftKey = false;
		if (nChar == VK_RIGHT)	_isHoldRightKey = false;

		if (!_isHoldDownKey && !_isHoldUpKey && !_isHoldLeftKey && !_isHoldRightKey) {
			_tempcollision = Stage1.GetFrontGridsIndex(_PlayerTank.GetTankFront());
			if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 3 &&
				Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 3) {
				_OnIceCountDown = 64;
			}
		}
	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  
{
	
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)    
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)    
{
	_MouseX = point.x;
	_MouseY = point.y;
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)    
{
}

void CGameStateRun::OnShow()
{
	_Menu.OnShow();
	if (_IfBattling) {
		Stage1.OnShow();
		if (_NowPropSize != 0) {
			_Prop[_NowPropSize-1].OnShow();
		}
		_PlayerTank.OnShow();
		for (int i = 0; i < 4; i++) {
			EnemyList[i].OnShow();
		}
		if (Stage1.GetIfGrassInMap()) {
			Stage1.OnShowGrass();
		}
	}
	OnShowText();
}
void CGameStateRun::OnShowText() {
	CDC *pDC = CDDraw::GetBackCDC();
	CFont *fp;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 180, 0));
	_TimerFinish = clock();
	CTextDraw::Print(pDC, 0, 0, (to_string(_TimerSpawn / CLOCKS_PER_SEC) + " " + to_string(_TimerFinish)));
	/*CTextDraw::Print(pDC, 0, 25, (to_string(_EnemyQuantity)));
	CTextDraw::Print(pDC, 0, 50, (to_string(_MouseX) + " " + to_string(_MouseY)));
	CTextDraw::Print(pDC, 0, 150, (to_string(Stage1.GetEnemySignNum())));
	CTextDraw::Print(pDC, 0, 125, (to_string(EnemyTankCollision(&_PlayerTank))));*/
	_Menu.OnShowText(pDC, fp);
	
	CTextDraw::ChangeFontLog(pDC, 15, "STZhongsong", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 0, 150, (to_string(_PlayerTank.GetTankState())));
	CTextDraw::Print(pDC, 0, 175, (to_string(_PlayerLife)));

	CTextDraw::Print(pDC, 0 , 450, (to_string(state)));
	CTextDraw::Print(pDC, 0, 475, (to_string(_NowStage)));
	CTextDraw::Print(pDC, 0, 500, (to_string(_EnemyNum)));
	
	for (int i = 0; i < 4; i++){
		if (EnemyList[i].GetTankState() == EnemyList[i].Death && clock() - _ScoreClock[i] <= 750 && EnemyList[i].GetIfexploded()) {
			CTextDraw::ChangeFontLog(pDC, 48, "STZhongsong", RGB(255,255,255));
			CTextDraw::Print(pDC,EnemyList[i].GetX1(), EnemyList[i].GetY1(), to_string(EnemyList[i].GetEnemyScore()));
		}
	}
	
	CDDraw::ReleaseBackCDC();
}
bool CGameStateRun::IfNoEnemy() {
	for (int i = 0; i < 4; i++) {
		auto& enemy = EnemyList[i];
		if (EnemyTypeList[i] != 0 || enemy.GetTankState() != enemy.Death) {
			return false;
		}
	}
	return true;
}
bool CGameStateRun::IfResetPropTime(int NowPropIndex, GameProps NowProp) {
	GameProps::ItemType NowPropType = _Prop[NowPropIndex].GetType();
	if (NowPropIndex == _NowPropSize - 1 && _Prop[NowPropIndex].count(NowPropType) > 1
		&& (NowPropType == GameProps::ItemType::Clock
		|| NowPropType == GameProps::ItemType::Shovel
		|| NowPropType == GameProps::ItemType::Steel_helmet) ) {
		_Prop[_Prop[NowPropIndex].find(NowPropType)].SetIfCountDown(false);
		_Prop[NowPropIndex].SetIfExist(false);
		return true;
	}
	return false;
}
void CGameStateRun::TrigAllProp() {
	for (int i = _NowPropSize - 1; i > -1; i--) {
		if ((CMovingBitmap::IsOverlap(_PlayerTank.GetTankBitmap(), _Prop[i].GetPropBitmap())
			|| _Prop[i].GetIfTouched()) && _Prop[i].GetIfExist()) {
			if (IfResetPropTime(i, _Prop[i])) {
				continue;
			}
			_IfEatItem[0] = 1;
			_IfEatItem[1] = _Prop[i].GetX();
			_IfEatItem[2] = _Prop[i].GetY();
			_IfEatItem[3] = clock();
			event.TriggerGetProps(_Prop[i], Stage1, _PlayerTank, EnemyList,_EnemyNum);
		}
	}
}
void CGameStateRun::PlayerOnMove(CPlayer &Player) {
	switch (Player.GetTankState())
	{
	case CPlayer::Spawn:
		if (Player.GetIfRespawnanimationdone()) {
			Player.SetPlayerInit();
			_PlayerLife -= 1;
		}
		break;
	case CPlayer::Alive:
		if (_isHoldRightKey == true || _isHoldLeftKey == true
			|| _isHoldDownKey == true || _isHoldUpKey == true) {
			Player.TurnFace(_HoldKey);
			PlayerTankCollisionMap(&Player);
		}
		else if (_OnIceCountDown > 0) {
			Player.TurnFace(_HoldKey);
			PlayerTankCollisionMap(&Player);
			_OnIceCountDown -= 4;
		}
		break;
	case CPlayer::Death:
		if ( _PlayerLife > 0) {
			Player.SetPlayerReSpawn();
		}
		break;
	}
}
void CGameStateRun::AllEnemyOnMove() {
	for (int i = 0; i < 4; i++) {
		auto& enemy = EnemyList[i];
		switch (enemy.GetTankState())
		{
		case Enemy::Spawn:
			RandomSpawnTank(i);
			break;
		case Enemy::Alive:
			EnemyList[i].OnMove();
			EnemyTankCollisionMap(&EnemyList[i]);
			_ScoreClock[i] = clock();
			break;
		case Enemy::Death:
			enemy.TankExpolsion();
			if (_EnemyNum > 0 && clock() - enemy.GetSpawnClock() >= 2500
				&& enemy.GetIfexploded()) {
				event.TriggerUpdateMap(Stage1);
				if (_EnemyNum % 4 == 0) {
					event.TriggerReSetProps(_Prop);
					EnemyList[i].SetEnemyHaveItem(true);
				}
				enemy.SetEnemyReSpawn();
				_EnemyNum -= 1;
			}
			break;
		}
	}
}
bool CGameStateRun::BulletHitTank(CBullet CurrentBullet, CTank *BulletOwner, CTank *DetectTarget,BulletOrder Order) {
	if (CMovingBitmap::IsOverlap(CurrentBullet.GetBitmap(), DetectTarget->GetTankBitmap())
		&& DetectTarget->GetTankState() == DetectTarget->Alive) {
		BulletOwner->SetBulletStatus(Order, false);
		BulletOwner->SetIfFire(Order, false);
		return true;
	}
	return false;
}
bool CGameStateRun::BulletHitBullet(CBullet CurrentBullet, CTank *BulletOwner, CTank *DetectTarget, BulletOrder Order) {
	if (CMovingBitmap::IsOverlap(CurrentBullet.GetBitmap(), DetectTarget->GetBulletBitmap())
		&& DetectTarget->GetIfFire(1) && BulletOwner->GetIfFire(Order)) {
		_PlayerTank.SetBulletStatus(Order, false);
		_PlayerTank.SetIfFire(Order, false);
		DetectTarget->SetBulletStatus(1, false);
		DetectTarget->SetIfFire(1, false);
		return true;
	}
	return false;
}
void CGameStateRun::PlayerBulletCollision(BulletOrder Order) {
	auto& CurrentBullet = Order == FirstBullet ? _AllBullet[0] : _AllBullet[1];
	for (auto& enemy : EnemyList) {
		if (BulletHitTank(*CurrentBullet, &_PlayerTank, &enemy, Order)) {
			enemy.SetLife(0);
			if (enemy.GetEnemyHaveItem()) {
				event.TriggerSetProps(_Prop, _NowPropSize);
				enemy.SetEnemyHaveItem(false);
				_NowPropSize += 1;
			}
			_NowTotalScore += enemy.GetEnemyScore();
			break;
		}
		if (BulletHitBullet(*CurrentBullet, &_PlayerTank, &enemy, Order)) {
			break;
		}
	}
	if (ShootCollision(*CurrentBullet, _PlayerTank.GetLevel()) && CurrentBullet->GetAlreadyFire()) {
		_PlayerTank.SetBulletStatus(Order, false);
		_PlayerTank.SetIfFire(Order, false);
	}
}
void CGameStateRun::EnemyAllBulletCollision() {
	for (const auto &i : { 2,3,4,5 }) { // Enemy's bullet
		if (!_AllBullet[i]->GetAlreadyFire()) continue;
		if (BulletHitTank(*_AllBullet[i], &EnemyList[i - 2], &_PlayerTank, FirstBullet)) {
			if (!_PlayerTank.GetIfInvicible()) {
				_PlayerTank.SetLife(_PlayerTank.GetLife()-1);
			}
			continue;
		}
		if (ShootCollision(*_AllBullet[i], EnemyList[i - 2].GetLevel())) {
			EnemyList[i - 2].SetBulletStatus(1, false);
			EnemyList[i - 2].SetIfFire(1, false);
		}
	}
}
void CGameStateRun::AllBulletCollision() {
	for (const auto &i : {0,1} ) { // player's bullet
		if ( ! _AllBullet[i]->GetAlreadyFire()) continue; 
		switch (i)
		{
		case 0:
			PlayerBulletCollision(FirstBullet);
			break;
		case 1:
			PlayerBulletCollision(SecondBullet);
			break;
		}
	}
	EnemyAllBulletCollision();
}
void CGameStateRun::AllBulletFly() {
	for (int i = 0; i < 6; i++) {
		if (_AllBullet[i]->GetAlreadyFire()) {
			_AllBullet[i]->BulletFly();
		}
	}
}
bool CGameStateRun::ShootCollision(CBullet Bullet, int TankLevel) {
	if (Stage1.GetIfBoardEdge(Bullet.GetNowBackPlace()[0][0], Bullet.GetNowBackPlace()[0][1]
		, Bullet.GetHeight(), Bullet.GetWidth(), Bullet.GetDirection()) == true) {
		_tempcollision = Stage1.GetFrontGridsIndex(Bullet.GetNowFrontPlace());
		if (Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0],Stage1.CanShoot) == true
			|| Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanShoot) == true) {
			if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 4 || Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 5) {
				Stage1.ShootWall(Bullet.GetDirection(), TankLevel, _tempcollision[0][1], _tempcollision[0][0]);
			}
			if (Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 4 || Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 5) {
				Stage1.ShootWall(Bullet.GetDirection(), TankLevel, _tempcollision[1][1], _tempcollision[1][0]);
			}
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}
void CGameStateRun::PlayerTankCollisionMap(CPlayer *tank) {
	tank->TankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if (Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0],Stage1.CanWalk) &&
			Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
			if (!EnemyTankCollision(tank)){
				tank->Move();
			}
		}
		if (tank->GetIfGetShip()) {
			if ((Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 2 ||
				Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 2)) {
				if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], Stage1.CanWalk) ||
					Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
					tank->Move();
				}
				else if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 2 &&
					Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 2) {
					tank->Move();
				}
			}
		}
	}
	tank->Animation();
}
void CGameStateRun::EnemyTankCollisionMap(Enemy *tank) {
	tank->TankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if (Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], Stage1.CanWalk) &&
			Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
			if (!EnemyTankCollision(tank)) {
				tank->Move();
			}
		}
	}
	tank->Animation();
}
bool CGameStateRun::EnemyTankCollision(CTank *tank) {
	bool _collision = false;
	for (int i = 0; i < 4; i++) {
		_collision |= TankCollision(tank, &EnemyList[i]);
	}
	return _collision || TankCollision(tank, &_PlayerTank);
}
bool CGameStateRun::TankCollision(CTank *tank, CTank *who) {
	if (who->GetTankState()== who->Alive){
		_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
		switch (tank->GetOriginAngle())
		{
		case Right:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1)) {
				return true;
			}
			break;
		case Down:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1)) {
				return true;
			}
			break;
		case Left:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 2 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) + 2 && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 2 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1)) {
				return true;
			}
			break;
		case Up:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 2) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 2) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1()) + 2)) {
				return true;
			}
			break;
		}
	}
	return false;
}
void CGameStateRun::RandomSpawnTank(int num) {
	if (!EnemyList[num].GetIfRespawnanimationdone() || clock() - EnemyReSpawnLastTime[num] <= 2500) {
		return;
	}
	tempIndex = rand() % 4;							
	while (EnemyTypeList[tempIndex] == 0) {
		tempIndex = rand() % 4;
	}
	EnemyReSpawnLastTime[num] = clock();
	EnemyTypeList[tempIndex] -= 1;
	EnemyList[num].SetEnemyType(tempIndex);
	EnemyList[num].SetEnemyInit();
}
