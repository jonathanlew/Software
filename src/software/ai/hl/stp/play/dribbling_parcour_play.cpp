#include "software/ai/hl/stp/play/dribbling_parcour_play.h"

#include "shared/constants.h"
#include "software/ai/hl/stp/tactic/dribble/dribble_tactic.h"
#include "software/ai/hl/stp/tactic/move/move_tactic.h"
#include "software/util/design_patterns/generic_factory.h"

DribblingParcourPlay::DribblingParcourPlay(std::shared_ptr<const PlayConfig> config)
    : Play(config, false)
{
}

bool DribblingParcourPlay::isApplicable(const World &world) const
{
    // This play is never applicable so it will never be chosen during gameplay
    // This play can be run for hardware challenges by using the Play override
    return false;
}

bool DribblingParcourPlay::invariantHolds(const World &world) const
{
    return false;
}

void DribblingParcourPlay::getNextTactics(TacticCoroutine::push_type &yield,
                                          const World &world)
{
    std::shared_ptr<DribbleTactic> dribble_tactic = std::make_shared<DribbleTactic>();
    dribble_tactic->updateRobot(world.friendlyTeam().getRobotById(0).value());

    std::shared_ptr<MoveTactic> move_tactic = std::make_shared<MoveTactic>(true);

    size_t current_gate = 0;
    std::array<Point, 4> gate_points = {GATE_ONE_MIDPOINT, GATE_TWO_MIDPOINT, GATE_THREE_MIDPOINT, GATE_FOUR_MIDPOINT};

    int times_crossed = 0;

    dribble_tactic->updateControlParams(gate_points[current_gate], Angle::quarter(), true);
    do
    {
        TacticVector result = {};
        if (world.gameState().isPlaying())
        {
            // TODO (#2108): implement parcour
            if (dribble_tactic->done()) {
                if (current_gate <= gate_points.size()) {

                    Angle finishing_orientation = Angle::quarter();
                    if (current_gate % 2 != 0) {
                        finishing_orientation = Angle::threeQuarter();
                    }

                    if (current_gate == 4)
                    {
                        if (times_crossed == 3) {
                            continue; //done
                        }

                        if (times_crossed % 2 == 0)
                        {
                            dribble_tactic->updateControlParams(GATE_FOUR_NEG, finishing_orientation, 0.0);
                        }
                        else
                        {
                            dribble_tactic->updateControlParams(GATE_FOUR_POS, finishing_orientation, 0.0);
                        }
                        times_crossed++;
                    }
                    else {
                        dribble_tactic->updateControlParams(gate_points[current_gate], finishing_orientation, 0.0);
                        current_gate++;
                    }
                }
            }

            result.emplace_back(dribble_tactic);
        }
        else
        {
            move_tactic->updateControlParams(Point(0, 0), Angle::zero(), 0.0);
            result.emplace_back(move_tactic);
        }
        yield({result});
    } while (true);
}

// Register this play in the genericFactory
static TGenericFactory<std::string, Play, DribblingParcourPlay, PlayConfig> factory;
