#include "software/ai/hl/stp/play/scoring_from_contested_possession_play.h"

#include "shared/constants.h"
#include "software/ai/hl/stp/tactic/attacker/attacker_tactic.h"
#include "software/ai/hl/stp/tactic/move/move_tactic.h"
#include "software/util/design_patterns/generic_factory.h"

ScoringFromContestedPossessionPlay::ScoringFromContestedPossessionPlay(
    std::shared_ptr<const PlayConfig> config)
    : Play(config, false)
{
}

bool ScoringFromContestedPossessionPlay::isApplicable(const World &world) const
{
    // This play is never applicable so it will never be chosen during gameplay
    // This play can be run for hardware challenges by using the Play override
    return false;
}

bool ScoringFromContestedPossessionPlay::invariantHolds(const World &world) const
{
    return false;
}

void ScoringFromContestedPossessionPlay::getNextTactics(TacticCoroutine::push_type &yield,
                                                        const World &world)
{
    auto attacker_tactic_config = std::make_shared<AttackerTacticConfig>();
    std::shared_ptr<AttackerTactic> attacker_tactic =
        std::make_shared<AttackerTactic>(attacker_tactic_config);
    std::shared_ptr<MoveTactic> move_tactic = std::make_shared<MoveTactic>(true);

    do
    {
        TacticVector result = {};
        if (world.gameState().isPlaying())
        {
            result.emplace_back(attacker_tactic);
        }
        else
        {
            auto ball_position = world.ball().position();
            // default to -x of destination
            Point destination =
                ball_position -
                Vector(-2 * (ROBOT_MAX_RADIUS_METERS + BALL_MAX_RADIUS_METERS), 0);
            Angle final_orientation = Angle::zero();
            for (const auto &enemy_robot : world.enemyTeam().getAllRobots())
            {
                if (enemy_robot.isNearDribbler(ball_position, 0.1))
                {
                    destination = enemy_robot.position() +
                                  Vector::createFromAngle(enemy_robot.orientation())
                                      .normalize(2 * ROBOT_MAX_RADIUS_METERS +
                                                 2 * BALL_MAX_RADIUS_METERS);
                    final_orientation =
                        (enemy_robot.orientation() + Angle::half()).clamp();
                    break;
                }
            }
            move_tactic->updateControlParams(destination, final_orientation, 0.0);
            result.emplace_back(move_tactic);
        }
        yield({result});
    } while (true);
}

// Register this play in the genericFactory
static TGenericFactory<std::string, Play, ScoringFromContestedPossessionPlay, PlayConfig>
    factory;
