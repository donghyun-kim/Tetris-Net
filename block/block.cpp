#include "block.h"
#include "iostream"

using namespace std;


CBlock::CBlock()
{

	//테트리스 블럭모양...
	int temp[7][4][4] = {  
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}  } ,
		{
			{0,0,0,0},
			{0,2,2,0},
			{0,2,2,0},
			{0,0,0,0}
					   },
		{
			{0,0,0,0},
			{0,3,3,3},
			{0,3,0,0},
			{0,0,0,0}
					   },
		{
			{0,0,0,0},
			{0,4,4,4},
			{0,0,0,4},
			{0,0,0,0}
					   },
		{
			{0,0,0,0},
			{0,0,5,0},
			{0,5,5,0},
			{0,5,0,0}
					   },
		{
			{0,0,0,0},
			{0,6,0,0},
			{0,6,6,0},
			{0,0,6,0}
					   },
		{
			{0,0,0,0},
			{0,0,7,0},
			{0,7,7,0},
			{0,0,7,0}
					   }

						};

	
	//블럭을생성시 모양을 만든다
	int	createBlockNumber = rand() % 6;

	memcpy(m_block,temp[createBlockNumber] ,sizeof(m_block));

	
	//다음블록..
	//m_pNextBlock =0;


	//블럭의 기본 위치
	m_x= 0; 
	m_y= 5;
	m_sub_x=0;
}



CBlock::~CBlock()
{
}



//좌측으로 돌리기
void CBlock::RotateLeft()
{
	//블록을 돌려라
	int temp[4][4] = {0,};
	int tempCount = 3;
	for(int i = 0 ; i <= 3 ; i++)
	{
		for(int j = 3 ; j >= 0 ; j--)
		{
			temp[i][3-j] = m_block[j][i];
			tempCount--;
		}	
	}


	memcpy(m_block,temp,sizeof(temp));

}


//우측으로 돌리기
void CBlock::RotateRight()
{
	//블록을 돌려라
	int temp[4][4] = {0,};
	int tempCount = 3;
	
	for(int i = 3 ; i >= 0 ; i--)
	{
		for(int j = 3 ; j >= 0 ; j--)
		{
			temp[3-i][3-j] = m_block[3-j][i];
			tempCount--;
		}	
	}

	memcpy(m_block,temp,sizeof(temp));

}

void CBlock::MoveLeft()
{
	m_y--;
}

void CBlock::MoveRight()
{
	m_y++;
}

void CBlock::MoveDown()
{
	m_x++;
}

void CBlock::MoveUp()
{
	m_x--;
}

void CBlock::MoveSubDown()
{
	m_sub_x++;
}

void CBlock::MoveSubUp()
{
	m_sub_x--;
}

int CBlock::GetPosX()
{
	return m_x;
}

int CBlock::GetPosY()
{
	return m_y;
}

int CBlock::GetSubPosX()
{
	return m_sub_x;
}

void CBlock::SetSubPosX(int subX)
{
	m_sub_x = subX;
}

int CBlock::GetBlockValue(int row, int col)
{
	return m_block[row][col];
}

int CBlock::GetSubBlockValue(int row, int col)
{
	return 0;
}
