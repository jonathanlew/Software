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

    std::shared_ptr<MoveTactic> move_tactic = std::make_shared<MoveTactic>(true);

    size_t current_gate = 0;
    size_t current_step = 0;
    std::array<Point, 5> gate_points = {            Point(-1.4, -1.45), Point(-1.6, -0.6), Point(-1.4, 0.1), Point(-1.6, .65), Point(-1.3, 1.2)};

    std::array<std::array<Point, 2>, 5> gates = {GATE_ONE, GATE_TWO, GATE_THREE, GATE_FOUR, GATE_FIVE};

    int times_crossed = 0;

    dribble_tactic->updateControlParams(gate_points[current_gate], Angle::quarter(), true);
    do
    {
        TacticVector result = {};
        if (world.gameState().isPlaying())
        {
            if (dribble_tactic->done()) {
                if (current_gate <= gate_points.size()) {

                    std::optional<Angle> finishing_orientation = std::nullopt;

                    if (current_gate == 5)
                    {
                        if (times_crossed == 3) {
                            continue; //done
                        }

                        finishing_orientation =  std::nullopt;

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
                        if (current_step == 1)
                        {
                            finishing_orientation = std::nullopt;
                        }

                        dribble_tactic->updateControlParams(gates[current_gate][current_step], finishing_orientation, 0.0);

                        if (current_step == 1) {
                            current_gate++;
                            current_step = 0;
                        }

                        current_step++;
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
