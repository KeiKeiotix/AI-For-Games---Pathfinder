#pragma once
#include "Renderer2D.h"
#include "Vector2.h"


#define NEIGHBOUR_COUNT 8

struct Node
{

	Node() {
		
	}

	Node(Vector2& pos, Node* pPrevious = nullptr) {
		m_position = new Vector2 (pos);
		m_pPrevious = pPrevious;
		m_isWall = false;
	}
	
	float GetX() {
		return m_position->x;
	}
	float GetY() {
		return m_position->y;
	}
	Vector2* GetPos() {
		return m_position;
	}
	void SetWall(bool _isWall) {
		m_isWall = _isWall;
	}
	bool IsWall() {
		return m_isWall;
	}


	Node* m_connections[NEIGHBOUR_COUNT];

	Vector2* m_position;
	

	bool m_isWall;

	float m_gScore;
	float m_hScore;
	float m_fScore;
	Node* m_pPrevious; // ? why does a node need a parent ?

};

