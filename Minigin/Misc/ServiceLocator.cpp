#include  "ServiceLocator.h"

std::unique_ptr<SoundSystem> ServiceLocator::_ss_instance		= nullptr;

std::unique_ptr<CollisionSystem> ServiceLocator::_cs_instance	= nullptr;