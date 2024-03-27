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

	//���� �����Ѵ�
	void CreateBlock();


	//����ʿ��� �ο��ִ� ���� ��Ҵ��� Ȯ��
	bool colBlock();
	bool colSubBlock();

	//�������̴� ��ȭ
	void NotMoveBlock();

	//����� �׸���
	void Show();
	
	//������ ����� �����Ѵ�
	void DeleteBlock();
	
	//���� ������� ����� ����ִ��� Ȯ���Ѵ�
	bool BlockAlive();

	//���ӿ��� ���θ� Ȯ���Ѵ�
	bool CheckGameOver();

	//�����͸� �ʱ�ȭ�Ѵ�
	void Init();

	//������ �����Ѵ�
	void LevelManager();


	//��ϰ��ݹ���
	void Attack(int Block);
	

	
	//�������δ�
	void ChangeBlock(int);


};