//---------------------------
// Include Files
//---------------------------
#include "Fygar.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Fygar::Fygar(dae::GameObject* owner)
	:	Component(owner)
{
	// nothing to create
}

void Fygar::HandleCollision(const CollisionEvent& )
{
}