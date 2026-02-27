# Core / Quantum PR Requirements

## Branch Target

Core PRs must target the `develop` branch (merged to `master` on the breaking changes timeline).

## Scope

- Smallest change sets across core components.
- PRs affecting multiple areas should be split and raised separately.
- Keyboard/keymap changes included only if they affect base builds or `default`-like keymaps.
- Non-default keymap modifications should be excluded from the initial PR and submitted as a separate follow-up PR post-merge.
- Large-scale refactoring that affects other keymaps should be raised separately.

## New Hardware Support

### Test Board Requirement

New hardware support requires a test board under `keyboards/handwired/onekey/`:

- **New MCUs**: Add a "child" keyboard targeting the new MCU for build verification.
- **New hardware** (displays, custom matrix, peripherals): Provide an associated keymap demonstrating the functionality.
- Existing keymaps that already leverage the functionality may not require a new keymap (consult Discord).

## Callback Requirements

- New `_kb`/`_user` callbacks must return `bool` to allow user override.

## Testing

- Unit tests are strongly recommended; they boost confidence in correctness.
- Critical code areas (e.g., keycode processing pipeline) almost certainly require unit tests.
- Expect collaborator requests for tests on critical functionality.

## Review Process

- Two or more meaningful approvals required before merge consideration.
- Reviews are not limited to collaborators; community reviewers are welcomed and encouraged.
- Community review checkmarks may not appear green but are still considered valid.
- Reviews are unpaid labor performed in free time. Family and life may delay PR reviews. Patience is requested.
- The repository averages ~200 opened/merged PRs per month.

## Other Requirements

Additional requirements are at the discretion of QMK collaborators. Core changes are highly subjective given the breadth of impact.
