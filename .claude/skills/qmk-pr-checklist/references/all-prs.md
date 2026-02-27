# Requirements for All PRs

These requirements apply to every QMK pull request regardless of type.

## Branch Requirements

- PRs must use a non-`master` branch on the source repository.
- Contributors who commit to their own `master` branch receive guidance post-merge directing them to best practices documentation.
- Frequent upstream merges are discouraged; only merge upstream to resolve conflicts or pull in relevant changes.

## Scope

- PRs should contain the smallest amount of modifications required for a single change.
- Multiple keyboards in one PR are not acceptable.
- Smaller PRs receive faster reviews and have lower conflict likelihood.

## File Naming

- Newly-added directories and filenames must be lowercase.
- Uppercase is acceptable only when upstream sources originally had uppercase (LUFA, ChibiOS, imported files).
- Valid justification required for exceptions (consistency with existing core files). Board designer naming preferences are not sufficient justification.

## Licensing

- Valid license headers required on all `*.c` and `*.h` source files.
- GPL2/GPL3 recommended for consistency.
- GPL-compatible licenses permitted if they allow redistribution.
- Different licenses may delay merging.
- Missing headers prevent merge due to license ambiguity.
- Simple assignment-only `rules.mk` files typically don't need headers; logic-heavy files may require them.

See `references/license-headers.md` for header templates.

## Code Best Practices

### Include Guards

Use `#pragma once` instead of `#ifndef` include guards in header files.

### Hardware Abstractions

Do not use old-school or low-level GPIO/I2C/SPI functions without justification. Use QMK abstraction layers instead.

### Timing Abstractions

- Use `wait_ms()` instead of `_delay_ms()` (remove `#include <util/delay.h>`)
- Use `timer_read()` and `timer_read32()` per the timer.h APIs

### New Abstractions

Encouraged workflow for new abstractions:
1. Prototype in your own keyboard
2. Discuss on Discord
3. Raise as a separate core change PR
4. Remove from your board once merged to core

## Merge Conflicts

- Fix all merge conflicts before opening a PR.
- PR submitters must keep their branches updated with the base branch, resolving conflicts as needed.
- Reach out to QMK Collaborators on Discord for help or advice with conflict resolution.
