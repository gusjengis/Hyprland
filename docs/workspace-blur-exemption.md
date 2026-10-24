# Layer exemptions from workspace blur

`no_workspace_blur` is an opt-in Boolean layer rule. It keeps background/bottom
layer surfaces sharp under occupied-workspace blur and special-workspace blur,
without raising them above application windows or disabling wallpaper blur.

```lua
hl.layer_rule({
    name = "desktop-widgets-stay-sharp",
    match = { namespace = "^my-desktop-widgets$" },
    no_workspace_blur = true,
})
```

The exemption follows the layer surface's current input region, clipped to its
surface bounds. This lets clients describe rounded cards without protecting
transparent padding. A surface with an empty input region (a click-through
layer) instead protects its full surface bounds. Top/overlay layers already
render after workspace blur and do not need this rule.

Only compositing of the full-screen workspace/special blur is masked. Blur
sampling and its damage expansion remain unchanged, including neighboring
pixels. Ordinary application windows overlapping a protected desktop layer
retain special-workspace blur; their decorated bounds occlude the exemption.
Fullscreen and special windows still cover the desktop normally. Window blur,
layer material blur, and special-workspace dimming are independent effects.

Default behavior is unchanged unless a layer matches this rule. The rule resets
and reapplies through the normal layer-rule lifecycle.

Lua configurations shared across versions can feature-detect before declaring
the rule: `hl.supports_layer_rule_effect and
hl.supports_layer_rule_effect("no_workspace_blur")`. The query also recognizes
existing static and dynamically registered layer effects.
