#include "Scene.h"
#include "Sprite.h"
#include "MovementComponent.h"

bool Scene::Init()
{
	Node::Init();

	Node* object = Node::Create();
	object->_transform.SetPosition(Vector3(1000.0f, 500.0f, 0.0f));
	object->_transform.SetScale(Vector3(400.0f, 400.0f, 1.0f));
	this->AddChild(object);

	Sprite* sprite = object->AddComponent<Sprite>();
	
	for (int i = 0; i < 8; ++i)
	{
		std::string textureKey = "sprite_frame_0" + std::to_string(i + 1);
		sprite->AddFrame(textureKey, 0.1f);
	}

	sprite->Play(true);

	MovementComponent* movement = object->AddComponent<MovementComponent>();
	movement->SetSpeed(100.0f);
	movement->SetVelocity(Vector3(-1.0f, 0.0f, 0.0f));

	return true;
}
