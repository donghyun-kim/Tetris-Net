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
	//블럭 소멸자...
}

//블럭을 생성한다
void CSystem::CreateBlock()
{
	//새로운 블록을 만든다
	
	if(m_pNewBlock ==0)
	{
		//버퍼 블록을 가져 온다
		m_pNewBlock = m_pBlockBuffer;
		m_pBlockBuffer =0;
		m_pBlockBuffer =new CBlock;
	}

	//블럭생성후 공격블록을 추가..
	Attack(m_AttackBlock);
	
}


//블럭을 좌로 돌린다
void CSystem::ChangeBlock(int key)
{

	///////////////////////////////////////////////////
	//
	//	충돌을 체크하고 이전 으로 되돌려 멈춘 블럭화 한다
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
	//그리기
	
	for(int i = 0 ; i < HEIGHT ; i++)
	{
		for(int j = 0 ; j< WIDTH ; j++)
		{
			//cout << m_Map[i][j] ;
			
			if(m_Map[i][j] == 0)
			{
				cout << "□";
			}
			else if(m_Map[i][j] == 1)
			{
				cout << "▩" ;
			}
			else if(m_Map[i][j] == 9)
			{
				cout << "■" ;
			}
		}
		cout << endl;
	}
}







//현재맵에서 싸여있는 블럭과 닿았는지 확인하고 블럭을 고정
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
			//현재 멈춘 블록과 진행중인 블록이 만난 경우
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
			//현재 멈춘 블록과 진행중인 블록이 만난 경우
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
	//굳어있는 블록과 부딛힘 진행중인 블록을 모두 굳은 블록화
	for(int i = 3 ; i >= 0 ; i--)
	{
		for(int j = 3 ; j >= 0 ; j--)
		{
			//덮어씌우려는 배열에 수가 있는가?
			if(m_Map[ m_pNewBlock->GetPosX() +i  ][m_pNewBlock->GetPosY() + j] == 0)
			{
				if(m_pNewBlock->GetBlockValue(i,j) != 0)
				{
					m_Map[m_pNewBlock->GetPosX() + i ][m_pNewBlock->GetPosY() + j] = m_pNewBlock->GetBlockValue(i,j);
				}
			}
		}
	}
			
	//1점 증정
	m_Score++;


	//다음블록을 위해 삭제
	delete m_pNewBlock;
	m_pNewBlock = 0;

}

void CSystem::DeleteBlock()
{
	//삭제할 블록인지 확인한다
	bool chackFlag = false;

	

	//삭제할 블록의 라인을 검색
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
				//한번이라도 빈 칸이 발견되면 이곳을 빠져 나간다
				chackFlag = false;
				break;
			}

		}

		//삭제할 라인에서
		if(chackFlag)
		{

			for(int j = i ;  j >= 0  ; j--)
			{
				//삭제할 라인으로 데이터 모두 1줄씩 떙긴다
				for(int k = 1 ; k < WIDTH-1  ; k++)
				{
					//배열 -1을 참조 하려 할때
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
			//라인 삭제시 포인트 업
			m_Score += 10;
			m_DeleteLine++;
			m_AttackLine++;
		

			//삭제 할 상태를 false로 바꾸고 종료
			chackFlag =false;
			
			break;
		}
	}
	
}

bool CSystem::CheckGameOver()
{
	
	for(int i = 1 ; i < WIDTH -1 ; i++)
	{
		//가장 상위의 값중 하나라도 멈춘 블록이 존재한다면..
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

	//벽만들기~
	m_Map[0][0]  =-1;
	m_Map[0][13] =-1;
	m_Map[1][0]  =-1;
	m_Map[1][13] =-1;
	m_Map[2][0]  =-1;
	m_Map[2][13] =-1;
	m_Map[3][0]  =-1;
	m_Map[3][13] =-1;



	//데이터 초기화
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
	//삭제 라인수에따라 레벨을 높인다
	if(m_DeleteLine >= (5*m_Level) )
	{
		m_Level++;
		
		//속도를 조절한다
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
		//블록이 현재 살아있음
		return true;
	}

	//블록이 죽었음
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