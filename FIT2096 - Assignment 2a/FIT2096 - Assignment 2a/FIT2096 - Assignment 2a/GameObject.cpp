#include "GameObject.h"
#include "MathsHelper.h"




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

		//roll up and down=Y
		//turning=X
		//tilting=Z

		// Accumulate change in mouse position 
		m_rotX += m_input->GetMouseDeltaX() * m_rotateSpeed * timestep;
		m_rotY += m_input->GetMouseDeltaY() * m_rotateSpeed * timestep;

		// Limit how far the player can look down and up
		m_rotY = MathsHelper::Clamp(m_rotY, ToRadians(-80.0f), ToRadians(80.0f));

		// Wrap heading and pitch up in a matrix so we can transform our look at vector
		// Heading is controlled by MouseX (horizontal movement) but it is a rotation around Y
		// Pitch  controlled by MouseY (vertical movement) but it is a rotation around X
		Matrix heading = Matrix::CreateRotationY(m_rotZ);
		Matrix pitch = Matrix::CreateRotationY(m_rotX);

		// Transform a world right vector from world space into local space
		Vector3 localRight = Vector3::TransformNormal(Vector3(1.15f, 0, 0), pitch);

		// Essentially our local forward vector but always parallel with the ground
		// Remember a cross product gives us a vector perpendicular to the two input vectors
		Vector3 localForwardXZ = localRight.Cross(Vector3(0, 1.15f, 0));

	//local translation
		Vector3 newPosition = m_position;

		//obseleted way m_position += Vector3(0.0f, 0.0f, -1.0f) * m_moveSpeed * timestep
		if (m_input->GetKeyUp('A'))
		{
			// Note you can instead use Vector3::Left etc. as a shorthand when declaring unit vectors
			//m_position += Vector3(-1.0f, 0.0f, 0.0);
			newPosition-=localRight;
		}
		if (m_input->GetKeyUp('D'))
		{
			//m_position += Vector3(1.0f, 0.0f, 0.0f);
			newPosition += localRight;
		}
		if (m_input->GetKeyUp('W'))
		{
			//m_position += Vector3(0.0f, 0.0f, 1.0f);
			newPosition += localForwardXZ;
		}
		if (m_input->GetKeyUp('S'))
		{
			//m_position += Vector3(0.0f, 0.0f, -1.0f);
			newPosition -= localForwardXZ;

		}
		//and then round off the X and Z value to lock itself in the grid
		m_position = Vector3(round(newPosition.x), newPosition.y, round(newPosition.z));

		m_playerPosition = m_position;
	}

	//and enemy will be keep rotate is direction towards player
	else if (m_attr == "enemy") {

		//using dot product to find the angle of m_posion and m_position-m_playerPosition

		/*
		Vector3 EO = m_position;
		//EO.Normalize();
		Vector3 EP = m_position - m_playerPosition;
		//EP.Normalize();
		
		float dot = EO.Dot(EP);
		float dotMod = fmod(EO.Dot(EP), 2.0f) - 1.0f;

		//preventing domain error, limit the dot value from -1 to 1(the domain of sine wave)
		// Force the dot product of the two input vectors to
		// fall within the domain for inverse cosine, which
		// is -1 <= x <= 1. This will prevent runtime
		// "domain error" math exceptions.
		dot = (dot < -1.0 ? -1.0 : (dot > 1.0 ? 1.0 : dot));

		float theta=acos(dot);
		//Vector3 newPosition = m_position.Cross(m_playerPosition);
		//1cycle:2pi

		m_rotX = 3.1415;
		//m_position = newPosition;
		int a = 0;
		*/

		//meh, why using that complex trigomoertry that can only find its local angle?
		//why not imagine a right angle triangle, and use some supersimple trigomoertry to find a global angle that touch the axis
		//then adjust it by its global relationship? stupid way might be a better approach?

		//E:enemy point
		//P:player point
		//C: local right angle point
		//1cycle:2pi(in radian)

		//find the local angle/direction for EP
			//get E and P first
		Vector3 E = m_position;
		Vector3 P = m_playerPosition;
			//get the distance of PE(not vector)
		float PElength = Vector3::Distance(P, E);
			//get the height and width of EC(imagine there's a point call C that makes a right angle, also not a vector)
		float EClength = abs(P.x - E.x);
			//using inverse cosine to check the angle
		float CEPcos = acos(EClength / PElength);
		//after that, use the slope to check the direction
		float PEslope = (P.z - E.z) / (P.x - E.x);
			//if the slope is negative, it means it's points either towards SE or NW
		if (PEslope < 0) {
			//if E.x <=P.x, it means it's at SE
			if (E.x <= P.x) {
				m_rotX = (M_PI / 2) + CEPcos;
			}
			//otherwise it's at NW
			else {
				m_rotX = (1.5*M_PI) + CEPcos;
			}
		}
			//if the slope is positive, it means it's either SW or NE
		else {
			//if E.x <=P.x, it means it's at NE
			if (E.x <= P.x) {
				m_rotX = (M_PI/2) - CEPcos;
			}
			//otherwise it's at SW
			else {
				m_rotX = (1.5*M_PI) - CEPcos;
			}

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
	outString << "coloured object type:\n";
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

Vector3 GameObject::m_playerPosition = Vector3(0.0f, 0.0f, 0.0f);

