#include "software/ai/intent/kick_intent.h"

#include <gtest/gtest.h>
#include <string.h>

// For equality operators, we only check for cases not covered in the Primitive tests,
// since Intents inherit from Primitives
TEST(KickIntentTest, test_equality_operator_intents_equal)
{
    KickIntent kick_intent = KickIntent(
        0, Point(), Angle::zero(), AutoChipOrKick{AutoChipOrKickMode::AUTOCHIP, 2.0});
    KickIntent kick_intent_other = KickIntent(
        0, Point(), Angle::zero(), AutoChipOrKick{AutoChipOrKickMode::AUTOCHIP, 2.0});

    EXPECT_EQ(kick_intent, kick_intent_other);
}

TEST(KickIntentTest, test_equality_operator_intents_not_equal)
{
    KickIntent kick_intent = KickIntent(
        0, Point(), Angle::zero(), AutoChipOrKick{AutoChipOrKickMode::AUTOCHIP, 2.0});
    KickIntent kick_intent_other_kick = KickIntent(
        0, Point(), Angle::zero(), AutoChipOrKick{AutoChipOrKickMode::AUTOKICK, 2.0});
    KickIntent kick_intent_other_speed = KickIntent(
        0, Point(), Angle::zero(), AutoChipOrKick{AutoChipOrKickMode::AUTOCHIP, 3.0});

    EXPECT_NE(kick_intent, kick_intent_other_kick);
    EXPECT_NE(kick_intent, kick_intent_other_speed);
}
