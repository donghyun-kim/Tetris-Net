#include "block.h"
#include "iostream"

using namespace std;


CBlock::CBlock()
{

	//��Ʈ���� �����...
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

	
	//���������� ����� �����
	int	createBlockNumber = rand() % 6;

	memcpy(m_block,temp[createBlockNumber] ,sizeof(m_block));

	
	//�������..
	//m_pNextBlock =0;


	//���� �⺻ ��ġ
	m_x= 0; 
	m_y= 5;
	m_sub_x=0;
}



CBlock::~CBlock()
{
}



//�������� ������
void CBlock::RotateLeft()
{
	//����� ������
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


//�������� ������
void CBlock::RotateRight()
{
	//����� ������
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
