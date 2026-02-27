---
name: qmk-pr-checklist
description: QMK pull request checklist and review guidelines based on https://docs.qmk.fm/pr_checklist. Use when preparing, reviewing, or auditing a QMK firmware pull request. Covers requirements for all PR types including keyboard PRs, keymap PRs, and core/quantum PRs. Triggers: PR review, PR checklist, PR readiness, submit PR, open PR, QMK contribution guidelines.
---

# QMK PR Checklist

Review guidelines for QMK firmware pull requests, based on the official [PR Checklist](https://docs.qmk.fm/pr_checklist).

## Workflow

1. **Determine PR type** using the classification below.
2. **Apply universal requirements** from `references/all-prs.md`.
3. **Apply type-specific checklist** from the corresponding reference file.
4. **Verify licensing** per `references/license-headers.md`.
5. **Run validation commands** (see below).
6. **Check branch targeting** (see below).

## PR Type Classification

Determine the PR type based on which files are modified:

| Modified paths | PR Type | Reference |
|---|---|---|
| New or modified `keyboards/` definitions (info.json, keyboard.c/h, rules.mk, config.h, readme.md) | **Keyboard** | `references/keyboard-prs.md` |
| Only `keymaps/` files (keymap.c, keymap config.h, keymap rules.mk) | **Keymap** | `references/keymap-prs.md` |
| `quantum/`, `tmk_core/`, `platforms/`, `drivers/`, `builddefs/` | **Core** | `references/core-prs.md` |

A PR may span multiple types (e.g., a keyboard PR that also adds a keymap). Apply all relevant checklists.

## Quick Reference by Type

### All PRs
- Non-`master` branch, minimal scope, single change
- Lowercase filenames (exceptions: LUFA, ChibiOS upstream)
- GPL2/3 license headers on all `.c` and `.h` files
- `#pragma once` in headers
- QMK abstractions for GPIO/I2C/SPI, `wait_ms()`, `timer_read()`/`timer_read32()`
- No merge conflicts

### Keyboard PRs
- info.json: valid URL, maintainer, USB VID/PID, layouts with matrix positions, MCU/bootloader, diode direction
- Layout naming: `LAYOUT` for single, `LAYOUT_all` for all positions, community names preferred
- readme.md: follows template, flash command with `:flash`, hardware link, reset instructions, external images
- rules.mk: no deprecated features (`MIDI_ENABLE`, `FAUXCLICKY_ENABLE`, `HD44780_ENABLE`), no keymap-only features at keyboard level
- config.h: no `DESCRIPTION`, no `config_common.h`, no default redefinitions, no `MATRIX_ROWS/COLS` unless custom matrix
- keyboard.c: no empty weak functions, migrate `matrix_init_board` to `keyboard_pre_init_kb`
- keyboard.h: `#include "quantum.h"`, no LAYOUT macros (use info.json)
- Default keymap: pristine, no custom keycodes, use encoder map, Tri Layer, no VIA
- No VIA JSON, no KLE JSON, no cross-keyboard source files

### Keymap PRs
- Personal keymaps no longer accepted (manufacturer-supported only)
- Vendor naming: `default_${vendor}`
- `#include QMK_KEYBOARD_H`, layer enums over `#define`, first custom keycode = `QK_USER`
- No VIA

### Core PRs
- Target `develop` branch
- Smallest change set, split multi-area changes
- New hardware: test board under `keyboards/handwired/onekey/`
- New `_kb`/`_user` callbacks must return `bool`
- Unit tests strongly recommended, critical code almost certainly requires them
- Two or more meaningful approvals before merge

## Branch Targeting

| PR Type | Target Branch |
|---|---|
| Most keyboard PRs (new keyboards) | `master` |
| Keyboard moves/renames | `develop` |
| Keyboard refactors (data-driven migration) | `develop` |
| Keymap PRs | `master` |
| Core / quantum PRs | `develop` |

## Validation Commands

Run these before submitting a PR:

```bash
# Lint keyboard definitions
qmk lint -kb <keyboard_name>

# Format JSON files in-place
qmk format-json -i keyboards/<name>/info.json

# Format Python code
qmk format-python

# Run tests and linting
qmk pytest

# Check for merge conflicts with target branch
git fetch origin && git merge --no-commit --no-ff origin/<target_branch> && git merge --abort
```

## Source

Content based on https://docs.qmk.fm/pr_checklist. Last synced: 2026-02-28.
