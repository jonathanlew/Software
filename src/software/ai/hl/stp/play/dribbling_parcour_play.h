#pragma once

#include "shared/parameter/cpp_dynamic_parameters.h"
#include "software/ai/hl/stp/play/play.h"

std::array<Point, 2> GATE_ONE = {Point(-0.6, 0), Point(-0.2, 0.25)};
std::array<Point, 2> GATE_TWO = {Point(0.1, 0), Point(0.4, -0.25)};
std::array<Point, 2> GATE_THREE = {Point(0.65, 0), Point(0.9, 0.25)};
std::array<Point, 2> GATE_FOUR = {Point(1.2, 0), Point(1.2, -0.2)};

const Point GATE_ONE_MIDPOINT = Point(-0.6, 0.1);
const Point GATE_TWO_MIDPOINT = Point(0.1, -0.1);
const Point GATE_THREE_MIDPOINT = Point(0.65, 0.1);
const Point GATE_FOUR_MIDPOINT = Point(1.2, -0.2);
const Point GATE_FOUR_POS = Point(1.2, 0.2);
const Point GATE_FOUR_NEG = Point(1.2, -0.2);

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
