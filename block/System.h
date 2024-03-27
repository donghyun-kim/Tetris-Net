#pragma once


#include "block.h"
#define WIDTH 14  
#define HEIGHT 25


class CSystem : public CBlock
{
protected:


	
	int m_Score;
	int m_DeleteLine;
	int m_Speed;
	int m_Level;
	CBlock* m_pNewBlock;
	CBlock* m_pBlockBuffer;

	bool m_Deleteing;
	


public:
	int m_AttackBlock;
	int	m_AttackLine;
	int m_Map[HEIGHT][WIDTH];

	CSystem();
	~CSystem();

	//블럭을 생성한다
	void CreateBlock();


	//현재맵에서 싸여있는 블럭과 닿았는지 확인
	bool colBlock();
	bool colSubBlock();

	//못움직이는 블럭화
	void NotMoveBlock();

	//블록의 그린다
	void Show();
	
	//가득찬 블록을 삭제한다
	void DeleteBlock();
	
	//현재 사용중인 블록이 살아있는지 확인한다
	bool BlockAlive();

	//게임오버 여부를 확인한다
	bool CheckGameOver();

	//데이터를 초기화한다
	void Init();

	//레벨을 조정한다
	void LevelManager();


	//블록공격받음
	void Attack(int Block);
	

	
	//블럭움직인다
	void ChangeBlock(int);


};