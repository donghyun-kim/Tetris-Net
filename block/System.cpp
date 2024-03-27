#include "block.h"
#include "system.h"
#include "iostream"

using namespace std;


CSystem::CSystem()
{
	Init();
}

CSystem::~CSystem()
{
	//�� �Ҹ���...
}

//���� �����Ѵ�
void CSystem::CreateBlock()
{
	//���ο� ����� �����
	
	if(m_pNewBlock ==0)
	{
		//���� ����� ���� �´�
		m_pNewBlock = m_pBlockBuffer;
		m_pBlockBuffer =0;
		m_pBlockBuffer =new CBlock;
	}

	//�������� ���ݺ���� �߰�..
	Attack(m_AttackBlock);
	
}


//���� �·� ������
void CSystem::ChangeBlock(int key)
{

	///////////////////////////////////////////////////
	//
	//	�浹�� üũ�ϰ� ���� ���� �ǵ��� ���� ��ȭ �Ѵ�
	//

	if(key == 100)
	{
		//d
		if(m_pNewBlock != 0)
		{

			m_pNewBlock->MoveRight();
			if(colBlock())
			{
				m_pNewBlock->MoveLeft();
			}
		}
	}

	else if(key == 97)
	{
		//a
		if(m_pNewBlock != 0)
		{
			m_pNewBlock->MoveLeft();
			if(colBlock())
			{
				m_pNewBlock->MoveRight();
			}
		}
	}
	else if(key == 32)
	{
		//space
		if(m_pNewBlock != 0)
		{
			m_pNewBlock->RotateLeft();
			if(colBlock())
			{
				m_pNewBlock->RotateRight();
			}
		}
	}else if(key == 115){
		if(m_pNewBlock !=0)
		{	

			while(1)
			{
				
				m_pNewBlock->MoveDown();
				if(colBlock())
				{
					m_pNewBlock->MoveUp();
					NotMoveBlock();
					//break;
				}
			}
		}
	}



}


void CSystem::Show()
{
	//�׸���
	
	for(int i = 0 ; i < HEIGHT ; i++)
	{
		for(int j = 0 ; j< WIDTH ; j++)
		{
			//cout << m_Map[i][j] ;
			
			if(m_Map[i][j] == 0)
			{
				cout << "��";
			}
			else if(m_Map[i][j] == 1)
			{
				cout << "��" ;
			}
			else if(m_Map[i][j] == 9)
			{
				cout << "��" ;
			}
		}
		cout << endl;
	}
}







//����ʿ��� �ο��ִ� ���� ��Ҵ��� Ȯ���ϰ� ���� ����
bool CSystem::colBlock()
{
	if(!BlockAlive())
	{
		return false;
	}

	for(int i = 3; i >= 0 ; i--)
	{
		for(int j = 3 ; j >= 0 ; j--)
		{
			//���� ���� ��ϰ� �������� ����� ���� ���
			if(	m_pNewBlock->GetBlockValue(i,j) != 0 )
			{
				if(m_Map[m_pNewBlock->GetPosX() + i ][m_pNewBlock->GetPosY() + j] != 0 )
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CSystem::colSubBlock()
{
	if(!BlockAlive())
	{
		return false;
	}

	for(int i = 3; i >= 0 ; i--)
	{
		for(int j = 3 ; j >= 0 ; j--)
		{
			//���� ���� ��ϰ� �������� ����� ���� ���
			if(	m_pNewBlock->GetBlockValue(i,j) != 0 )
			{
				if(m_Map[m_pNewBlock->GetSubPosX() + i ][m_pNewBlock->GetPosY() + j] != 0 )
				{
					return true;
				}
			}
		}
	}

	return false;
}




void  CSystem::NotMoveBlock()
{
	//�����ִ� ��ϰ� �ε��� �������� ����� ��� ���� ���ȭ
	for(int i = 3 ; i >= 0 ; i--)
	{
		for(int j = 3 ; j >= 0 ; j--)
		{
			//�������� �迭�� ���� �ִ°�?
			if(m_Map[ m_pNewBlock->GetPosX() +i  ][m_pNewBlock->GetPosY() + j] == 0)
			{
				if(m_pNewBlock->GetBlockValue(i,j) != 0)
				{
					m_Map[m_pNewBlock->GetPosX() + i ][m_pNewBlock->GetPosY() + j] = m_pNewBlock->GetBlockValue(i,j);
				}
			}
		}
	}
			
	//1�� ����
	m_Score++;


	//��������� ���� ����
	delete m_pNewBlock;
	m_pNewBlock = 0;

}

void CSystem::DeleteBlock()
{
	//������ ������� Ȯ���Ѵ�
	bool chackFlag = false;

	

	//������ ����� ������ �˻�
	for(int i =  0  ; i < HEIGHT-1 ; i++)
	{
		for(int j = 1; j  < WIDTH+1 ; j++)
		{
			if(m_Map[i][j] != 0)
			{
				chackFlag = true;
			}
			else
			{
				//�ѹ��̶� �� ĭ�� �߰ߵǸ� �̰��� ���� ������
				chackFlag = false;
				break;
			}

		}

		//������ ���ο���
		if(chackFlag)
		{

			for(int j = i ;  j >= 0  ; j--)
			{
				//������ �������� ������ ��� 1�پ� �����
				for(int k = 1 ; k < WIDTH-1  ; k++)
				{
					//�迭 -1�� ���� �Ϸ� �Ҷ�
					if(j == 0)
					{
						m_Map[j][k]=0;
					}
					else
					{
						m_Map[j][k] = m_Map[j-1][k] ;
					}
				}
			}
			//���� ������ ����Ʈ ��
			m_Score += 10;
			m_DeleteLine++;
			m_AttackLine++;
		

			//���� �� ���¸� false�� �ٲٰ� ����
			chackFlag =false;
			
			break;
		}
	}
	
}

bool CSystem::CheckGameOver()
{
	
	for(int i = 1 ; i < WIDTH -1 ; i++)
	{
		//���� ������ ���� �ϳ��� ���� ����� �����Ѵٸ�..
		if(m_Map[3][i] != 0 )
		{
			return true;
			
		}
	}
	
	return false;
}

void CSystem::Init()
{
	for(int i = 0 ; i < HEIGHT ; i++)
	{
		for(int j = 0 ; j <= WIDTH ; j++)
		{
			m_Map[i][j] = 0;
			m_Map[24][j] = 9;
		}
		m_Map[i][0] = 9;
		m_Map[i][13] = 9;
			
	}

	//�������~
	m_Map[0][0]  =-1;
	m_Map[0][13] =-1;
	m_Map[1][0]  =-1;
	m_Map[1][13] =-1;
	m_Map[2][0]  =-1;
	m_Map[2][13] =-1;
	m_Map[3][0]  =-1;
	m_Map[3][13] =-1;



	//������ �ʱ�ȭ
	m_pNewBlock = 0;
	m_pBlockBuffer =0;
	m_Score =0;
	m_DeleteLine =0;
	m_Speed = 1000;
	m_Level = 1;


	
	m_Deleteing = false;
	m_AttackBlock = 0;
	m_AttackLine =0;
	
}

void CSystem::LevelManager()
{
	//���� ���μ������� ������ ���δ�
	if(m_DeleteLine >= (5*m_Level) )
	{
		m_Level++;
		
		//�ӵ��� �����Ѵ�
		if(m_Speed <= 100)
		{
			m_Speed -= 1;
		}
		else
		{
			m_Speed -= 100;
		}

	}
	
	
}

bool CSystem::BlockAlive()
{
	if(m_pNewBlock != 0)
	{
		//����� ���� �������
		return true;
	}

	//����� �׾���
	return false;
}


void CSystem::Attack(int Block)
{
	int punch = rand() % 11;
	for(int up = 0 ; up < Block ; up++)
	{
		for(int i = 3  ; i <  HEIGHT-1 ; i++)
		{
			for(int j = 1; j  < WIDTH-1 ; j++)
			{
				if(m_Map[i][j] != 0)
				{
					m_Map[i-1][j] = m_Map[i][j];
					m_Map[i][j] = 0;
				}

				if(i == 23)
				{
					if(j != (punch+1))
					{
						m_Map[i][j] = 1;
					}
				}
			}
		}
	}

	m_AttackBlock =0;
}