# Keyboard PR Requirements

This is the most detailed section of the PR checklist. See [closed keyboard PRs](https://github.com/qmk/qmk_firmware/pulls?q=is%3Apr+is%3Aclosed+label%3Akeyboard) for review examples.

## Repository Movement

- Keyboard moves must use the `develop` branch (not `master`) for user compatibility.
- Update `data/mappings/keyboard_aliases.hjson` to maintain Configurator detection.
- Keyboard updates/refactors must use `develop` to reduce merge conflicts.

## kbfirmware Imports

- kbfirmware exports are unacceptable unless converted to QMK standards.
- Use `qmk import-kbfirmware` to convert first.

## info.json

### Mandatory Elements

- Valid URL
- Valid maintainer
- Valid USB VID/PID and device version
- Display correctly in Configurator (use Ctrl+Shift+I preview, enable fast input for ordering)
- Layout definitions with matrix positions enabling LAYOUT macro generation at build time
- Community Layout macro names where applicable (preferred over `LAYOUT`/`LAYOUT_all`)
- Microcontroller and bootloader
- Diode direction (if not using direct pins)

### Layout Naming Rules

- **Single electrical layout**: Use `LAYOUT` unless a community layout name exists.
- **Multiple layouts**: Include `LAYOUT_all` with all positions. Use alternate names for specific layouts (prefer community layout names like `LAYOUT_tkl_ansi`, `LAYOUT_ortho_4x4`).
- Standard/community layout definitions are always preferred.

### Conditional Configuration (where applicable)

- Direct pin configuration
- Backlight configuration
- Split keyboard configuration
- Encoder configuration
- Bootmagic configuration
- LED indicator configuration
- RGB Light configuration
- RGB Matrix configuration

### Formatting

Run `qmk format-json` before submitting. Use `-i` flag to modify files directly:
```bash
qmk format-json -i keyboards/<name>/info.json
```

## readme.md

- Follow the [official template](https://github.com/qmk/qmk_firmware/blob/master/data/templates/keyboard/readme.md).
- Flash command must be present with `:flash` at the end.
- Valid hardware availability link required (unless handwired). Private group buys acceptable; one-off prototypes may be questioned.
- Open-source designs require links to source files.
- Clear bootloader reset instructions.
- Keyboard and PCB pictures preferred.
- Images must be hosted externally (imgur recommended), not in the repository.
- Use direct image links, not preview URLs (e.g., `https://i.imgur.com/vqgE7Ok.jpg`).

## rules.mk

### Remove Deprecated Features

- Remove `MIDI_ENABLE`
- Remove `FAUXCLICKY_ENABLE`
- Remove `HD44780_ENABLE`

### Cleanup

- Change `# Enable Bluetooth with the Adafruit EZ-Key HID` to `# Enable Bluetooth`
- Remove `(-/+size)` feature comments
- Remove alternate bootloader lists if one is already specified
- No MCU parameter redefinitions matching `builddefs/mcu_selection.mk`

### Keyboard-Level Feature Restrictions

Do not enable "keymap only" features at the keyboard level:
- `COMBO_ENABLE`
- `ENCODER_MAP_ENABLE`

## config.h

### Prohibited Defines

- No `#define DESCRIPTION`
- No Magic Key, MIDI, or HD44780 configuration
- No `#include "config_common.h"`
- No `#define MATRIX_ROWS` / `#define MATRIX_COLS` unless using custom matrix

### Avoid

- User preference defines at keyboard level
- Redefining default values (DEBOUNCE, RGB settings, etc.)
- Copy/pasted feature explanation comments
- Commented-out unused defines

### Boot Code

Keep bare minimum boot code:
- Matrix and critical device initialization only
- Custom keycodes/animations should be handled through non-default keymaps

### Vial

No Vial-related files or changes (not used by QMK firmware).

## keyboard.c

- Remove empty `xxxx_xxxx_kb()`, `xxxx_xxxx_user()`, or other weak default implementations.
- Remove commented-out functions.
- Migrate `matrix_init_board()` etc. to `keyboard_pre_init_kb()` per documentation.
- Custom matrix: use the 'lite' variant where possible for standard debounce. Full custom matrix requires justification.
- Prefer LED Indicator Configuration Options over custom `led_update_*()` implementations.
- Hardware enabled at keyboard level that requires configuration should have a basic implementation.

## keyboard.h

- `#include "quantum.h"` at the top.
- LAYOUT macros must be in `info.json`, not in this file (no longer accepted here).

## Default Keymap (keymaps/default/keymap.c)

### Pristine Baseline

Default keymaps serve as pristine baselines for user customization:
- No custom keycodes or advanced features (tap dance, macros).
- Basic mod-taps/home row mods acceptable where necessary.
- Standard layouts preferred.

### Cleanup

- Remove `QMKBEST`/`QMKURL` example macros.
- Replace manual `MO(1)`/`MO(2)` layer access with the Tri Layer feature.
- Avoid manual `layer_on()`/`layer_off()` and `update_tri_layer()` implementations.

### Encoders

Use the encoder map feature, not `encoder_update_user()`.

### VIA

Do not enable VIA in the default keymap. Submit VIA keymaps to [VIA QMK Userspace](https://github.com/the-via/qmk_userspace_via).

## Keymap config.h

No duplication of keyboard-level `rules.mk` or `config.h` content.

## Additional Keymaps

- Submitters may add an example/"bells-and-whistles" keymap showcasing keyboard capabilities in the same PR.
- "Manufacturer-matching" keymaps are permitted for porting existing boards.
- Advanced features should not be embedded in the default keymap.

## File Restrictions

- No VIA JSON files (belong in the [VIA Keyboard Repo](https://github.com/the-via/keyboards)).
- No KLE JSON files (no QMK use).
- No source files from other keyboards or vendor folders (core files are acceptable).
  - Example: Only `wilba_tech` includes `keyboards/wilba_tech/wt_main.c`; drivers like `drivers/sensors/pmw3360.c` are acceptable for all boards.
- Multi-board code is a candidate for a core change instead.

## Wireless / Bluetooth

- QMK does not accept vendor wireless/Bluetooth PRs without the wireless code included.
- GPL2+ requires full source disclosure for distributed binaries.
- Missing wireless code leaves the PR on-hold/unmergeable until source is provided.
- Future PRs from vendors with previously merged wireless boards lacking sources will also be held.

## ChibiOS-Specific Requirements

- **Strong preference** for existing ChibiOS board definitions.
- Prefer equivalent Nucleo board with different flash size/model. Example: STM32L082KZ should use `BOARD = ST_NUCLEO64_L073RZ` in rules.mk.
- QMK is migrating away from custom board definitions (maintenance burden).
- New board definitions are prohibited in keyboard PRs (raise under Core PRs instead).
- If custom `board.c` is unavoidable:
  - `__early_init()` is replaced by `early_hardware_init_pre()` or `early_hardware_init_post()`
  - `boardInit()` is migrated to `board_init()`
