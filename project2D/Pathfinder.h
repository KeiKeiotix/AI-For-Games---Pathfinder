#pragma once
#include "Node.h"
#include "Renderer2D.h"
#include "Vector2.h"
#include "Input.h"
#include "Settings.h"
#include <vector>
using namespace std;

class Pathfinder
{
public:
    Pathfinder(aie::Renderer2D* _Renderer);
    ~Pathfinder();

    bool Pathfind(Vector2 startPos, Vector2 endPos, vector<Vector2>* finalPath);
    bool Pathfind(Vector2 startPos, Vector2 endPos);


    bool DijkstrasPath(Node* nodeStart, Node* nodeEnd, vector<Vector2>* finalPath);
    bool AStar(Node* nodeStart, Node* nodeEnd, vector<Vector2>* finalPath);
    Node* GetNodeByPos(Vector2 v2Pos);

    void Render();
    void RenderPaths(vector<Vector2>* finalPath);


private:
    Node* GetNeighbour(int nCurrentX, int nCurrentY, int nNeighbour);
    void SetNeighbours(int nCurrentX, int nCurrentY);
    float CalcHeuristic(Node* node1, Node* node2);
    void ToggleNodes();

    Vector2 CameraPos();
    //To click on nodes and change what they are set to
    bool m_PoDWalls; //Place or Destroy walls
    bool m_MousePressedLF; //Mouse Pressed Last Frame


    void BubbleSort(vector<Node*>* list);

    aie::Renderer2D* m_Renderer;
    aie::Input* m_Input;

    int finalPathLength;

    vector<Node*>* m_OpenList;
    vector<Node*>* m_ClosedList;



    Node* m_pNodes[MAP_SIZE_X][MAP_SIZE_Y];
};