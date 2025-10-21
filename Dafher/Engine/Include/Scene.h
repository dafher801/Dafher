#ifndef __SCNENE_H__
#define __SCNENE_H__

#include "Node.h"

class Scene : public Node
{
public:
	Scene() noexcept = default;
	~Scene() noexcept = default;

public:

	virtual bool Init() override;
	virtual void Update(float delta) override;
};

#endif