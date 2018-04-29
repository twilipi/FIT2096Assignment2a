#include "FPCamera.h"
#include "MathsHelper.h"


FPCamera::FPCamera()
{
	m_catchupMode = false;
	m_catchupSpeed = 1.0f;
	m_objectToFollow = NULL;
	m_offset = Vector3(0.0f, 10.0f, -10.0f);


	SetPosition(m_objectToFollow->GetPosition() + TransformOffset());

}

FPCamera::FPCamera(GameObject * target, Vector3 offset, bool catchup, float catchupSpeed, InputController* input)
{
	m_input = input;
	m_catchupMode = catchup;
	m_catchupSpeed = catchupSpeed;
	m_objectToFollow = target;
	m_offset = offset;
	m_cameraAdjust = Vector3(0.0f, 0.75f, 0.0f);//adjust the angle of the camera...a dirty way

	// Set starting position so catchup mode doesn't show camera animating from the origin
	SetPosition(m_objectToFollow->GetPosition() + TransformOffset());


}

void FPCamera::Update(float timestep)
{
	if (m_objectToFollow != NULL)
	{

		//BUG: WHILE CHANGING POSITION, THE CAMERA ITSELF WILL SNAP TO FRONT FIRST AND RETURN IT BACK TO ORIGINAL POSITION.

		//// Accumulate change in mouse position 
		//m_heading += m_input->GetMouseDeltaX() * m_rotationSpeed * timestep;
		//m_pitch += m_input->GetMouseDeltaY() * m_rotationSpeed * timestep;

		//// Limit how far the player can look down and up
		//m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f));

		//	// Wrap heading and pitch up in a matrix so we can transform our look at vector
		//	// Heading is controlled by MouseX (horizontal movement) but it is a rotation around Y
		//	// Pitch  controlled by MouseY (vertical movement) but it is a rotation around X
		//Matrix heading = Matrix::CreateRotationY(m_heading);
		//Matrix pitch = Matrix::CreateRotationX(m_pitch);

		//roll up and down=X
		//turning=Y
		//tilting=Z
		// Combine pitch and heading into one matrix for convenience (from player and the x=z and Y=X are inverted and I dun know why)
		Matrix lookAtRotation = Matrix::CreateRotationX(m_objectToFollow->GetYRotation())*Matrix::CreateRotationY(m_objectToFollow->GetXRotation());

		// Transform a world forward vector into local space (take pitch and heading into account)
		Vector3 lookAt = Vector3::TransformNormal(Vector3(0, 0, 1), lookAtRotation);

		// At this point, our look-at vector is still relative to the origin
		// Add our position to it so it originates from the camera and points slightly in front of it
		// Remember the look-at vector needs to describe a point in the world relative to the origin
		lookAt += GetPosition();

		// We'll look at the object we're following
		// Sometimes we'll even add an offset here too 
		//SetLookAt(m_objectToFollow->GetPosition()+m_cameraAdjust);
		SetLookAt(lookAt);

		// Not storing this directly in m_offset as it would accumulate over time and spin out of control
		Vector3 rotatedOffset = TransformOffset();
		//Vector3 rotatedOffset = Vector3::TransformNormal(m_offset, lookAtRotation);

		if (m_catchupMode)
		{
			// Catchup to target position. A simple LERP will do.
			SetPosition(Vector3::Lerp(GetPosition(), m_objectToFollow->GetPosition() + rotatedOffset, timestep * m_catchupSpeed)+m_cameraAdjust );
			//			SetPosition(Vector3::Lerp(lookAt += GetPosition(), m_objectToFollow->GetPosition() + rotatedOffset, timestep * m_catchupSpeed)+m_cameraAdjust );
			//SetPosition(lookAt += GetPosition());
		}
		else
		{
			// Jump directly to target position (including offset)
			//SetPosition(m_objectToFollow->GetPosition() + rotatedOffset);
			SetPosition(m_objectToFollow->GetPosition() + rotatedOffset);
		}
	}


	// Give our parent a chance to regenerate the view matrix
	Camera::Update(timestep);

}

FPCamera::~FPCamera()
{

}

Vector3 FPCamera::TransformOffset()
{
	// Wrap up the Y rotation of the object we're following in a matrix
	Matrix rotation = Matrix::CreateRotationY(m_objectToFollow->GetYRotation());

	// Transform our offset vector by the rotation so the offset rotates with our follow object
	return Vector3::TransformNormal(m_offset, rotation);
}
