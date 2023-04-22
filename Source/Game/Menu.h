#pragma once

namespace game_framework {
	class Menu {
	public:
		Menu();
		enum MenuType {
			LobbyMenu, 
			ChooseStageMenu,
			BattleMenu,
			SettleMenu //ChoosingStage's work include the onshow gray in battleing
		};
		int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnShow();
		void OnShowText(CDC *pDC, CFont* &fp);

		void SetMenuType(MenuType Type);
		void SetMenuRaise(MenuType Type);
		void SetSelecting(bool select);
		void SetIfLobbyMenuing(bool menu);
		void SetIfAnimation(bool Status);
		void SetIfChoosingStage(bool Status);
		void SetSettlement(vector<int>& EnemyNum, vector<int>& EnemyScore, int& NowScore, int& TheHighestScore,int& NowStage);

		bool GetIfSelecting();
		bool GetIfLobbyMenuing();
		bool GetChoosingStage();
		bool GetIfAnimation();
		int GetMenuY(MenuType Type); // type = 0 Lobby Menu,type = 1 GrayScreen(Top) 
		
		void LoadBitMap();
	private:
		int tempselect; //option arrow locate (temp,not the final location)
		int finalselect;
		vector<int> _CountNumber;
		int _NowCountTank;
		vector<vector<int>> _Dialog;
		int _MenuType;

		bool _IfAnimation;
		bool _IfSelecting; // If Selecting (in Main Menu)
		bool _IfLobbyMenuing; // If Menuing (in Main Menu)
		bool _IfSettlement;
		bool _IfChoosingStage; // Choosing Stage will appear gray Menu and let player choose stage 
		CMovingBitmap _BattleMenuGray; //GrayMenu(background)
		CMovingBitmap _ChooseStageMenuTop; //GrayMenu from Top
		CMovingBitmap _ChooseStageMenuDown; //GrayMenu from Down
		CMovingBitmap _Settlementdivided;
		vector<CMovingBitmap> _SettlementMenuEnemy;
		vector<CMovingBitmap> _SettleArrow;

		int _RaiseSpeed; // Lobby raise speed (animation)
		int _RaiseDelay; // Lobby raise animation delay
		clock_t _Last_time; // Lobby animation need now time to cal delay
		CMovingBitmap _Menu;
		CMovingBitmap _Arrow;
	};

}