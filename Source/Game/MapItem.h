#pragma once

namespace game_framework {
	class MapItem { //�a�Ϫ�����(��l)
	public:
		MapItem(int Type);
		int _Type;
		bool _IfShoot;  //�O�_�i�H�g�� false ���� , true�i�H
		bool _IfBreak;  //�O�_�i�H�}�a false ���� , true�i�H
		bool _IfWalk;	//�O�_�i�H���W�h(��l) false ���� , true�i�H
		CMovingBitmap _OneGrid;

		void OnShow();
	};
}