#include "CollisionHandler.h"
#include "ServiceLocator.h"
#include "GameObject.h"
#include "Player.h"
#include "Pooka.h"
#include "Fygar.h"
#include "Rock.h"
#include "RopeComponent.h"
#include "RopeHeadComponent.h"

void CollisionHandler::Notify(dae::GameObject* gameObject, dae::EventID event)
{
	if (!gameObject)
	{
		std::cerr << "[CollisionHandler] Null GameObject passed to Notify!\n";
		return;
	}

	if (event != dae::EventRegistry::GetInstance().GetEventID("CollisionEvent")) return;

	const auto& collision = ServiceLocator::GetCollisionSystem().GetLastCollisionEvent();

	if (!collision.collider || !collision.collided)
	{
		std::cerr << "[CollisionHandler] CollisionEvent contains null objects!\n";
		return;
	}

	if (gameObject->HasComponent<RopeHeadComponent>())
		gameObject->GetComponent<RopeHeadComponent>()->GetRopeOwner()->HandleCollision(collision);
	
	else if (gameObject->HasComponent<Player>())
		gameObject->GetComponent<Player>()->HandleCollision(collision);

	else if (gameObject->HasComponent<Pooka>())
			 gameObject->GetComponent<Pooka>()->HandleCollision(collision);

	else if (gameObject->HasComponent<Fygar>())
			 gameObject->GetComponent<Fygar>()->HandleCollision(collision);

	else if	(gameObject->HasComponent<Rock>())
			 gameObject->GetComponent<Rock>()->HandleCollision(collision);
}
