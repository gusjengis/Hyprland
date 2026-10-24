#include "WorkspaceBlurExclusion.hpp"

using namespace Hyprutils::Math;
using namespace Render;

void CWorkspaceBlurExclusion::addLayer(const CRegion& input, const CBox& surface) {
    CRegion    local = input;
    const CBox bounds{0, 0, surface.w, surface.h};
    if (local.empty())
        local = bounds;
    else
        local.intersect(bounds);
    local.translate(surface.pos());
    m_region.add(local);
}

void CWorkspaceBlurExclusion::occlude(const CBox& window) {
    m_region.subtract(window);
}

bool CWorkspaceBlurExclusion::empty() const {
    return m_region.empty();
}

CRegion CWorkspaceBlurExclusion::forMonitor(const CBox& monitor, float scale) const {
    CRegion region = m_region;
    region.intersect(monitor);
    region.translate(-monitor.pos());
    return region.scale(scale);
}
