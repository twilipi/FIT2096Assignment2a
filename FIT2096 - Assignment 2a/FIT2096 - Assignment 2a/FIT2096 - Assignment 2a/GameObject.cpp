#include "GameObject.h"





GameObject::GameObject(Mesh * mesh, Shader * shader, InputController * input, Vector3 position)
{

	m_mesh= mesh;
	Texture* m_texture;
	m_shader= shader;
	m_input=input;
	m_texture = NULL;

	m_rotX = m_rotY = m_rotZ = 0.0f;
	SetUniformScale(1.0f);

	m_moveSpeed=1.5f;
	m_rotateSpeed=1.0f;

}

//this constructor will be used for making the NULL mesh object
GameObject::GameObject(Mesh * mesh, Shader * shader, InputController * input, Vector3 position, Texture * texture)
{
	m_mesh = mesh;
	m_shader = shader;
	m_input = input;
	m_position = position;
	m_texture = texture;

	m_rotX = m_rotY = m_rotZ = 0.0f;
	SetUniformScale(1.0f);

	m_moveSpeed = 1.5f;
	m_rotateSpeed = 1.0f;
}

//this line will be used for manuplating the mesh for the player, enemy and item
GameObject::GameObject(Mesh* mesh, Shader* shader, InputController* input, Vector3 position, Texture* texture, 
	string name, string attr, int atk, int def, int speed, int HP) {
	m_mesh = mesh;
	m_shader = shader;
	m_input = input;
	m_position = position;
	m_texture = texture;

	m_name = name;
	m_attr = attr;
	m_atk = atk;
	m_def = def;
	m_speed = speed;
	m_HP = HP;

	m_rotX = m_rotY = m_rotZ = 0.0f;
	SetUniformScale(1.0f);

	m_moveSpeed = 1.5f;
	m_rotateSpeed = 1.0f;
}

//this constructor will be used for making the floor, wall, wrap zone mesh object
GameObject::GameObject(Mesh * mesh, Shader * shader, InputController * input, Vector3 position, Texture * texture, string attr)
{
	m_mesh = mesh;
	m_shader = shader;
	m_input = input;
	m_position = position;
	m_texture = texture;

	m_attr = attr;

	m_rotX = m_rotY = m_rotZ = 0.0f;
	SetUniformScale(1.0f);

	m_moveSpeed = 1.5f;
	m_rotateSpeed = 1.0f;

}

GameObject::~GameObject()
{
}

void GameObject::Update(float timestep)
{

	// Rotation (so we can see all parts of our custom meshes)
	/*
		if (m_input->GetKeyHold('Q'))
	{
		m_rotZ -= m_rotateSpeed* timestep;
	}
	if (m_input->GetKeyHold('E'))
	{
		m_rotZ += m_rotateSpeed* timestep;
	}
	*/

	// Translation along the world axes
	//only player can translate in this game

	if (m_attr == "player") {
		if (m_input->GetKeyHold(VK_LEFT))
		{
			// Note you can instead use Vector3::Left etc. as a shorthand when declaring unit vectors
			m_position += Vector3(-1.0f, 0.0f, 0.0f) * m_moveSpeed * timestep;
		}
		if (m_input->GetKeyHold(VK_RIGHT))
		{
			m_position += Vector3(1.0f, 0.0f, 0.0f) * m_moveSpeed * timestep;
		}
		if (m_input->GetKeyHold(VK_UP))
		{
			m_position += Vector3(0.0f, 0.0f, 1.0f) * m_moveSpeed * timestep;
		}
		if (m_input->GetKeyHold(VK_DOWN))
		{
			m_position += Vector3(0.0f, 0.0f, -1.0f) * m_moveSpeed * timestep;

		}
	}
	
}

void GameObject::Render(Direct3D * renderer, Camera * cam)
{

	if (m_mesh) {
		m_world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * Matrix::CreateFromYawPitchRoll(m_rotX, m_rotY, m_rotZ) * Matrix::CreateTranslation(m_position);
			m_mesh->Render(renderer, m_shader, m_world, cam, m_texture);
	}
}

string GameObject::displayStatus()
{
	stringstream outString;
	outString << "--------------------------------------------------\n";
	outString <<"\t\t"<< m_attr << " " << m_name << " \n";
	outString << "Attack: " << m_atk << "  Defence: " << m_def << "  Speed: " << m_speed << "  HP: " << m_HP<<"\n";
	outString << "--------------------------------------------------\n";
	return outString.str();
}

string GameObject::displayStatus(bool withInstruct)
{
	stringstream outString;
	outString << "--------------------------------------------------\n";
	outString << "\t\t" << m_attr << " " << m_name << " \n";
	outString << "Attack: " << m_atk << "  Defence: " << m_def << "  Speed: " << m_speed << "  HP: " << m_HP << "\n";
	outString << "--------------------------------------------------\n";
	
	outString << "\nHow to Play: press the arrow key to move, move to the coloured block to trigger actions\n";
	outString << "coloured floor type:\n";
	outString << "red: enemy\ndarker red: stronger enemy\nblue: warp zone\ngreen: item\n";
	if (m_HP <= 10) {
		outString << "\nyou have not enough HP, please find item to recover it!\n";
	}
	
	return outString.str();

}

void GameObject::warpLocation()
{
	//take it to the randomLocation
	//srand(time(NULL));
	 Vector3 warpLocation= m_warpList[rand() % m_warpList.size()];
	 warpLocation.x -= 10.0f;
	 warpLocation.z -= 10.0f;
	 m_position=warpLocation;
	//and then set the last wrap location to prevent random wrap again.
	m_lastWarp = m_position;
}

void GameObject::gainItem(GameObject * inObject)
{
	m_atk += inObject->getAtk();
	if (m_atk > M_MAX_PARA) {
		m_atk = M_MAX_PARA; 
	}
	m_def += inObject->getDef();
	if (m_def>M_MAX_PARA){
		m_def = M_MAX_PARA;
	}
	m_HP += inObject->getHP();
	if (m_HP>M_MAX_PARA) {
		m_HP = M_MAX_PARA;
	}
	m_speed += inObject->getSpeed();
	if (m_speed>M_MAX_PARA) {
		m_speed = M_MAX_PARA;
	}
}

bool GameObject::isDefeated()
{
	if (m_HP <= 0) {
		return true;
	}
	else if (m_HP > M_MAX_PARA) {
		m_HP = M_MAX_PARA;
	}
	return false;
}

void GameObject::changeHP(int inHP)
{
	m_HP += inHP;
	if (m_HP < 0) {
		m_HP = 0;
	}

}

void GameObject::changeHPBar(int inHP)
{
	//the length of the HPbar= currentHP / the maximum HP value(99) *2
	m_scaleX = static_cast<float>(inHP) / static_cast<float>(M_MAX_PARA) * 2;
	if (m_scaleX > 2.0f) { m_scaleX = 2.0f; }
}

int GameObject::m_enemyNumbers = 0;

Vector3 GameObject::m_lastWarp = Vector3(0.0f, 0.0f, 0.0f);

vector<Vector3>GameObject::m_warpList = { Vector3::Zero };

float GameObject::mapSize = 0;

