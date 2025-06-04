#include "CollisionHandler.h"
#include "ServiceLocator.h"
#include "GameObject.h"
#include "Player.h"
#include "Pooka.h"
#include "Fygar.h"
#include "Rock.h"
#include "RopeComponent.h"

void CollisionHandler::Notify(dae::GameObject* gameObject, dae::EventID event)
{
	if (event != dae::EventRegistry::GetInstance().GetEventID("CollisionEvent")) return;

	auto& collision = ServiceLocator::GetCollisionSystem().GetLastCollisionEvent();

	if (gameObject->HasComponent<Player>())
		gameObject->GetComponent<Player>()->HandleCollision(collision);

	else if (gameObject->HasComponent<Pooka>())
			 gameObject->GetComponent<Pooka>()->HandleCollision(collision);

	else if (gameObject->HasComponent<Fygar>())
			 gameObject->GetComponent<Fygar>()->HandleCollision(collision);

	else if	(gameObject->HasComponent<Rock>())
			 gameObject->GetComponent<Rock>()->HandleCollision(collision);
}
