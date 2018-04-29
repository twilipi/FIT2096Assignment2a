#ifndef FPCAMERA_H
#define FPCAMERA_H

#include "Camera.h"
#include "GameObject.h"
#include "InputController.h"


class FPCamera :public Camera
{
private:
	InputController * m_input;

	GameObject * m_objectToFollow;
	Vector3 m_offset;

	Vector3 m_cameraAdjust;

	bool m_catchupMode;
	float m_catchupSpeed;

	float m_moveSpeed;
	float m_rotationSpeed;
	float m_heightChangeSpeed;

	float m_heading;
	float m_pitch;

	Vector3 TransformOffset();

public:
	FPCamera();
	FPCamera(GameObject* target, Vector3 offset, bool catchup, float catchupSpeed , InputController* input);

	void Update(float timestep);

	~FPCamera();
};


#endif // !FPCAMERA_H




