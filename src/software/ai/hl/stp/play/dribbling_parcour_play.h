#pragma once

#include "shared/parameter/cpp_dynamic_parameters.h"
#include "software/ai/hl/stp/play/play.h"

std::array<Point, 2> GATE_ONE = {Point(-1.5, -1.45), Point(-1.75, -1.2)};
std::array<Point, 2> GATE_TWO = {Point(-1.5, -0.6),  Point(-1.25, -0.4)};
std::array<Point, 2> GATE_THREE = {Point(-1.5, 0.1), Point(-1.75, 0.3)};
std::array<Point, 2> GATE_FOUR = {Point(-1.5, 0.65), Point(-1.25, 0.7)};
std::array<Point, 2> GATE_FIVE = {Point(-1.5, 1.2),  Point(-1.75, 1.2)};


const Point GATE_FOUR_POS = Point(-1.7, 1.2);
const Point GATE_FOUR_NEG = Point(-1.3, 1.2);

/**
 * Play for the dribbling parcour hardware challenge
 * https://robocup-ssl.github.io/ssl-hardware-challenge-rules/rules.html
 */
class DribblingParcourPlay : public Play
{
   public:
    DribblingParcourPlay(std::shared_ptr<const PlayConfig> config);

    bool isApplicable(const World &world) const override;

    bool invariantHolds(const World &world) const override;

    void getNextTactics(TacticCoroutine::push_type &yield, const World &world) override;
};
