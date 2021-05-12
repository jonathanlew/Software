#pragma once

#include "software/ai/intent/direct_primitive_intent.h"
#include "software/geom/angle.h"
#include "software/geom/point.h"

class KickIntent : public DirectPrimitiveIntent
{
   public:
    /**
     * Creates a new Kick Intent
     *
     * @param robot_id The id of the robot that this Intent is for
     * @param dest The destination of the Movement
     * @param final_angle The final angle the robot should have at the end of the movement
     * @param auto_chip_or_kick The command to autochip or autokick
     */
    explicit KickIntent(unsigned int robot_id, const Point& dest,
                        const Angle& final_angle, AutoChipOrKick auto_chip_or_kick);

    KickIntent() = delete;
};
