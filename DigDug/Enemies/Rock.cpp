//---------------------------
// Include Files
//---------------------------
#include "Rock.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Rock::Rock(dae::GameObject* owner)
	:	Component(owner)
{
	// nothing to create
}

void Rock::HandleCollision(const CollisionEvent& )
{
	std::cout << "Rock handled collision!\n";
}
