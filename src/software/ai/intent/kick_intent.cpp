#include "software/ai/intent/kick_intent.h"

KickIntent::KickIntent(unsigned int robot_id, const Point &kick_origin,
                       const Angle &kick_direction, AutoChipOrKick auto_chip_or_kick)
    : DirectPrimitiveIntent(
          robot_id, *createMovePrimitive(kick_origin, 0, kick_direction,
                                         DribblerMode::OFF, auto_chip_or_kick,
                                         MaxAllowedSpeedMode::PHYSICAL_LIMIT, 0.0))
{
}
