#include "Scene.h"
#include "Sprite.h"

bool Scene::Init()
{
	Node::Init();

	Sprite* sprite = new Sprite();
	
	for (int i = 0; i < 8; ++i)
	{
		std::string textureKey = "sprite_frame_0" + std::to_string(i + 1);
		sprite->AddFrame(textureKey, 0.1f);
	}

	sprite->_transform.SetPosition(Vector3(1000.0f, 500.0f, 0.0f));
	sprite->_transform.SetScale(Vector3(400.0f, 400.0f, 1.0f));
	sprite->Play(true);
	this->AddChild(sprite);
	return true;
}

void Scene::Update(float delta)
{
	Node::Update(delta);
}
