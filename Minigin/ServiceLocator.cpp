#include  "ServiceLocator.h"

std::unique_ptr<SoundSystem> ServiceLocator::_ss_instance = nullptr;