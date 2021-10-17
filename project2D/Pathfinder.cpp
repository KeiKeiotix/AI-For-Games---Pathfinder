#include "Pathfinder.h"
#include <iostream>
#include "Settings.h"

#define UP 0
#define UPLEFT 1
#define LEFT 2
#define DOWNLEFT 3
#define DOWN 4
#define DOWNRIGHT 5
#define RIGHT 6
#define UPRIGHT 7

Pathfinder::Pathfinder(aie::Renderer2D* _Renderer) {
    Vector2 nodePos;
    for (int x = 0; x < MAP_SIZE_X; x++) {
        for (int y = 0; y < MAP_SIZE_Y; y++) {
            nodePos.Set(x, y);
            m_pNodes[x][y] = new Node(nodePos);
        }
    }
    for (int x = 0; x < MAP_SIZE_X; x++) {
        for (int y = 0; y < MAP_SIZE_Y; y++) {
            SetNeighbours(x, y);
        }
    }

    m_OpenList = new vector<Node*>();
    m_ClosedList = new vector<Node*>();

    m_Renderer = _Renderer;

    //Get input manager
    m_Input = aie::Input::GetInstance();

}

Pathfinder::~Pathfinder() {


}

bool Pathfinder::Pathfind(Vector2 startPos, Vector2 endPos, vector<Vector2>* finalPath) {
    Node* nodeStart = GetNodeByPos(startPos);
    Node* nodeEnd = GetNodeByPos(endPos);
    if (nodeStart != nullptr && nodeEnd != nullptr) {
        //return DijkstrasPath(nodeStart, nodeEnd, finalPath);
        return AStar(nodeStart, nodeEnd, finalPath);
    }
    return false;
}

bool Pathfinder::Pathfind(Vector2 startPos, Vector2 endPos) {
    vector<Vector2>* finalPath;
    Node* nodeStart = GetNodeByPos(startPos);
    Node* nodeEnd = GetNodeByPos(endPos);
    if (nodeStart != nullptr && nodeEnd != nullptr) {
        return DijkstrasPath(nodeStart, nodeEnd, finalPath);
    }
    return false;
}

float Pathfinder::CalcHeuristic(Node* node1, Node* node2) {
    return (sqrtf((node1->GetX() - node2->GetX()) * (node1->GetX() - node2->GetX()) + (node1->GetY() - node2->GetY()) * (node1->GetY() - node2->GetY())));
}

bool Pathfinder::AStar(Node* nodeStart, Node* nodeEnd, vector<Vector2>* finalPath)
{

    int currentStep = 0;


    //Find start and end nodes GetNodeByPos

    //Clear finalPath
    finalPath->clear();

    //Check for invalid path
    if (nodeStart == nullptr)
        return false;
    if (nodeEnd == nullptr)
        return false;
    //if (nodeEnd->m_bBlocked)
    //    return false;

    //Clear open list
    m_OpenList->clear();

    //Clear closed list
    m_ClosedList->clear();

    //Reset start nodes scores and pPrev
    nodeStart->m_gScore = 0;
    nodeStart->m_hScore = 0;
    nodeStart->m_fScore = 0;
    nodeStart->m_pPrevious = nullptr;

    //Add start node to open list
    m_OpenList->push_back(nodeStart); //"openList.push_back() used in vid - check here when there is an error with order.

    //While the open list is not empty
    while (m_OpenList->empty() == false)
    {
        //Get cheapest node from open list and call it Current. 
       // m_OpenList
        BubbleSort(m_OpenList);
        Node* nodeCurrent = (*m_OpenList)[0];

        //Also remove it from the open list
        m_OpenList->erase(m_OpenList->begin());

        //Add Current to closed list
        m_ClosedList->push_back(nodeCurrent);

        //If Current is equal to the end node then path has been found
        if (nodeCurrent == nodeEnd)
        {
            //Starting at the end node, loop back through all pPrev nodes and add them
            //to final path
            int it = 0;
            while (nodeEnd->m_pPrevious != nullptr) {
                finalPath->push_back(*nodeEnd->GetPos());
                it++;
                nodeEnd = nodeEnd->m_pPrevious;
            }

            //We found the path
            return true;
        }

        //Loop through all neighbours
        for (int n = 0; n < NEIGHBOUR_COUNT; ++n)
        {
            //Get neighbour
            Node* nodeNeighbour = nodeCurrent->m_connections[n];//GetNeighbour(nodeCurrent->GetX(), nodeCurrent->GetY(), n);

            //Skip neighbour if it's null
            if (nodeNeighbour == nullptr) {
                continue;
            }
            //Skip neighbour if it's blocked
            if (nodeNeighbour->IsWall() == true) {
                continue;
            }

#if DISABLE_DIAGONAL
            if (n % 2 != 0) {
                continue;
            }
#endif //DISABLE_DIAGONAL

            //Skip neighbour if it's in the closed list            
            if (find(m_ClosedList->begin(), m_ClosedList->end(), nodeNeighbour) != m_ClosedList->end()) {
                continue;
            }


                    //n when even (0, 2, 4, 6) is vertical or horizontal,
                    //when odd (1, 3, 5, 7) it is a diagonal connection
                float addedGScore = (((n % 2) == 0) ? 1 : 1.4f);
                float addedHScore = CalcHeuristic(nodeNeighbour, nodeEnd);
                float addedFScore = (addedGScore * GSCORE_WEIGHT) + (addedHScore * HSCORE_WEIGHT);

            //Check if neighbour is in open list
            if (find(m_OpenList->begin(), m_OpenList->end(), nodeNeighbour) != m_OpenList->end()) {

                //Check if the new path's F score is less than the neighbours F score
                if (nodeCurrent->m_fScore + addedFScore < nodeNeighbour->m_fScore) {
                    //If so, update the neighbours f score and Prev

                    nodeNeighbour->m_fScore = nodeCurrent->m_fScore + addedFScore;
                    nodeNeighbour->m_pPrevious = nodeCurrent;
                }

            }
            //else
            else {

                //Calculate the neighbours F score and Prev 
                nodeNeighbour->m_fScore = nodeCurrent->m_fScore + addedFScore;
                //Add neighbour to open list
                m_OpenList->push_back(nodeNeighbour);
                nodeNeighbour->m_pPrevious = nodeCurrent;
            }
        }
    }

    //Ran out of nodes and didn't find the path
    std::cout << "No path" << std::endl;
    return false;
}

bool Pathfinder::DijkstrasPath(Node* nodeStart, Node* nodeEnd, vector<Vector2>* finalPath)
{

    int currentStep = 0;


    //Find start and end nodes GetNodeByPos

    //Clear finalPath
    finalPath->clear();

    //Check for invalid path
    if (nodeStart == nullptr)
        return false;
    if (nodeEnd == nullptr)
        return false;
    //if (nodeEnd->m_bBlocked)
    //    return false;

    //Clear open list
    m_OpenList->clear();

    //Clear closed list
    m_ClosedList->clear();

    //Reset start nodes G score and pPrev
    nodeStart->m_fScore = 0;
    nodeStart->m_pPrevious = nullptr;

    //Add start node to open list
    m_OpenList->push_back(nodeStart); //"openList.push_back() used in vid - check here when there is an error with order.

    //While the open list is not empty
    while (m_OpenList->empty() == false)
    {
        //Get cheapest node from open list and call it Current. 
       // m_OpenList
        BubbleSort(m_OpenList);
        Node* nodeCurrent = (*m_OpenList)[0];

        //Also remove it from the open list
        m_OpenList->erase(m_OpenList->begin());

        //Add Current to closed list
        m_ClosedList->push_back(nodeCurrent);

        //If Current is equal to the end node then path has been found
        if (nodeCurrent == nodeEnd)
        {
            //Starting at the end node, loop back through all pPrev nodes and add them
            //to final path
            int it = 0;
            while (nodeEnd->m_pPrevious != nullptr) {
                finalPath->push_back(*nodeEnd->GetPos());
                it++;
                nodeEnd = nodeEnd->m_pPrevious;
            }

            //We found the path
            return true;
        }

        //Loop through all neighbours
        for (int n = 0; n < NEIGHBOUR_COUNT; ++n)
        {
            //Get neighbour
            Node* nodeNeighbour = nodeCurrent->m_connections[n];//GetNeighbour(nodeCurrent->GetX(), nodeCurrent->GetY(), n);

            //Skip neighbour if it's null
            if (nodeNeighbour == nullptr) {
                continue;
            }
            //Skip neighbour if it's blocked
            if (nodeNeighbour->IsWall() == true) {
                continue;
            }

            //Skip neighbour if it's in the closed list            
            if (find(m_ClosedList->begin(), m_ClosedList->end(), nodeNeighbour) != m_ClosedList->end()) {
                continue;
            }


            //Check if neighbour is in open list
            if (find(m_OpenList->begin(), m_OpenList->end(), nodeNeighbour) != m_OpenList->end()) {

                //Check if the new path's G score is less than the neighbours G score
                if (nodeCurrent->m_fScore + (((n % 2) == 0) ? 1 : 1.4f) < nodeNeighbour->m_fScore) {
                //If so, update the neighbours G score and Prev
                nodeNeighbour->m_fScore = nodeCurrent->m_fScore + (((n % 2) == 0) ? 1 : 1.4f);
                                            //n when even (0, 2, 4, 6) is vertical or horizontal,
                                            //when odd (1, 3, 5, 7) it is a diagonal connection
                nodeNeighbour->m_pPrevious = nodeCurrent;
                
                }

            }
            //else
            else {

                //Calculate the neighbours G score and Prev 
                nodeNeighbour->m_fScore = nodeCurrent->m_fScore + (((n % 2) == 0) ? 1 : 1.4f);
                //Add neighbour to open list
                m_OpenList->push_back(nodeNeighbour);
                nodeNeighbour->m_pPrevious = nodeCurrent;
            }
        }
    }

    //Ran out of nodes and didn't find the path
    std::cout << "No path" << std::endl;
    return false;
}

Node* Pathfinder::GetNodeByPos(Vector2 v2Pos) {
    if (MAP_SIZE_X > v2Pos.x && v2Pos.x >= 0 && MAP_SIZE_Y > v2Pos.y && v2Pos.y >= 0) {
        return m_pNodes[(int)v2Pos.x][(int)v2Pos.y];
    }
    else {
        return nullptr;
    }
}

//old renderer

//void Pathfinder::Render() {
//
//    ToggleNodes();
//
//#if R_NODES
//    for (int x = 0; x < MAP_SIZE_X; x++) {
//        for (int y = 0; y < MAP_SIZE_Y; y++) {
//            if (m_pNodes[x][y]->IsWall()) {
//                m_Renderer->SetRenderColour C_WALLSOUT;
//            } else
//                m_Renderer->SetRenderColour C_NODEOUT; //Node Outline
//
//            m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE, 5);
//#if R_PATHFINDSPREAD
//            if (find(m_ClosedList->begin(), m_ClosedList->end(), m_pNodes[x][y]) != m_ClosedList->end()) {
//                m_Renderer->SetRenderColour C_PNODE; // Processed Nodes
//            }
//            else
//#endif //R_PATHFINDSPREAD
//                m_Renderer->SetRenderColour C_UNODE; // Unprocessed Node
//
//            if (m_pNodes[x][y]->IsWall()) {
//                m_Renderer->SetRenderColour C_WALLS; //Wall
//            }
//
//            m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE - NODE_OUTLINE, 3);
//#endif //R_NODES
//
//#if R_CONNECTIONS
//            for(int c = 0; c < NEIGHBOUR_COUNT; c++) {
//                if (m_pNodes[x][y]->m_connections[c] != nullptr) {
//                   
//                    m_Renderer->SetRenderColour C_LINE; // Line
//                    m_Renderer->SetRenderColour(0.8, 0.8, 0.8);
//                    m_Renderer->DrawLine((m_pNodes[x][y]->GetX()) * (NODE_GAP + 1),
//                        (m_pNodes[x][y]->GetY()) * (NODE_GAP + 1),
//                        m_pNodes[x][y]->m_connections[c]->GetX() * (NODE_GAP + 1),
//                        m_pNodes[x][y]->m_connections[c]->GetY() * (NODE_GAP + 1), 2, 7);
//                }
//            }
//#endif //R_CONNECTIONS
//        }
//    }
//}

void Pathfinder::Render() {

    ToggleNodes();

    for (int x = 0; x < MAP_SIZE_X; x++) {
        for (int y = 0; y < MAP_SIZE_Y; y++) {

#if R_NODES
            //Render walls
            if (m_pNodes[x][y]->IsWall()) {
                m_Renderer->SetRenderColour C_WALLSOUT;
                m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE, 5);
                m_Renderer->SetRenderColour C_WALLS;
                m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE - NODE_WALLOUTLINE, 3);
            }
            else 
#if R_PATHFINDSPREAD
                //Render nodes that are "processed" within finding the path
                if (find(m_ClosedList->begin(), m_ClosedList->end(), m_pNodes[x][y]) != m_ClosedList->end()) //if is in closedList
                {
                    m_Renderer->SetRenderColour C_NODEOUT;
                    m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE, 5);
                    m_Renderer->SetRenderColour C_PNODE;
                    m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE - NODE_OUTLINE, 3);
                }
                else
#endif //R_PATHFINDSPREAD
                {
                    //Render all other nodes (included "processed" if they aren't being drawn)
                    m_Renderer->SetRenderColour C_NODEOUT;
                    m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE, 5);
                    m_Renderer->SetRenderColour C_UNODE;
                    m_Renderer->DrawCircle(m_pNodes[x][y]->GetX() * (NODE_GAP + 1), m_pNodes[x][y]->GetY() * (NODE_GAP + 1), NODE_SIZE - NODE_OUTLINE, 3);
                }
#endif //R_NODES

#if R_CONNECTIONS //Render connections between nodes
            for (int c = 0; c < NEIGHBOUR_COUNT; c++) {
                if (m_pNodes[x][y]->m_connections[c] != nullptr) {

                    m_Renderer->SetRenderColour C_LINE; // Line
                    m_Renderer->SetRenderColour(0.8, 0.8, 0.8);
                    m_Renderer->DrawLine((m_pNodes[x][y]->GetX()) * (NODE_GAP + 1),
                        (m_pNodes[x][y]->GetY()) * (NODE_GAP + 1),
                        m_pNodes[x][y]->m_connections[c]->GetX() * (NODE_GAP + 1),
                        m_pNodes[x][y]->m_connections[c]->GetY() * (NODE_GAP + 1), 2, 7);
                }
            }
#endif //R_CONNECTIONS
        }
    }
}


void  Pathfinder::RenderPaths(vector<Vector2>* finalPath) {
    m_Renderer->SetRenderColour C_PATH; // Path

    for (int i = 0; i < finalPath->size(); i++) {
        //std::cout << i;
        //std::cout << ": " << m_finalPath[i]->x << " + " << m_finalPath[i]->y << std::endl;

        m_Renderer->DrawCircle((*finalPath)[i].x * (NODE_GAP + 1), (*finalPath)[i].y * (NODE_GAP + 1), NODE_SIZE - NODE_OUTLINE, 2);
    }
}

// UP - 0 LEFT - 2 DOWN - 4 RIGHT  - 6
Node* Pathfinder::GetNeighbour(int nCurrentX, int nCurrentY, int nNeighbour) {
    if (nNeighbour == UP) {
        if (nCurrentY <= 0) {
            return nullptr;
        }
        return m_pNodes[nCurrentX][nCurrentY - 1];
    }
    if (nNeighbour == UPLEFT) {
        if (nCurrentY <= 0 || nCurrentX <= 0) {
            return nullptr;
        }
        return m_pNodes[nCurrentX - 1][nCurrentY - 1];
    }
    if (nNeighbour == LEFT) {
        if (nCurrentX <= 0) {
            return nullptr;
        }
        return m_pNodes[nCurrentX - 1][nCurrentY];
    }
    if (nNeighbour == DOWNLEFT) {
        if (nCurrentX <= 0 || nCurrentY >= MAP_SIZE_Y - 1) {
            return nullptr;
        }
        return m_pNodes[nCurrentX - 1][nCurrentY + 1];
    }
    if (nNeighbour == DOWN) {
        if (nCurrentY >= MAP_SIZE_Y - 1) {
            return nullptr;
        }
        return m_pNodes[nCurrentX][nCurrentY + 1];
    }
    if (nNeighbour == DOWNRIGHT) {
        if (nCurrentY >= MAP_SIZE_Y - 1 || nCurrentX >= MAP_SIZE_X - 1) {
            return nullptr;
        }
        return m_pNodes[nCurrentX + 1][nCurrentY + 1];
    }
    if (nNeighbour == RIGHT) {
        if (nCurrentX >= MAP_SIZE_X - 1) {
            return nullptr;
        }
        return m_pNodes[nCurrentX + 1][nCurrentY];
    }
    if (nNeighbour == UPRIGHT) {
        if (nCurrentY <= 0 || nCurrentX >= MAP_SIZE_X - 1) {
            return nullptr;
        }
        return m_pNodes[nCurrentX + 1][nCurrentY - 1];
    }
   // std::cout << "oops" << std::endl;
    return nullptr;
}

void Pathfinder::SetNeighbours(int nCurrentX, int nCurrentY) {
    for (int c = 0; c < NEIGHBOUR_COUNT; c++) {
        m_pNodes[nCurrentX][nCurrentY]->m_connections[c] = (GetNeighbour(nCurrentX, nCurrentY, c));
    }
}

void Pathfinder::ToggleNodes() {
    Vector2 cameraPos = CameraPos();
    Vector2 mousePos = Vector2(m_Input->GetMouseX() + cameraPos.x, m_Input->GetMouseY() + cameraPos.y);

    //Mouse Effective Position (lines up to "mouse position is at 1 if over node 1"
    Vector2 mouseEPos = Vector2((int)(mousePos.x / (NODE_GAP + 1) + 0.5), (int)(mousePos.y / (NODE_GAP + 1) + 0.5));
    //cout << mouseEPos.x << ", " << mouseEPos.y << endl;


    if (m_Input->IsMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT)) {
        if (mouseEPos.x >= 0 && mouseEPos.y >= 0 &&
            mouseEPos.x < MAP_SIZE_X && mouseEPos.y < MAP_SIZE_Y) {
            if (!m_MousePressedLF) {
                m_MousePressedLF = true;
                m_PoDWalls = !m_pNodes[(int)mouseEPos.x][(int)mouseEPos.y]->m_isWall;
            }
            m_pNodes[(int)mouseEPos.x][(int)mouseEPos.y]->m_isWall = m_PoDWalls;
        }
    }
    else {
        m_MousePressedLF = false;
    }

}

Vector2 Pathfinder::CameraPos() {
    Vector2 camPos;
    m_Renderer->GetCameraPos(camPos.x, camPos.y);
    return camPos;
}

void Pathfinder::BubbleSort(vector<Node*>* list)
{
    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (int i = 0; i < list->size() - 1; i++)
        {
            if ((*list)[i]->m_fScore > (*list)[i + 1]->m_fScore)
            {
                sorted = false;
                Node* temp = (*list)[i];
                (*list)[i] = (*list)[i + 1];
                (*list)[i + 1] = temp;

            }
        }
    }
}