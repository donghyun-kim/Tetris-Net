#pragma once

class CBlock
{
protected:
	int m_block[4][4];
	int m_x;
	int m_y;
	int m_sub_x;

public:

	CBlock();
	~CBlock();

	//블록 컨트롤
	void RotateLeft();
	void RotateRight();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void MoveUp();
	void MoveSubDown();
	void MoveSubUp();


	//값가져 오기
	int GetPosX();
	int GetPosY();
	int GetSubPosX();
	void SetSubPosX(int subX);
	int GetBlockValue(int row, int col);
	int GetSubBlockValue(int row, int col);

};

