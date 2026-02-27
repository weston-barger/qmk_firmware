# Keymap PR Requirements

## Personal Keymaps No Longer Accepted

Personal keymap submissions are no longer accepted. Only manufacturer-supported keymaps are permitted (see GitHub issue #22724).

## Vendor Keymap Naming

- PRs for vendor-specific keymaps are permitted.
- Use naming convention `default_${vendor}` (e.g., `default_clueboard`).
- Vendor keymaps need not be "vanilla" and can be feature-rich versus stock defaults.

## Code Standards

- Prefer `#include QMK_KEYBOARD_H` over specific board file includes.
- Prefer layer enums over `#define` constants.
- Custom keycode enums must have first entry = `QK_USER`.
- Spacing care preferred (alignment on commas or keycode first characters); use spaces over tabs.

## VIA Compatibility

- Keymaps must NOT enable VIA.
- VIA-targeting keymaps should be submitted to the [VIA QMK Userspace repository](https://github.com/the-via/qmk_userspace_via).
