#pragma once

#define MAP_SIZE_X 30
#define MAP_SIZE_Y 30

//Agent speed
#define DEFAULT_SPEED 3.5
//Distance agent needs to be from next node to regenerate path
#define DIST_TO_REGEN_PATH 0.1

//If agent can go diagonal
#define DISABLE_DIAGONAL false

//Agent Size
#define AGENT_SIZE 12

//Gap between nodes
#define NODE_GAP 20
//Size of nodes
#define NODE_SIZE 10
//Node Outline Thickness
#define NODE_OUTLINE 2
#define NODE_WALLOUTLINE 4


//GScoreStrength
#define GSCORE_WEIGHT 5
//HScoreStrength
#define HSCORE_WEIGHT 1


// -=-=-=- COLOURS -=-=-=- || Colour_Purpose
#define _RGB(r,g,b) (r/256.0f, g/256.0f, b/256.0f)

//The lines connecting nodes
#define C_LINE _RGB(91, 93, 93)
//Walls
#define C_WALLS _RGB(180, 180, 210) 
//Walls Outline
#define C_WALLSOUT _RGB(35, 39, 56)

//Processed Node
#define C_PNODE _RGB(0, 119, 172)
//Unprocessed Node
#define C_UNODE _RGB(30, 82, 136)
//Node Outline
#define C_NODEOUT _RGB(138, 152, 188)
//Path the agent takes
#define C_PATH  _RGB(239, 32, 58)
//Agent
#define C_AGENT _RGB(196, 2, 171)
//Agent Secondary State
#define C_AGENT2 _RGB(255, 89, 90) 
//Agent Outline
#define C_AGENTOUT _RGB(28, 15, 19) 
//Agent Energy Bar
#define C_AGENTENERGY _RGB(50, 205, 60);


//Background
#define C_BACKGROUND _RGB(18, 0, 35)

// -=-=-=- Enable/Disable Rendering -=-=-=-
#define R_CONNECTIONS false
#define R_NODES true
#define R_PATHFINDSPREAD true
#define R_AGENTPATHNODES true
#define R_AGENTPATHLINES //Not implemented