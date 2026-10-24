#pragma once

#include <hyprutils/math/Box.hpp>
#include <hyprutils/math/Region.hpp>

namespace Render {
    // Logical global coordinates until the final monitor-space conversion.
    class CWorkspaceBlurExclusion {
      public:
        void                     addLayer(const Hyprutils::Math::CRegion& input, const Hyprutils::Math::CBox& surface);
        void                     occlude(const Hyprutils::Math::CBox& window);
        bool                     empty() const;
        Hyprutils::Math::CRegion forMonitor(const Hyprutils::Math::CBox& monitor, float scale) const;

      private:
        Hyprutils::Math::CRegion m_region;
    };
}
