#ifndef __SCNENE_H__
#define __SCNENE_H__

#include "Node.h"

class Scene : public Node
{
protected:
	Scene() noexcept = default;

	Scene(const Scene& scene) noexcept = delete;
	Scene(Scene&& scene) noexcept = delete;
	Scene& operator=(const Scene& scene) noexcept = delete;
	Scene& operator=(Scene&& scene) noexcept = delete;

public:
	virtual ~Scene() noexcept override = default;

public:
	CREATE(Scene)

public:
	virtual bool Init() override;
};

#endif