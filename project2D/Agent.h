#include "Renderer2D.h"
#include "Vector2.h"
#include "Pathfinder.h"
#include "Settings.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <iostream>
#pragma once

class Agent
{
public:

	Agent(Pathfinder* _pathfinder) {
		m_pathfinder = _pathfinder;
		m_speed = DEFAULT_SPEED;
		m_distToRegen = DIST_TO_REGEN_PATH;

		srand(time(0));

		m_path = new vector<Vector2>();

		m_pos = RandPos();
		m_target = RandPos();
		m_pathfinder->Pathfind(m_pos, m_target, m_path);

		//

		m_restPos = Vector2(1, 1);
		m_energy = 40;
		m_maxEnergy = 60;
		m_restSpeed = 10;

	}

	~Agent() {

	}

	void Update(float deltatime) {
		m_energy = fmax(m_energy - deltatime, 0);

		if (m_target == m_restPos && DistFrom(m_restPos) < DIST_TO_REGEN_PATH && m_energy < m_maxEnergy) {
			m_energy += deltatime * m_restSpeed;
		}
		else {
			if (m_path->empty() || DistFrom((*m_path)[m_path->size() - 1]) < m_distToRegen) {
				if (DistFrom(m_target) < m_distToRegen || m_path->empty()) {
					if (m_energy > 0) {
						m_target = RandPos();
					}
					else {
						m_target = m_restPos;
					}
				}
				if (m_path->size() >= 2) {
					m_pathfinder->Pathfind((*m_path)[m_path->size() - 1], m_target, m_path);
				}
				else {
					m_pathfinder->Pathfind(m_pos, m_target, m_path);
				}
			}
			else {
				Vector2 direction = (*m_path)[m_path->size() - 1] - m_pos;
				direction = direction.GetNormalised();
				m_pos += direction * m_speed * deltatime;
			}
		}


	}
		
	void Render(aie::Renderer2D* _Renderer) {

		_Renderer->SetRenderColour C_AGENTOUT; // Agent 
		_Renderer->DrawCircle(m_pos.x * (NODE_GAP + 1), m_pos.y * (NODE_GAP + 1), AGENT_SIZE, 1);
		
		_Renderer->SetRenderColour C_AGENT; // Agent 
		_Renderer->DrawCircle(m_pos.x * (NODE_GAP + 1), m_pos.y * (NODE_GAP + 1), AGENT_SIZE - NODE_OUTLINE - 1, 1);

#if R_AGENTPATHNODES
		m_pathfinder->RenderPaths(m_path);
#endif

		_Renderer->SetRenderColour C_AGENTENERGY; // Energy
		_Renderer->DrawLine(m_pos.x * (NODE_GAP + 1) - 2 * AGENT_SIZE, m_pos.y * (NODE_GAP + 1) + 2 * AGENT_SIZE,
			m_pos.x * (NODE_GAP + 1) - 2 * AGENT_SIZE + (fmin(m_energy, m_maxEnergy) / m_maxEnergy) * 4 * AGENT_SIZE, m_pos.y * (NODE_GAP + 1) + 2 * AGENT_SIZE,
			AGENT_SIZE, 0);

	}

private:
	//returns distance between 2 points
	float DistFrom(Vector2 posA) {
		Vector2 posB = m_pos;
		return sqrtf((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y));
	}
	//returns random position within the map
	Vector2 RandPos() {
		return Vector2(rand() % MAP_SIZE_X, rand() % MAP_SIZE_Y);
	}
	//Moves the agent
	void MoveTo(Vector2* pos) {
		Vector2 posDelta;
	}

	//for the second action
	Vector2 m_restPos; //position to rest at
	float m_energy; //when low, rest.
	float m_maxEnergy;
	float m_restSpeed;

	Pathfinder* m_pathfinder;

	Vector2 m_pos;
	Vector2 m_target;
	vector<Vector2>* m_path;

	float m_speed;
	//How far away from its target before it calls to regenerate the path
	float m_distToRegen; 

	
};

