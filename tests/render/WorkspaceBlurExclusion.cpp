#include <render/WorkspaceBlurExclusion.hpp>

#include <gtest/gtest.h>

using namespace Hyprutils::Math;
using namespace Render;

TEST(WorkspaceBlurExclusion, DefaultDoesNotChangeBlur) {
    CWorkspaceBlurExclusion exclusion;
    EXPECT_TRUE(exclusion.forMonitor(CBox{0, 0, 1920, 1080}, 1.F).empty());
}

TEST(WorkspaceBlurExclusion, ShapedInputDoesNotProtectTransparentPadding) {
    CWorkspaceBlurExclusion exclusion;
    CRegion                 input{CBox{20, 20, 280, 160}};
    input.subtract(CBox{20, 20, 10, 10});
    exclusion.addLayer(input, CBox{100, 200, 320, 200});
    auto region = exclusion.forMonitor(CBox{0, 0, 1920, 1080}, 1.F);
    EXPECT_EQ(region.getExtents(), CBox(120, 220, 280, 160));
    EXPECT_TRUE(CRegion{region}.intersect(CBox{120, 220, 10, 10}).empty());
    EXPECT_FALSE(CRegion{region}.intersect(CBox{130, 220, 10, 10}).empty());
}

TEST(WorkspaceBlurExclusion, ClickThroughLayerUsesSurfaceBounds) {
    CWorkspaceBlurExclusion exclusion;
    exclusion.addLayer(CRegion{}, CBox{100, 200, 320, 200});
    EXPECT_EQ(exclusion.forMonitor(CBox{0, 0, 1920, 1080}, 1.F).getExtents(), CBox(100, 200, 320, 200));
}

TEST(WorkspaceBlurExclusion, ClipsInputAndConvertsOffsetFractionalScaleMonitor) {
    CWorkspaceBlurExclusion exclusion;
    exclusion.addLayer(CRegion{CBox{-50, -50, 500, 500}}, CBox{1800, 100, 320, 200});
    EXPECT_EQ(exclusion.forMonitor(CBox{1920, 0, 1280, 720}, 1.5F).getExtents(), CBox(0, 150, 300, 300));
    EXPECT_TRUE(exclusion.forMonitor(CBox{-1920, 0, 1920, 1080}, 1.F).empty());
}

TEST(WorkspaceBlurExclusion, SpecialBlurStillCoversOverlappingApplication) {
    CWorkspaceBlurExclusion exclusion;
    exclusion.addLayer(CRegion{}, CBox{100, 100, 300, 200});
    exclusion.occlude(CBox{200, 50, 300, 300});
    EXPECT_EQ(exclusion.forMonitor(CBox{0, 0, 1920, 1080}, 1.F).getExtents(), CBox(100, 100, 100, 200));
    exclusion.occlude(CBox{0, 0, 1920, 1080});
    EXPECT_TRUE(exclusion.forMonitor(CBox{0, 0, 1920, 1080}, 1.F).empty());
}

TEST(WorkspaceBlurExclusion, MultipleLayersKeepDisjointExemptions) {
    CWorkspaceBlurExclusion exclusion;
    exclusion.addLayer(CRegion{}, CBox{100, 100, 100, 100});
    exclusion.addLayer(CRegion{}, CBox{300, 100, 100, 100});
    auto region = exclusion.forMonitor(CBox{0, 0, 1920, 1080}, 2.F);
    EXPECT_TRUE(CRegion{region}.intersect(CBox{400, 200, 200, 200}).empty());
    EXPECT_FALSE(CRegion{region}.intersect(CBox{600, 200, 200, 200}).empty());
}
