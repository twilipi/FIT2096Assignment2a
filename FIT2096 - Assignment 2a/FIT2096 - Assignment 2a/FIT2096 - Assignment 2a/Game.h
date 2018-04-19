/*	FIT2049 - Example Code
*	Game.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class is the heart of our game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Direct3D.h"
#include "Camera.h"
#include "InputController.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "GameObject.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <fstream>
#include <sstream>
#include<limits>
#undef max

using namespace std;

class Game
{
private:
	//game stuffs
	/*
	m_player: object of player
	m_mapBlocks: the array of the enemy, item, floor, wrap zone and 
	m_mapSize: the size of the whole map
	m_gameSwitch: switch for breaking the game loop
	*/
	GameObject* m_player;
	GameObject* m_mapBlocks[20][20];
	GameObject* m_HPBar;
	int m_mapSize;
	bool m_gameSwitch;


	//3D stuffs
	Camera* m_currentCam;
	Direct3D* m_renderer;
	InputController* m_input;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	Shader* m_unlitVertexColouredShader;
	Shader* m_unlitTexturedShader;

	vector<GameObject*> m_gameObjects;

	

	// Splitting initialisation up into several steps
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void InitGameWorld();

	//creating enemy, item, wrap zone and walls
	void genEnemy();
	void genItem();
	void genWrap();
	void genWall();
	void genFloor();

	//functions for generate locations
	vector<Vector3> genLocation(int genNumber);

	//identify the position and take actions
	void checkPos();

	//take battle, the output boolean will show who is the winner
		//1: player wins (cond: player remains health and enemy not)
		//3: player lose (cond: player has no health and enemy has)
	bool Battle(GameObject* player, GameObject* enemy);


public:
	Game();	
	~Game();

	//core methods
	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	void Shutdown(); //Cleanup everything we initialised

	//other methods
	bool isGameOver();//trace the game is finished or not. true: the game is over, false: can still keep going
};

#endif