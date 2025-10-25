#include "Scene.h"
#include "Sprite.h"
#include "Movement.h"

bool Scene::Init()
{
	Node::Init();
	SetName("Scene");

	return true;
}