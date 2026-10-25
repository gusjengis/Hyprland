#include <desktop/rule/layerRule/LayerRule.hpp>

#include <gtest/gtest.h>

using namespace Desktop::Rule;

TEST(LayerRule, parsesWorkspaceBlurExemption) {
    CLayerRule rule;

    const auto disabled = rule.addEffect(LAYER_RULE_EFFECT_NO_WORKSPACE_BLUR, "false");
    ASSERT_TRUE(disabled.has_value()) << disabled.error();
    ASSERT_EQ(rule.effects().size(), 1);
    EXPECT_FALSE(std::get<bool>(rule.effects().front().value));

    const auto enabled = rule.addEffect(LAYER_RULE_EFFECT_NO_WORKSPACE_BLUR, "true");
    ASSERT_TRUE(enabled.has_value()) << enabled.error();
    ASSERT_EQ(rule.effects().size(), 2);
    EXPECT_TRUE(std::get<bool>(rule.effects().back().value));
}
