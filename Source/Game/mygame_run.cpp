﻿#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
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
	_IfGameOver = false;
	IfGotoNextStage = false;
	_isHoldDownKey = _isHoldUpKey = _isHoldLeftKey = _isHoldRightKey = false;
	_ScoreClock = { 0,0,0,0,0,0,0,0,0};
	_MouseX = 0;
	_MouseY = 0;
	_OnIceCountDown = 0;
	_PlayerRespawnTimes = 2;
	_NowPropSize = 0;
	_EnemyReSpawnLastTime = clock();
	_GameOverSign.SetTopLeft(516, 900);
}
void CGameStateRun::PlayAudio(AUDIO_ID Id,bool IfRepeat) {
	if (!_AllAudioIfPlaying[Id]) {
		_AllAudioIfPlaying[Id] = IfRepeat;
		audio->Play(Id, IfRepeat);
	}
}
void CGameStateRun::OnMove()                            
{
	switch(state) {
		case SelectStage:
			event.TriggerSelectingStage(_Menu);
			break;
		case PreBattle:
			_PlayerRespawnTimes += 1;
			event.TriggerSetBattleMap(_AllStage[_NowStage-1],Stage1,_Menu,_PlayerTank,_Prop,EnemyList);
			EnemyTypeList.assign(_AllStageEnemy[_NowStage - 1].begin(), _AllStageEnemy[_NowStage - 1].end());
			_IfBattling = true;
			_IfGameOver = false;
			IfGotoNextStage = false;
			_IfSettling = false;
			_NowPropSize = 0;
			_EnemyExistNum = 20;
			DeadEnemyList = { 0,0,0,0 };
			break;
		case Battle:
			TriggerAllProp();
			PlayerOnMove();
			AllEnemyOnMove();
			AllBulletCollision();
			AllBulletFly();
			if (_IfGameOver) {
				SetGameOverAndGotoSettlement();
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
		PlayAudio(AUDIO_BGM,true);
		break;
	case Battle:
		if (_EnemyExistNum == 0) {
			if (!IfGotoNextStage) {
				GotoNextStageDelay = clock();
				IfGotoNextStage = true;
			}
			if (clock() - GotoNextStageDelay >= 2500) {
				_IfBattling = false;
				_IfSettling = true;
				Stage1.SetIfShowMap(false);
				event.TriggerSettlement(_Menu, _AllStageEnemy[_NowStage - 1], _NowTotalScore, _TheHighestScore, _NowStage);
				state = Settlement;
			}
		}
		IfGameOver();
		break;
	case Settlement:
		if (_IfGameOver && !_IfSettling) {
			GotoGameState(GAME_STATE_INIT);
		}
		if (!_IfSettling) {
			state = PreBattle;
		}
	}
}
void CGameStateRun::OnInit()                                  
{
	for (int i = 1;i < 18;i++) {
		string tempname = string("resources/Sound/Battle_City_SFX(");
		tempname += to_string(i) + ").wav";
		audio->Load(i-1, const_cast<char*>(tempname.c_str()));
	}
	
	for (int i = 0;i < 17;i++) {
		_AllAudioIfPlaying.push_back(false);
	}
	srand((unsigned)time(NULL));
	_Menu.LoadBitMap();
	_PlayerTank.LoadBitmap();
	_GameOverSign.LoadBitmapByString({ "resources/GameOverSign.bmp" }, RGB(0, 0, 0));
	for (int i = 0; i < 5; i++) {
		_Prop.push_back(GameProps());
		_Prop[i].OnInit();
	}
	for (int i = 0; i < 4; ++i) {
		EnemyList[i].LoadBitmap();
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
	if (_IfBattling && _PlayerTank.GetTankState() == CPlayer::Alive && !_IfGameOver) {
		if (nChar == 0x5A || nChar == 0x58) {
			if (_PlayerTank.GetLevel() > 3 && _PlayerTank.GetIfFire(1) == true) {
				_PlayerTank.FireBullet(2);
			}
			_PlayerTank.FireBullet(1);
			PlayAudio(AUDIO_FireBullet,false);
		}
		if (nChar == VK_DOWN)	_isHoldDownKey = true;
		if (nChar == VK_UP)		_isHoldUpKey = true;
		if (nChar == VK_LEFT)	_isHoldLeftKey = true;
		if (nChar == VK_RIGHT)	_isHoldRightKey = true;
		if (nChar == VK_DOWN || nChar == VK_RIGHT || nChar == VK_LEFT || nChar == VK_UP) {
			_HoldKey = nChar;
			PlayAudio(AUDIO_Move,true);
		} 
		if (nChar == 'A') {
			_IfBattling = false;
			_IfSettling = true;
			Stage1.SetIfShowMap(false);
			event.TriggerSettlement(_Menu, _AllStageEnemy[_NowStage - 1], _NowTotalScore, _TheHighestScore,_NowStage);
			state = Settlement;
		}
		if (nChar == 'L') {
			_PlayerRespawnTimes+=1;
		}
		if (nChar == 'D') {
			_PlayerTank.SetLife(0);
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
			else {
				if (_AllAudioIfPlaying[AUDIO_Move]) {
					_AllAudioIfPlaying[AUDIO_Move] = false;
					audio->Stop(AUDIO_Move);
				}
			}
		}
	}
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)    
{
	_MouseX = point.x;
	_MouseY = point.y;
}

void CGameStateRun::OnShow()
{
	_Menu.OnShow();
	if (_IfBattling) {
		Stage1.OnShow();
		_PlayerTank.OnShow();
		for (int i = 0; i < 4; i++) {
			EnemyList[i].OnShow();
		}
		if (Stage1.GetIfGrassInMap()) {
			Stage1.OnShowGrass();
		}
		if (_NowPropSize != 0) {
			_Prop[_NowPropSize-1].OnShow();
		}
		if (_IfGameOver) {
			_GameOverSign.ShowBitmap();
		}
	}
	OnShowText();
}
void CGameStateRun::OnShowScore(CDC* pDC, CFont*& fp) {
	for (int i = 0; i < 4; i++) {
		if (EnemyList[i].GetTankState() == EnemyList[i].Death && clock() - _ScoreClock[i] <= 750 && EnemyList[i].GetIfexploded()) {
			CTextDraw::ChangeFontLog(pDC, 48, "STZhongsong", RGB(255, 255, 255));
			CTextDraw::Print(pDC, EnemyList[i].GetX1(), EnemyList[i].GetY1(), to_string(EnemyList[i].GetEnemyScore()));
		}
	}
	int nowpropindex = _NowPropSize - 1;
	if (nowpropindex < 0) {
		return;
	}
	if (!_Prop[nowpropindex].GetIfTouched()) {
		_ScoreClock[4 + nowpropindex] = clock();
	}
	if (_Prop[nowpropindex].GetIfTouched() && clock() - _ScoreClock[4+nowpropindex] <= 750) {
		CTextDraw::ChangeFontLog(pDC, 48, "STZhongsong", RGB(255, 255, 255));
		CTextDraw::Print(pDC, _Prop[nowpropindex].GetX(), _Prop[nowpropindex].GetY(), to_string(500));
	}
}
void CGameStateRun::OnShowText() {
	CDC *pDC = CDDraw::GetBackCDC();
	CFont *fp;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 180, 0));
	_TimerFinish = clock();
	CTextDraw::Print(pDC, 0, 0, ("CPU Clock:"+ to_string(_TimerFinish)));

	_Menu.OnShowText(pDC, fp);
	CTextDraw::ChangeFontLog(pDC, 15, "STZhongsong", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 0, 70, ("EnemyNums:" + to_string(_EnemyExistNum)));
	//CTextDraw::Print(pDC, 0, 50, ("EatItem:"+to_string(_IfPlayerEatItem)));
	CTextDraw::Print(pDC, 0, 90, ("PlayerRespawnTimes:" + to_string(_PlayerRespawnTimes)));
	CTextDraw::Print(pDC, 0, 110, ("PlayerLife:" + to_string(_PlayerTank.GetLife())));
	CTextDraw::Print(pDC, 0, 130, ("Bullet Position:" + to_string(_AllBullet[0]->GetNowFrontPlace()[0][0]) +","+ to_string(_AllBullet[0]->GetNowFrontPlace()[0][1])));
	CTextDraw::Print(pDC, 0, 150, ("PlayerAngle:" + to_string(_PlayerTank.GetOriginAngle())));
	CTextDraw::Print(pDC, 0, 170, ("PlayerBitmapIndex:" + to_string(_PlayerTank.GetBitmapIndex())));
	CTextDraw::Print(pDC, 0, 190, ("PlayerHoleKey:" + to_string(_HoldKey)));
	CTextDraw::Print(pDC, 0, 500, ("Press L Add RespawnTimes"));
	CTextDraw::Print(pDC, 0, 520, ("Press A Jump to Next Stage"));
	CTextDraw::Print(pDC, 0, 540, ("Press D Kill Yourself"));
	if (state == Battle){
		OnShowScore(pDC, fp);
		CTextDraw::ChangeFontLog(pDC, 42, "STZhongsong", RGB(0, 0, 0));
		CTextDraw::Print(pDC, 1035, 515, (to_string(_PlayerRespawnTimes)));
		CTextDraw::Print(pDC, 1100, 755, ("1"));
	}
	
	CDDraw::ReleaseBackCDC();
}
bool CGameStateRun::IfHaveEnemy() {
	for (int i = 0; i < 4; i++) {
		if (EnemyTypeList[i] != 0) {
			return true;
		}
	}
	return false;
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
void CGameStateRun::TriggerAllProp() {
	for (int i = _NowPropSize - 1; i > -1; i--) {
		if ((CMovingBitmap::IsOverlap(_PlayerTank.GetTankBitmap(), _Prop[i].GetPropBitmap())
			|| _Prop[i].GetIfTouched()) && _Prop[i].GetIfExist()) {
			if (IfResetPropTime(i, _Prop[i])) {
				continue;
			}
			event.TriggerGetProps(_Prop[i], Stage1, _PlayerTank, EnemyList,_EnemyExistNum,_PlayerRespawnTimes,DeadEnemyList);
		}
	}
}
void CGameStateRun::IfGameOver() {
	if (_PlayerRespawnTimes == 0 && _PlayerTank.GetTankState() == CTank::Death) {
		_IfGameOver = true;
	}
}
void CGameStateRun::SetGameOverAndGotoSettlement() {
	if (clock() - GameOverClock > 5000) {
		_IfBattling = false;
		_IfSettling = true;
		Stage1.SetIfShowMap(false);
		event.TriggerSettlement(_Menu, DeadEnemyList, _NowTotalScore, _TheHighestScore, _NowStage);
		state = Settlement;
	}
	else {
		event.TriggerGameOver(_GameOverSign);
	}
}
void CGameStateRun::PlayerOnMove() {
	switch (_PlayerTank.GetTankState())
	{
	case CPlayer::Spawn:
		if (_PlayerTank.GetIfRespawnanimationdone()) {
			_isHoldDownKey = _isHoldUpKey = _isHoldLeftKey = _isHoldRightKey = false;
			_PlayerTank.SetPlayerInit();
			_PlayerRespawnTimes -= 1;
		}
		break;
	case CPlayer::Alive:
		if (_isHoldRightKey == true || _isHoldLeftKey == true
			|| _isHoldDownKey == true || _isHoldUpKey == true) {
			_PlayerTank.TurnFace(_HoldKey);
			PlayerTankCollisionMap(&_PlayerTank);
		}
		else if (_OnIceCountDown > 0) {
			_PlayerTank.TurnFace(_HoldKey);
			PlayerTankCollisionMap(&_PlayerTank);
			_OnIceCountDown -= 4;
		}
		break;
	case CPlayer::Death:
		if ( _PlayerRespawnTimes > 0) {
			_PlayerTank.SetPlayerReSpawn();
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
			enemy.OnMove();
			EnemyTankCollisionMap(&enemy);
			if (enemy.GetEnemyType()== Enemy::ArmorTank){
				enemy.OnMove();
				EnemyTankCollisionMap(&enemy);
			}
			_ScoreClock[i] = clock();
			break;
		case Enemy::Death:
			enemy.TankExpolsion();
			if ( IfHaveEnemy() && clock() - enemy.GetSpawnClock() >= 2500
				&& enemy.GetIfexploded()) {
				event.TriggerUpdateMap(Stage1);
				int RespawnEnemyNumber = (EnemyTypeList[0] + EnemyTypeList[1] + EnemyTypeList[2] + EnemyTypeList[3]);
				if (RespawnEnemyNumber % 4 == 0 && RespawnEnemyNumber != 20) {
					event.TriggerReSetProps(_Prop);
					EnemyList[i].SetEnemyHaveItem(true);
				}
				enemy.SetEnemyReSpawn();
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
			enemy.SetLife(enemy.GetLife() - 1);
			if (enemy.GetLife() <= 0) { 
				_EnemyExistNum -= 1;
				DeadEnemyList[enemy.GetEnemyType()] += 1;
				if (enemy.GetEnemyHaveItem()) {
				event.TriggerSetProps(_Prop, _NowPropSize);
				enemy.SetEnemyHaveItem(false);
				_NowPropSize += 1;
				}
			}
			_NowTotalScore += enemy.GetEnemyScore();
			PlayAudio(AUDIO_Explosion,false);
			break;
		}
		if (BulletHitBullet(*CurrentBullet, &_PlayerTank, &enemy, Order)) {
			break;
		}
	}
	if (ShootCollision(*CurrentBullet, _PlayerTank.GetLevel(),BulletOwner::Player) && CurrentBullet->GetAlreadyFire()) {
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
				GameOverClock = clock();
				PlayAudio(AUDIO_HitHomeOrPlayer, false);
			}
			continue;
		}
		if (ShootCollision(*_AllBullet[i], EnemyList[i - 2].GetLevel(),BulletOwner::Eenemy)) {
			EnemyList[i - 2].SetBulletStatus(1, false);
			EnemyList[i - 2].SetIfFire(1, false);
		}
	}
}
void CGameStateRun::AllBulletCollision() {
	for (const auto &i : {0,1} ) { // player's bullet
		if ( !_AllBullet[i]->GetAlreadyFire()) continue; 
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
bool CGameStateRun::ShootCollision(CBullet Bullet, int TankLevel,int who) {
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
			if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 7 || Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 7) {
				Stage1.SetHomeBreak();
				_IfGameOver = true;
				PlayAudio(AUDIO_HitHomeOrPlayer, false);
				GameOverClock = clock();
			}
			if (who == BulletOwner::Player){
				if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 5 || Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 5) {
					PlayAudio(AUDIO_HitIronOrBound, false);
				}
				if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 4 || Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 4) {
					PlayAudio(AUDIO_HitBrickWall, false);
				}
			}
			return true;
		}
	}
	else {
		if (who == BulletOwner::Player) {
			PlayAudio(AUDIO_HitIronOrBound, false);
		}
		return true;
	}
	return false;
}
void CGameStateRun::PlayerTankCollisionMap(CPlayer *tank) {
	tank->UpdateTankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if (Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0],Stage1.CanWalk) &&
			Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
			if (!TankCollision(tank)){
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
	tank->UpdateTankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if (Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], Stage1.CanWalk) &&
			Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
			if (!TankCollision(tank)) {
				tank->Move();
			}
		}
	}
	tank->Animation();
}
bool CGameStateRun::TankCollision(CTank *tank) {
	bool _collision = false;
	for (int i = 0; i < 4; i++) {
		if (EnemyList[i].GetX1() != tank->GetX1() || EnemyList[i].GetY1() != tank->GetY1()) {
			_collision |= IfFrontHaveTank(tank, &EnemyList[i]);
		}
	}
	if (tank->GetX1() != _PlayerTank.GetX1() || tank->GetY1() != _PlayerTank.GetY1()){
		_collision |= IfFrontHaveTank(tank, &_PlayerTank);
	}
	return _collision;
}
bool CGameStateRun::IfFrontHaveTank(CTank *tank, CTank *who) {
	if (who->GetTankState() == who->Alive){
		//這是tank判斷點的位置，為Bitmap的四個角
		vector<vector<int>> _Collision2D = { {2,0,2,2/*Right*/},{0,2,2,2/*Down*/} ,{0,0,0,2/*Left*/},{0,0,2,0/*Up*/} };
		//使用Axis-Aligned Bounding Box方法判斷碰撞
		//但是tank不是以碰撞箱作為碰撞判斷，以點作為碰撞判斷，避免tank碰撞箱重疊會無法移動
		// +1 校準位置 避免會重疊到邊界的碰撞箱
		if ((tank->GetX1() + 1 + _Collision2D[tank->GetOriginAngle()][0] * 31 <= who->GetX1() + 64 && 
			tank->GetX1() + 1 + _Collision2D[tank->GetOriginAngle()][0] * 31 >= who->GetX1()&&
			tank->GetY1() + 1 + _Collision2D[tank->GetOriginAngle()][1] * 31 <= who->GetY1() + 64 && 
			tank->GetY1() + 1 + _Collision2D[tank->GetOriginAngle()][1] * 31 >= who->GetY1())){
			return true;
		}
		if (tank->GetX1() + _Collision2D[tank->GetOriginAngle()][2] * 31 <= who->GetX1() + 64 &&
			tank->GetX1() + _Collision2D[tank->GetOriginAngle()][2] * 31 >= who->GetX1()&&
			tank->GetY1() + _Collision2D[tank->GetOriginAngle()][3] * 31 <= who->GetY1() + 64 &&
			tank->GetY1() + _Collision2D[tank->GetOriginAngle()][3] * 31 >= who->GetY1()) {
			return true;
		}
	}
	return false;
}
void CGameStateRun::RandomSpawnTank(int num) {
	if (!EnemyList[num].GetIfRespawnanimationdone() || clock() - _EnemyReSpawnLastTime <= 2500) {
		return;
	}
	int tempIndex = rand() % 4;							
	while (EnemyTypeList[tempIndex] == 0) {
		tempIndex = rand() % 4;
	}
	_EnemyReSpawnLastTime = clock();
	EnemyTypeList[tempIndex] -= 1;
	EnemyList[num].SetEnemyType(tempIndex);
	EnemyList[num].SetEnemyInit();
}
