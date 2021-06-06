#include "software/ai/hl/stp/play/pass_endurance_play.h"

#include "shared/constants.h"
#include "software/ai/hl/stp/tactic/attacker/attacker_tactic.h"
#include "software/ai/hl/stp/tactic/move/move_tactic.h"
#include "software/ai/hl/stp/tactic/receiver/receiver_tactic.h"
#include "software/util/design_patterns/generic_factory.h"

PassEndurancePlay::PassEndurancePlay(std::shared_ptr<const PlayConfig> config)
    : Play(config, false)
{
}

bool PassEndurancePlay::isApplicable(const World &world) const
{
    // This play is never applicable so it will never be chosen during gameplay
    // This play can be run for hardware challenges by using the Play override
    return false;
}

bool PassEndurancePlay::invariantHolds(const World &world) const
{
    return false;
}

void PassEndurancePlay::getNextTactics(TacticCoroutine::push_type &yield,
                                       const World &world)
{
    std::array<std::shared_ptr<MoveTactic>, NUM_ROBOTS> move_tactics;
    std::generate(move_tactics.begin(), move_tactics.end(),
                  []() { return std::make_shared<MoveTactic>(true); });

    /**
     *            0
     *           / \
     *          /   \
     *         /     \
     *        /       \
     *       /         \
     *      1-----------2
     *
     * Attempt to pass from point 0 to 1 to 2 and back to 0 again
     */

    std::array<Point, NUM_ROBOTS> pass_points = {Point(-2.25, 2), Point(-4.25, -2),
                                                 Point(-0.25, -2)};
    std::unordered_map<Point, std::pair<Pass, Point>> start_point_to_pass_and_next_point;
    for (unsigned int i = 0; i < NUM_ROBOTS; i++)
    {
        start_point_to_pass_and_next_point.insert(
            {pass_points[i],
             std::make_pair(Pass(pass_points[i], pass_points[(i + 1) % NUM_ROBOTS],
                                 BALL_MAX_SPEED_METERS_PER_SECOND - 1),
                            pass_points[(i + 2) % NUM_ROBOTS])});
    }

    do
    {
        if (world.gameState().isPlaying())
        {
            // Select pass closest to ball position
            auto pass_and_next_point =
                start_point_to_pass_and_next_point.at(*std::min_element(
                    pass_points.begin(), pass_points.end(),
                    [world](const Point &a, const Point &b) -> bool {
                        return (a - world.ball().position()).lengthSquared() <
                               (b - world.ball().position()).lengthSquared();
                    }));
            auto pass = pass_and_next_point.first;
            auto attacker =
                std::make_shared<AttackerTactic>(play_config->getAttackerTacticConfig());
            auto receiver = std::make_shared<ReceiverTactic>(pass);

            do
            {
                attacker->updateControlParams(pass);
                receiver->updateControlParams(pass);
                std::get<0>(move_tactics)
                    ->updateControlParams(
                        pass_and_next_point.second,
                        (pass.receiverPoint() - pass_and_next_point.second).orientation(),
                        0);
                std::get<1>(move_tactics)
                    ->updateControlParams(
                        pass.passerPoint(),
                        (pass.receiverPoint() - pass.passerPoint()).orientation(), 0);


                if (!attacker->done())
                {
                    yield(PriorityTacticVector{
                        {attacker, receiver, std::get<0>(move_tactics)}});
                }
                else
                {
                    yield(PriorityTacticVector{{receiver, std::get<0>(move_tactics),
                                                std::get<1>(move_tactics)}});
                }

            } while (!receiver->done());
        }
        else
        {
            // line up along center line
            int initial_offset = static_cast<int>(-move_tactics.size() / 2 + 1);
            for (size_t k = 0; k < move_tactics.size(); k++)
            {
                auto next_position = Point(
                    world.field().centerPoint().x(),
                    (initial_offset + static_cast<int>(k)) * 4 * ROBOT_MAX_RADIUS_METERS);
                move_tactics[k]->updateControlParams(next_position, Angle::zero(), 0);
            }
            TacticVector result = {};
            result.insert(result.end(), move_tactics.begin(), move_tactics.end());
            yield({{result}});
        }
    } while (true);
}

// Register this play in the genericFactory
static TGenericFactory<std::string, Play, PassEndurancePlay, PlayConfig> factory;
