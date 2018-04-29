/*	FIT2049 - Example Code
*	Game.cpp
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*/

#include "Game.h"
#include "TexturedShader.h"

#include "DirectXTK/CommonStates.h"




Game::Game()
{
	m_renderer = NULL;
	m_currentCam = NULL;
	m_input = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_unlitTexturedShader = NULL;
	m_unlitVertexColouredShader = NULL;


	m_spriteBatch = NULL;
	m_arialFont12 = NULL;
	m_arialFont18 = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{
	m_renderer = renderer;	
	m_input = input;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	m_gameSwitch = true;
	m_gameModeSwitch = map;
	m_battle = NULL;
	m_lastPosition=Vector3::Zero;
	isInputing=false;

	//set every map tiles into null first
	for (int i = 0; i < MAX_MAP_SIZE; i++) {
		for (int j = 0; j < MAX_MAP_SIZE; j++) {
			m_mapBlocks[j][i] = NULL;
			m_mapObjects[j][i] = NULL;
		}
	}

	InitGameWorld();
	//lerp mode isn't move so well that will jitter when move horizontally/in a slash direction
	m_currentCam = new FPCamera(m_player, Vector3(0.0f, 0.0f, -0.25f), true, 30.0f, m_input);
	//m_currentCam = new Camera();

	//lastly, initialise sprites, do sprites here to prevent crashes due to unprepared parameters.
	LoadFonts();
	InitUI();
	RefreshUI();




	MessageBox(NULL, "Game Start!\n Reminder: please seperate the graphic and graphic interface first\nclick 'OK' to continue...", "initialize complete", MB_OK);

	return true;
}

bool Game::InitShaders()
{
	m_unlitVertexColouredShader = new Shader();
	if (!m_unlitVertexColouredShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/VertexColourPixelShader.ps"))
		return false;
	
	m_unlitTexturedShader = new TexturedShader();
	if (!m_unlitTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/TexturedPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{
	// Don't load a mesh until you've completed its "Create" function in the Mesh class
	// If you get a "Could not create window" error, you're loading an incomlpete mesh

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/player_capsule.obj")) {
		return false;
	}

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/floor_tile.obj")) {
		return false;
	}

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/wall_tile.obj")) {
		return false;
	}

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/progress_cube.obj")) {
		return false;
	}

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy.obj")) {
		return false;
	}

	
	
	return true;
}

bool Game::LoadTextures()
{
	// this will load the texture of the mesh(some of it can be shared with 1 mesh as well)

	if(!m_textureManager->Load(m_renderer, "Assets/Textures/checkerboard.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/floor.png")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/warp.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/enemy.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/item.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/enemyStrong.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/healthBar.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/wall.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/player.jpg")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redDarker.png")) {
		return false;
	}
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redLighter.png")) {
		return false;
	}
	if
		(!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png")) {
		return false;
	}
		

	return true;
}

void Game::InitGameWorld()
{
	// Create some GameObjects so we can see our custom meshes
	// We need to load the meshes we intend to use.
	// This is in Game::LoadMeshes
	/* template:
	m_gameObjects.push_back(new GameObject(
		m_meshManager->GetMesh("Assets/Meshes/player_capsule.obj"),
		m_unlitTexturedShader, 
		m_input,
		Vector3(2.0f,0.0f,-1.0f),
		m_textureManager->GetTexture("Assets/Textures/checkerboard.jpg")));

		m_gameObjects.push_back(new GameObject(
		m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),
		m_unlitTexturedShader,
		m_input,
		Vector3(2.0f,0.0f,-1.0f),
		m_textureManager->GetTexture("Assets/Textures/checkerboard.jpg")));
	*/

	//initialize map property
		//maybe have the preset map?
		//or generate a self define map with certain uniform size?<== no thanks, might be preserved the space
	m_mapSize = 20;
	GameObject::mapSize = static_cast<float>(m_mapSize);

	//initialize player
		//ask the name of the player
	string m_playerName = "Player"; //might change into a lambda function<--discard due to the D3D's weird properties


		//generate player object 
		// send the address to the gameObject

	m_player= new GameObject(
		m_meshManager->GetMesh(""),//define mesh
		m_unlitTexturedShader,//define shader
		m_input,//define input value...actually no use for non player object
		Vector3(0.0f, 0.0f, 0.0f),//define initial position
		m_textureManager->GetTexture(""),//define mesh texture
		m_playerName, "player", 10, 10, 10, 10 );//define parameters of the certain object
	m_gameObjects.push_back(m_player);
	//Assets/Textures/player.jpg  Assets/Meshes/player_capsule.obj

	//initialize arrays of non-player object
	//known bug: the random position generating function can't avoid choosing the 0,0 position when generating
	genWall();
		//initialize enemies
	genEnemy();
		//initialize items
	genItem();
		//initialize wrap zone (10 pcs)
	genWrap();
		//initialize floor
			//generate the remaining NULL array item into floors
	genFloor();

	////send the array of gameObject to the gameObjects
	//for (int z = 0; z < m_mapSize; z++) {// z:row
	//	for (int x = 0; x < m_mapSize; x++) {//x:column
	//		m_gameObjects.push_back(m_mapBlocks[x][z]);
	//	}
	//}
	
	m_HPBar = new GameObject(
		m_meshManager->GetMesh("Assets/Meshes/progress_cube.obj"),//define mesh
		m_unlitTexturedShader,//define shader
		m_input,//define input value...actually no use for non player object
		Vector3(0.0f, 0.0f, 0.0f),//define initial position
		m_textureManager->GetTexture("Assets/Textures/healthBar.jpg"),//define mesh texture
		"HPBar");//define parameters of the certain object
	//m_gameObjects.push_back(m_HPBar); 

	cout << m_player->displayStatus(true);
	m_gameSwitch = true;
}

void Game::genEnemy()
{
	//generate enemy
	/*
	preprare:
	import the name of monster and its status to mapObject array
	then generate a set of enemy location based on the number of them
	*/

	vector<string> objNames;
	vector<string> objAttrs;
	vector<int> objStat;
	fstream objFile;
	objFile.open("Assets/Database/monster.txt");
	string objLine;
	while (getline(objFile, objLine, '\n')) {
		string objData;
		vector<string> tempVector;
		istringstream tLine(objLine);
		while (getline(tLine, objData, '\t')) {
			tempVector.push_back(objData);
		}
		objNames.push_back(tempVector[0]);
		objAttrs.push_back(tempVector[1]);
		int tempArray[4] = { stoi(tempVector[2]), stoi(tempVector[3]), stoi(tempVector[4]), stoi(tempVector[5]) };
		objStat.insert( objStat.end(), tempArray, end(tempArray) );
	}
	vector<Vector3> locationList = genLocation(objNames.size());
	/*
	create:
	set those enemy to the array, then do the increment on its counter
		the powerful enemy(overall value are more than 100(?) will be used a dimmer color texture
		(be sure to notice in cmd screen)
		and the number of monsters for game loop control
	*/
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		int objIndex = i*4;
		//if enemy has much health, it will change it to a deeper color texture
		auto enemyTexture = [](int inHP) {
			if (inHP >= 50) {
				return "Assets/Textures/gradient_redDarker.png";
			}
			else {
				return "Assets/Textures/gradient_redLighter.png";
			}
			return "";
		};

		//set this thing to the object matrix
		m_mapObjects[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos) - 10.0f, 0.0f, static_cast<float>(zPos) - 10.0f),//define initial position
			m_textureManager->GetTexture(enemyTexture(objStat[objIndex + 3])),//define mesh texture
			objNames[i], objAttrs[i], objStat[objIndex], objStat[objIndex + 1], objStat[objIndex + 2], objStat[objIndex + 3]);
	}
	GameObject::m_enemyNumbers=objNames.size();


}

void Game::genItem()
{
	//generate items
	/*
	preprare:
	import the name of items and its status
	then generate a set location based on the number of them
	*/
	vector<string> objNames;
	vector<string> objAttrs;
	vector<int> objStat;
	fstream objFile;
	objFile.open("Assets/Database/item.txt");
	string objLine;
	while (getline(objFile, objLine, '\n')) {
		string objData;
		vector<string> tempVector;
		istringstream tLine(objLine);
		while (getline(tLine, objData, '\t')) {
			tempVector.push_back(objData);
		}
		objNames.push_back(tempVector[0]);
		objAttrs.push_back(tempVector[1]);
		int tempArray[4] = { stoi(tempVector[2]), stoi(tempVector[3]), stoi(tempVector[4]), stoi(tempVector[5]) };
		objStat.insert(objStat.end(), tempArray, end(tempArray));
	}

	/*
	create:
	set those item to the mapObject array
	the powerful item(overall value are more than 100(?) will be used a dimmer color texture
	(be sure to notice in cmd screen)
	*/
	vector<Vector3> locationList = genLocation(objNames.size());
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		int objIndex = i * 4;
		m_mapObjects[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/player_capsule.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos) - 10.0f, 0.0f, static_cast<float>(zPos) - 10.0f),//define initial position
			m_textureManager->GetTexture("Assets/Textures/item.jpg"),//define mesh texture
			objNames[i], objAttrs[i], objStat[objIndex], objStat[objIndex + 1], objStat[objIndex + 2], objStat[objIndex +3]);
	}
}

void Game::genWrap()
{
	//generate wrap zone
	/*
	preprare:
	generate a set of wrap location(in random)
	and then set those location into the static vector for wraping action
	*/
	vector<Vector3> locationList = genLocation(10);
	GameObject::m_warpList.clear();//clear all data of the vector first
	GameObject::m_warpList = locationList;
	
	/*
	create:
	set those warp zone to the gameObject array
	*/
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		m_mapObjects[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/player_capsule.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos)-10.0f, 0.0f, static_cast<float>(zPos)-10.0f),//define initial position
			m_textureManager->GetTexture("Assets/Textures/warp.jpg"),//define mesh texture
			"warp");
	}
}

void Game::genWall()
{
	//generate
	/*
	create border wall:
	generate a set of wall location and surrounding wall(do this first)
	*/
	for (int z = 0; z < m_mapSize; z++) {
		if (z == 0 || z == m_mapSize - 1) {
			for (int x = 0; x < m_mapSize; x++) {
				m_mapObjects[z][x] = new GameObject(
					m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
					m_unlitTexturedShader,//define shader
					m_input,//define input value...actually no use for non player object
					Vector3(static_cast<float>(x) - 10.0f, 0.0f, static_cast<float>(z) - 10.0f),//define initial position
					m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
					"wall");

			}
		}
		else {
			m_mapObjects[z][0] = new GameObject(
				m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
				m_unlitTexturedShader,//define shader
				m_input,//define input value...actually no use for non player object
				Vector3(-10.0f, 0.0f, static_cast<float>(z) - 10.0f),//define initial position
				m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
				"wall");
			m_mapObjects[z][m_mapSize - 1] = new GameObject(
				m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
				m_unlitTexturedShader,//define shader
				m_input,//define input value...actually no use for non player object
				Vector3(static_cast<float>(m_mapSize - 1) - 10.0f, 0.0f, static_cast<float>(z) - 10.0f),//define initial position
				m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
				"wall");

		}
	}
	vector<Vector3> locationList = genLocation(50);
	/*
	create random wall:
	 generate the surrounding wall of the map.
	*/
	for (int i = 0; i != locationList.size(); i++) {
		int xPos = static_cast<int>(locationList[i].x);
		int zPos = static_cast<int>(locationList[i].z);
		m_mapObjects[zPos][xPos] = new GameObject(
			m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"),//define mesh
			m_unlitTexturedShader,//define shader
			m_input,//define input value...actually no use for non player object
			Vector3(static_cast<float>(xPos) - 10.0f, 0.0f, static_cast<float>(zPos) - 10.0f),//define initial position
			m_textureManager->GetTexture("Assets/Textures/wall.jpg"),//define mesh texture
			"wall");
	}

}

void Game::genFloor()
{
	//generate floor
	//loop all the dimension of the array
	for (int z = 0; z < 20; z++) {// z:row
		float zPos = static_cast<int>(z) - 10.0f;
		for (int x = 0; x < 20; x++) {//x:column
			float xPos = static_cast<int>(x) - 10.0f;
			if (m_mapBlocks[z][x] == NULL) {// if the array item is blank, create the floor.
				m_mapBlocks[z][x] = new GameObject(
					m_meshManager->GetMesh("Assets/Meshes/floor_tile.obj"),//define mesh
					m_unlitTexturedShader,//define shader
					m_input,//define input value...actually no use for non player object
					Vector3(xPos, 0.0f, zPos),//define initial position
					m_textureManager->GetTexture("Assets/Textures/floor.png"),//define mesh texture
					"floor");//define parameters of the certain object
			}
		}
	}
}

vector<Vector3> Game::genLocation(int genNumber)
{
	vector<Vector3> outLocation;
	Vector3 randLocation;
	//generate number of UNIQUE location depends on getNumber
	for (int i = 0; i < genNumber; i++) {
		bool isUnique = true;
		//keep generate till it's unique.
		do {
			bool isUnique = true;		//reset the boolean switch to true as a default situaton
			int randX = rand() % (m_mapSize-2) +1;//generate the random x location from 1 to max size-1
			int randZ = rand() % (m_mapSize-2) +1;//generate the random z location from 1 to max size-1
			randLocation = Vector3(static_cast<float>(randX), 0.0f, static_cast<float>(randZ));//set it as the Vector.

			// check the location has no occupant to the array and avoid the starting grid, otherwise do the random position again
			//bug: sometimes it will generated at center. and I don't know why.
			if ((randZ != (m_mapSize / 2) && (randX =! (m_mapSize / 2)))//not in the center
				//&& m_mapBlocks[randZ][randX] == NULL//the place isn't occupied in the mapBlocks
				&& m_mapObjects[randZ][randX] == NULL//the place isn't occupied in the mapObjects
				) {
				//and make sure the random location are also unique each other
				for (int j = 0; j != outLocation.size(); j++) {
					if (outLocation[j] == randLocation) {
						isUnique = false;
						break;
					}
				}
			}
			else { isUnique = false; }
			/*
						if ( randZ == (m_mapSize / 2) && (randX == (m_mapSize / 2)) ) { isUnique = false; }
			//if not occupied, make sure the randomized location is unique to all set
			else if (m_mapBlocks[randZ][randX] != NULL) { isUnique = false; }
			else if (m_mapObjects[randZ][randX] != NULL) { isUnique = false; }
			else {

			}
			*/

		} while (isUnique==false);
		outLocation.push_back(randLocation);// and then add it to the output vector.
	}

	return outLocation;
}

void Game::checkPos()
{
	//getting the position and its position array

	//collect the array and its coordination(and integer) of the current position(in integer)
	float posX = m_player->GetPosition().x;
	float posZ = m_player->GetPosition().z;
	//adjust to the position to a array-friendly value
	float correctedPosX = posX - 0.5f;
	float correctedPosZ = posZ - 0.5f;
	int gridX = 0;
	int	gridZ = 0;

	//super dirty way: elimate double zeros after turning to integer with round down
	if (correctedPosX > 0) { gridX = static_cast<int>(correctedPosX) + 1; }
	else { gridX = static_cast<int>(correctedPosX); }
	if (correctedPosZ > 0) { gridZ = static_cast<int>(correctedPosZ) + 1; }
	else { gridZ = static_cast<int>(correctedPosZ); }

	int arrX = gridX + 10;
	int arrZ = gridZ + 10;
	

	//cout << arrX << " " << arrZ << "\n";

	//if position is an item: just collect the last position.
	if (m_mapBlocks[arrZ][arrX]->getAttr() == "floor") {}


	
	//if the object inside the mapObject isn't nothing, so further actions
	if (m_mapObjects[arrZ][arrX] != NULL) {
		//check the current position of player is what what kind of stuff in the m_mapObject array
		//if position is an item: adjust back the position back to normal.
		//known bug: when going to the corner of some wall pattern by entering 2 arrow key in a same time, it can sneak inside easily, if it's over the bound, it'll crash the program.
		//PS: solved
		if (m_mapObjects[arrZ][arrX]->getAttr() == "wall") {
			//check and adjust by checking their last action

			m_player->SetPosition(m_lastPosition);

			/*
			//if goes left
			if ((m_input->GetKeyUp(VK_LEFT))) {
			m_player->SetPosition(Vector3(posX + 1.0f, 0.0f, posZ));
			}

			//if goes right
			if ((m_input->GetKeyUp(VK_RIGHT))) {
			m_player->SetPosition(Vector3(posX - 1.0f, 0.0f, posZ));
			}

			//if goes up
			if ((m_input->GetKeyUp(VK_UP))) {
			m_player->SetPosition(Vector3(posX, 0.0f, posZ - 1.0f));
			}

			//if goes down
			if ((m_input->GetKeyUp(VK_DOWN))) {
			m_player->SetPosition(Vector3(posX, 0.0f, posZ + 1.0f));
			}
			*/

		}
		//if position is an item: acquire the item status and then delete itself and change to null object, then collect the last position
		else if (m_mapObjects[arrZ][arrX]->getAttr() == "item") {
			//cancel the arrow key buffer
			m_input->clearCurrentKey();
			//gain the value of the item
			m_player->gainItem(m_mapObjects[arrZ][arrX]);
			//notice player
			stringstream itemMsg;
			itemMsg << "you got a " << m_mapObjects[arrZ][arrX]->getName() << "!\n";
			itemMsg << "You gain: " << m_mapObjects[arrZ][arrX]->getAtk() << "Attack, ";
			itemMsg << m_mapObjects[arrZ][arrX]->getDef() << "Defence, ";
			itemMsg << m_mapObjects[arrZ][arrX]->getHP() << "HP\n";
			itemMsg << "Click 'OK' to continue...";
			MessageBox(NULL, itemMsg.str().c_str(), "Item found!", MB_OK);

			//then delete the object in the mapObjects
			delete m_mapObjects[arrZ][arrX];
			m_mapObjects[arrZ][arrX] = NULL;

			/*
			//and then change the attribute into floor with changing the texture (although it's dirty, it works)<--delete this line
			m_mapObjects[arrZ][arrX]->SetTexture(m_textureManager->GetTexture("Assets/Textures/floor.jpg"));
			m_mapObjects[arrZ][arrX]->SetAttr("floor");
			*/

			//update the prompt screen
			system("cls");
			cout << m_player->displayStatus(true);

			//save the last position of the player
			m_lastPosition = m_player->GetPosition();
		}
		//if position is a wrap zone: randomly choose a place to and take it to the that coordination, then collect the last position
		else if (m_mapObjects[arrZ][arrX]->getAttr() == "warp") {
			//check the location isn't the same array then the current array
			float lastWarpX = GameObject::m_lastWarp.x;
			float lastWarpZ = GameObject::m_lastWarp.z;
			int arrWarpX = static_cast<int>(GameObject::m_lastWarp.x - 0.5f) + 10;
			int arrWarpZ = static_cast<int>(GameObject::m_lastWarp.z - 0.5f) + 10;
			if (lastWarpX - 0.5f > 0) { arrWarpX += 1; }
			if (lastWarpZ - 0.5f > 0) { arrWarpZ += 1; }
			if ((arrWarpX != arrX) || (arrWarpZ != arrZ)) {
				m_player->warpLocation();
			}

			//save the last position of the player
			m_lastPosition = m_player->GetPosition();
		}
		//if position is an enemy: enter to battle
		else if (m_mapObjects[arrZ][arrX]->getAttr() == "enemy") {
			//clear the arrow key buffer
			m_input->clearCurrentKey();
			//notify player
			MessageBox(NULL, "You've found an enemy!\nIt's time to battle!!!\nClick 'OK' to continue...", "Enemy found!", MB_OK);
			//initialize the battle 
			//set the enemy address to the global memory
			m_battleEnemy[0] = arrZ;
			m_battleEnemy[1] = arrX;
			//initialize the battle
			battle(m_player, m_mapObjects[m_battleEnemy[0]][m_battleEnemy[1]]);
			//change it to battle mode
			m_gameModeSwitch = battling;
		}
	}
	//otherwise just make collect the current position
	else {
		//save the last position of the player
		m_lastPosition = m_player->GetPosition();
	}



	


	//if it's a null object, just keep update it.
}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();

	//if it's on battle
	if (m_gameModeSwitch == battling) {

		//do the battle flow.
		int battleResult = battle(m_player, m_mapObjects[m_battleEnemy[0]][m_battleEnemy[1]]);
		//take action when there's a result
		//nothing
		if (battleResult == 0) {
		}
		//player defeat enemy
		if (battleResult == 1) {
			Sleep(500);

			//delete the enemy from mapObjects
			delete m_mapObjects[m_battleEnemy[0]][m_battleEnemy[1]];
			m_mapObjects[m_battleEnemy[0]][m_battleEnemy[1]] = NULL;

			GameObject::m_enemyNumbers--;
			//if no more enemy is here, update the prompt screen
			if (GameObject::m_enemyNumbers == 0) {
				//inform the player
				Sleep(500);
				cout << "Congratulations! You've defeated all monsters!\n";
				Sleep(500);
				MessageBox(NULL, "Game Clear!\nClick 'OK' to close the application...", "", MB_OK);
				//turn off the game switch to terminate the game loop.
				m_gameSwitch = false;
			}
			//stop the battle and get back to map;
			system("pause");
			m_gameModeSwitch = map;
			system("cls");
			cout << m_player->displayStatus(true);

		}
		//enemy defeat player
		if (battleResult == 2) {
			Sleep(500);
			system("pause");
			MessageBox(NULL, "Game Over!\nClick 'OK' to close the application...", "", MB_OK);
			//turn off the game switch to terminate the game loop.
			m_gameSwitch = false;
			m_gameModeSwitch = initial;//why initial? because the enemy object still in the map and it'll still detect in the second frame, use this to hang all frame untilfinish the frame cycle.
		}
		//player escaped
		if (battleResult == 3) {
			//return player back to the previous position
			m_player->SetPosition(m_lastPosition);
			m_gameModeSwitch = map;
			system("cls");
			cout << m_player->displayStatus(true);


		}
	}
	//if it's on field
	else if (m_gameModeSwitch == map) {
		//update the player object
		/*because player need to check the surroundings has touched the wall(s) or not, so it will do the seperate action.*/
		m_player->Update(timestep);
		m_HPBar->Update(timestep);

		//update all gameobjects (only the non-player objects)
		/*for (unsigned int i = 1; i < m_gameObjects.size(); i++) {
			m_gameObjects[i]->Update(timestep);
		}*/
		for (int i = 0; i < m_mapSize; i++) {
			for (int j = 0; j < m_mapSize; j++) {
				m_mapBlocks[j][i]->Update(timestep);
				if (m_mapObjects[j][i] != NULL) {
					m_mapObjects[j][i]->Update(timestep);
				}
			}
		}

		//and then check the position has what kind of stuff around player
		checkPos();

		//check again player's HP, because it might have some HP changes when walking on the map.
		if (m_player->getHP() <= 0) {
			MessageBox(NULL, "Game Over!\nClick 'OK' to close the application...", "", MB_OK);
			m_gameSwitch = false;
		}

		//and then move the HP bar or change its length for showing the health
		m_HPBar->SetPosition(m_player->GetPosition());
		m_HPBar->changeHPBar(m_player->getHP());


		//and the camera
		//add a update variation to adapting the position between camera and player object
		//follow the player object
		//m_currentCam->SetPosition(Vector3(m_player->GetPosition().x, 7.5f, m_player->GetPosition().z - 5.0f));
		//and look at the player object
		m_currentCam->SetLookAt(m_player->GetPosition());

		// We're refreshing our UI every frame as we're assuming the distance travelled text will be changing frequently
		// Usually our UI will only need refreshing when a piece of data we're displaying changes
		RefreshUI();

		m_currentCam->Update(timestep);


	}
	

	m_input->EndUpdate();

}

void Game::Render()
{
	m_renderer->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

	// render all gameobjects
	/*for (unsigned int i = 0; i < m_gameObjects.size();i++) {
		m_gameObjects[i]->Render(m_renderer,m_currentCam);
	}*/
	for (int i = 0; i < m_mapSize; i++) {
		for (int j = 0; j < m_mapSize; j++) {
			m_mapBlocks[j][i]->Render(m_renderer, m_currentCam);
			if (m_mapObjects[j][i] != NULL) {
				m_mapObjects[j][i]->Render(m_renderer, m_currentCam);
			}
		}
	}
	m_player->Render(m_renderer, m_currentCam);
	//m_HPBar->Render(m_renderer, m_currentCam);

	//draw sprites
	DrawUI();

	m_renderer->EndScene();
}

void Game::Shutdown()
{
	// TODO delete all gameobjects

	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_unlitVertexColouredShader)
	{
		m_unlitVertexColouredShader->Release();
		delete m_unlitVertexColouredShader;
		m_unlitVertexColouredShader = NULL;
	}

	if (m_meshManager)
	{
		m_meshManager->Release();
		delete m_meshManager;
		m_meshManager = NULL;
	}

	if (m_textureManager)
	{
		m_textureManager->Release();
		delete m_textureManager;
		m_textureManager = NULL;
	}

	for (int i = 0; i != m_gameObjects.size(); i++) {
		delete m_gameObjects[i];
		m_gameObjects[i] = NULL;
	}
	
	for (int i = 0; i < m_mapSize; i++) {
		for (int j = 0; j < m_mapSize; j++) {
			if(m_mapBlocks[i][j]){ m_mapBlocks[i][j] = NULL; }
		}
	}
	
	if (m_player) { m_player = NULL; }

	if (m_HPBar) { m_HPBar = NULL; }

	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_spriteBatch)
	{
		delete m_spriteBatch;
		m_spriteBatch = NULL;
	}

	if (m_arialFont12)
	{
		delete m_arialFont12;
		m_arialFont12 = NULL;
	}

	if (m_arialFont18)
	{
		delete m_arialFont18;
		m_arialFont18 = NULL;
	}


}

bool Game::isGameOver()
{
	return !m_gameSwitch;
}

int Game::battle(GameObject * player, GameObject * enemy)
{
	int outResult = 0;
	//set a new  battle object if it's newly established.
	if (m_battle==NULL) {
		system("cls");
		m_battle = new GameBattle(player,enemy);
		//reset the input switch to false to make sure the first loop can be workable
		isInputing = false;
	}
	//otherwise, check the current turn's attacker attribute and the inputing status to take action
	else {

		// when inputing isn't started
		if (isInputing == false) {
			system("cls");
			//print the current status of the battle candidates
			cout << m_battle->getBattleStatus();
			//print current attacker is on that turn
			cout << m_battle->getCurrentAtkrMsg();
			Sleep(500);
			//if it's a human, ask for the key input
			if (m_battle->getCurrentAtkrAttr() == "player") {
				//ask player for input certain value
				cout << m_battle->askPlayer();
				//and then clear the input for better input behaviour
				m_input->clearCurrentKey();
			}
			//if it's an enemy, just show the message above and that's all
			isInputing = true;
		}
		//when input is started
		else {
			//if it's a player, keep do nothing until there's certain key is pressed
			if (m_battle->getCurrentAtkrAttr() == "player") {
				//the prompt action should be get the key to take action, but it still can't make a good behaviour to use... I can only use the random in this moment.
				
				//if player types 1,2,3,4, it will add the coorisponding action to the memory of the battle object, and stop the input loop
				//1: attack
				if (m_input->GetKeyHold('1')) {
					isInputing = false;
					m_battle->setAction(1);
					cout << 1;
				}
				//2: defence
				else if (m_input->GetKeyUp('2')) {
					isInputing = false;
					m_battle->setAction(2);
					cout << 2;
				}
				//3: aware
				else if (m_input->GetKeyUp('3')) {
					isInputing = false;
					m_battle->setAction(3);
					cout << 3;
				}
				//4: escape
				else if (m_input->GetKeyUp('4')) {
					isInputing = false;
					m_battle->setAction(4);
					cout << 4;

				}
				//others will be just break and wait for another frame to get the key instead
				else {
					//clear the input for batter next input
					//m_input->clearCurrentKey();
					//cout << "nothing\n";
					return outResult;
				}
				cout << endl;

				
				
				/*
				//randomly choose an action
				m_battle->setAction(rand() % 3 + 1);
				*/

				//and then close the input
				isInputing = false;

			}
			//if it's not a player,
			else {
				//randomly choose an action
				m_battle->setAction(rand() % 3 + 1);
				//and then close the input
				isInputing = false;
			}

			//after input, show the message for the action
			cout << endl << m_battle->getActionMsg();
			Sleep(500);

			//and then get the battle action result and do actions on response
			outResult = m_battle->takeBattle();
			//0: no result is occured
			if (outResult == 0) {
			}
			//1: player wins (cond: player remains health and enemy not)
			else if (outResult == 1) {
				cout << player->getName() << " defeated " << enemy->getName() << "!\n";
				//delete battle object
				delete m_battle;
				m_battle = NULL;
			}
			//2: player lose (cond: player has no health and enemy has)
			else if (outResult == 2) {
				cout << player->getName() << " defeated by " << enemy->getName() << "!\n";
				//delete battle object
				delete m_battle;
				m_battle = NULL;
			}
			//3: player escaped(cond: player selected the escape option)
			else if (outResult == 3) {
				cout << player->getName() << " successfully escaped!\n";
				system("pause");
				//delete battle object
				delete m_battle;
				m_battle = NULL;
			}
		}

	}
	
	//if nothing breaks before, breaks here.
	return outResult;
}

void Game::InitUI()
{
	//initialise the batch
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());

	m_currentHPSprite= m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png");

	for (int i = 0; i < MAX_HEALTH; i++) {
		m_currentHPSprites.push_back(m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png"));
	}
	m_battleInterfaceReminder = L"Reminder: if you want to input command, please click on the graphic interface first.\nIf you found 'press any key to continue', please click on the console and press keys to resolve";
	m_battleInterfaceReminder = L"Reminder: if you want to input command, please click on the graphic interface first.\nIf you found 'press any key to continue', please click on the console and press keys to resolve";
}

void Game::RefreshUI()
{
	// Ensure text in UI matches latest scores etc (call this after data changes)
	// Concatenate data into our label string using a wide string stream
	if (m_player)
	{
		std::wstringstream ss;
		ss << "HP: " << m_player->getHP();
		m_currentHPText = ss.str();

		//reuse the ss for other stringstream by clearing the data of it.
		ss.str(wstring());
		ss << "Remaining enemies: " << GameObject::m_enemyNumbers;
		m_currentEnemyText = ss.str();
	}

}

void Game::DrawUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	// Do UI drawing between the Begin and End calls

	// Here's how we draw a sprite over our game
	//m_spriteBatch->Draw(m_currentHPSprite->GetShaderResourceView(), Vector2(210, 29), Color(1.0f, 0.0f, 0.0f));

	if (m_gameModeSwitch == map) {
		//show health bar
		for (int i = 0; i < m_player->getHP(); i++) {
			int increment = 5;
			m_spriteBatch->Draw(m_currentHPSprites[i]->GetShaderResourceView(), Vector2(178 + (i*increment), 29), Color(1.0f, 0.0f, 0.0f));
		}

		// draw the HP info.
		m_arialFont18->DrawString(m_spriteBatch, m_currentHPText.c_str(), Vector2(128, 32), Color(1.0f, 1.0f, 1.0f), 0, Vector2(0, 0));
		//draw the remaining enemy number
		m_arialFont18->DrawString(m_spriteBatch, m_currentEnemyText.c_str(), Vector2(960, 32), Color(1.0f, 1.0f, 1.0f), 0, Vector2(0, 0));
	}
	else if (m_gameModeSwitch == battling) {
		m_arialFont18->DrawString(m_spriteBatch, 
			m_battleInterfaceReminder.c_str(),
		 Vector2(128, 32), Color(1.0f, 1.0f, 1.0f), 0, Vector2(0, 0));
	}


	




	m_spriteBatch->End();
}

void Game::LoadFonts()
{
	// There's a few different size fonts in there, you know
	m_arialFont12 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");
	m_arialFont18 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-18pt.spritefont");
}