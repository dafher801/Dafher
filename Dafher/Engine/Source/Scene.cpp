#include "Scene.h"
#include "Sprite.h"
#include "MovementComponent.h"

bool Scene::Init()
{
	Node::Init();
	SetName("Scene");

	Node* object = Node::Create();
	object->_transform->SetLocalPosition(Vector3(1000.0f, 500.0f, 0.0f));
	object->SetName("123");
	this->AddChild(object);

	Node* object2 = Node::Create();
	object2->_transform->SetLocalPosition(Vector3(-200.0f, 100.0f, 0.0f));
	object2->SetName("456");
	object->AddChild(object2);

	Sprite* sprite = object->AddComponent<Sprite>();
	sprite->SetSize(400, 400);
	for (int i = 0; i < 8; ++i)
	{
		std::string textureKey = "sprite_frame_0" + std::to_string(i + 1);
		sprite->AddFrame(textureKey, 0.1f);
	}
	sprite->Play(true);

	Sprite* sprite2 = object2->AddComponent<Sprite>();
	sprite2->SetSize(400, 400);
	for (int i = 0; i < 8; ++i)
	{
		std::string textureKey = "sprite_frame_0" + std::to_string(i + 1);
		sprite2->AddFrame(textureKey, 0.1f);
	}
	sprite2->Play(true);

	MovementComponent* movement = object->AddComponent<MovementComponent>();
	movement->SetSpeed(100.0f);
	movement->SetVelocity(Vector3(-1.0f, 0.0f, 0.0f));

	return true;
}
