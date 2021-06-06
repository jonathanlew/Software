#include "software/ai/hl/stp/play/pass_endurance_play.h"

#include <gtest/gtest.h>

#include "software/simulated_tests/simulated_play_test_fixture.h"
#include "software/simulated_tests/validation/validation_function.h"
#include "software/test_util/test_util.h"
#include "software/time/duration.h"
#include "software/world/world.h"

class PassEndurancePlayTest : public SimulatedPlayTestFixture
{
   protected:
    Field field = Field::createSSLDivisionBField();
};

TEST_F(PassEndurancePlayTest, test_pass_endurance_play_force_start)
{
    BallState ball_state(Point(-0.8, 0), Vector(0, 0));
    auto friendly_robots = TestUtil::createStationaryRobotStatesWithId(
        {Point(-4, 0), Point(0.5, 0), Point(-3, 1)});
    auto enemy_team = TestUtil::createStationaryRobotStatesWithId(
        {Point(4, 0), Point(4, 0.2), Point(4, -0.2), Point(4, -0.4), Point(4, -0.4)});
    setFriendlyGoalie(0);
    setAIPlay(TYPENAME(PassEndurancePlay));

    setRefereeCommand(RefereeCommand::FORCE_START, RefereeCommand::STOP);

    std::vector<ValidationFunction> terminating_validation_functions = {
        // This will keep the test running for 9.5 seconds to give everything enough
        // time to settle into position and be observed with the Visualizer
        // TODO (#2109): Implement proper validation
        [](std::shared_ptr<World> world_ptr, ValidationCoroutine::push_type& yield) {
            while (world_ptr->getMostRecentTimestamp() < Timestamp::fromSeconds(29.5))
            {
                yield("Timestamp not at 29.5s");
            }
        }};

    std::vector<ValidationFunction> non_terminating_validation_functions = {};

    runTest(field, ball_state, friendly_robots, enemy_team, terminating_validation_functions,
            non_terminating_validation_functions, Duration::fromSeconds(30));
}

TEST_F(PassEndurancePlayTest, test_pass_endurance_play_stopped)
{
    BallState ball_state(Point(-0.8, 0), Vector(0, 0));
    auto friendly_robots = TestUtil::createStationaryRobotStatesWithId(
        {Point(4, 0), Point(0.5, 0), Point(-3, 1)});
    setFriendlyGoalie(0);
    setAIPlay(TYPENAME(PassEndurancePlay));

    setRefereeCommand(RefereeCommand::STOP, RefereeCommand::HALT);

    std::vector<ValidationFunction> terminating_validation_functions = {
        // This will keep the test running for 9.5 seconds to give everything enough
        // time to settle into position and be observed with the Visualizer
        // TODO (#2109): Implement proper validation
        [](std::shared_ptr<World> world_ptr, ValidationCoroutine::push_type& yield) {
            while (world_ptr->getMostRecentTimestamp() < Timestamp::fromSeconds(9.5))
            {
                yield("Timestamp not at 9.5s");
            }
        }};

    std::vector<ValidationFunction> non_terminating_validation_functions = {};

    runTest(field, ball_state, friendly_robots, {}, terminating_validation_functions,
            non_terminating_validation_functions, Duration::fromSeconds(10));
}
