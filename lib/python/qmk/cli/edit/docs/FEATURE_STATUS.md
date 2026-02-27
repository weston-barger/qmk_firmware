# Feature Status: `qmk edit`

Last updated: 2026-02-22

This document tracks which PRD features have been implemented, which are partially done, and which remain. Features are grouped by PRD section.

---

## Fully Implemented

These features are complete and tested.

### 4.1.1 Keyboard Selection
- `qmk edit -kb <keyboard>` command with argument parsing and tab completion
- Keyboard folder validation via decorator
- Automatic detection of `keyboard.json` vs `info.json`
- Clear error message if keyboard not found

### 4.1.2 Schema-Driven UI Generation
- Schema loaded from `data/schemas/keyboard.jsonschema` at startup
- `$ref` resolution (relative, local, schema_store)
- Widget registry maps schema types to UI widgets automatically
- All six field types have widget implementations: string, boolean, integer/number, enum, array, object
- New schema fields appear without code changes

### 4.1.3 Navigation Structure
- Flat field list with schema definition order preserved
- Object properties become navigable sub-categories
- Search/filter: live filtering by search term, toggle with `/`, exit with Esc/Enter, focus restoration after search
- Breadcrumb display showing current path (e.g., `QMK Edit: keyboards/name > usb > shared_endpoint`)

### 4.1.4 Field Display
- All schema-defined fields shown regardless of presence in file
- Current values displayed; blank/`[-]` for undefined fields
- Unknown fields preserved in data (not displayed, but saved back to file)

### 4.1.6 Input Types
- **Text Input**: Free-form text entry via `TextField`; empty string converts to None (unset)
- **Boolean Toggle**: Tri-state cycle (None/True/False) via `BooleanField` with `[ ]`/`[x]`/`[-]` indicators
- **Numeric Input**: Integer/float parsing with min/max constraint validation via `NumericField`; hex format support for USB VID/PID via path-specific override; invalid input reverts to previous value
- **Enum Picker**: Dropdown with `(unset)` option via `EnumField`; all schema enum values shown
- **Array Editor**: Navigation into arrays, per-item display with `[index]`, add (Ctrl-N), delete (Ctrl-D with confirmation), move up/down, respects `maxItems` constraint, scaffolds new items with defaults based on type/required fields
- **Object Editor**: Navigation into nested objects, breadcrumb path display, back navigation

### 4.1.8 Persistence
- Explicit save via Ctrl+S
- `[Modified]` indicator in breadcrumb when changes exist
- Exit prompt with Save/Discard/Cancel when unsaved changes exist
- Output via `InfoJSONEncoder` (4-space indent, sorted keys)
- Backup `.bak` file created before overwriting
- Comments stripped on save (matches `qmk format-json` behavior)

### 4.3.1 Default Keybindings
- Arrow keys and vim j/k for navigation
- Enter to select/edit, Esc to back/cancel
- Ctrl+S save, `/` search, `q` quit, `?` help (keybinding defined but no help dialog — see below)

### 4.3.2 List-Specific Keybindings
- Ctrl-N add, Ctrl-D delete, Ctrl-K/J and Ctrl-Up/Down for move up/down
- Configurable via `Keymap` class with override support

### Other
- Terminal size validation (minimum 80x24)
- Resize handling via `ResizeScreenError` loop
- `EditorConfig` with `--no-confirm-delete` CLI flag
- Comprehensive unit tests for state, file manager, schema loader, keymap, widgets, arrays, registry, config (~230+ tests across 13 files)

---

## Partially Implemented

### 4.1.7 Validation
**What works:** Full JSON schema validation on save; validation errors displayed in `ErrorDialog` with field paths and messages; save is blocked when validation fails.

**What's missing:** PRD specifies "option to force save" when validation fails (section 4.1.7: "Prevent save if validation fails (with option to force)"). Currently the save is hard-blocked with no force override.

### 4.3.3 Keybinding Configuration
**What works:** `Keymap` class supports custom overrides via constructor parameter. `EditorConfig` provides runtime config from CLI flags.

**What's missing:** No persistent config file. PRD specifies keybindings stored in a config file (`~/.config/qmk/edit.conf` or similar) loaded at startup. Currently keybindings can only be customized programmatically, not by end users.

### Testing — TUI Integration Tests
**What works:** Extensive unit tests for all non-UI modules. Basic structural tests for views (array mode detection, breadcrumb formatting).

**What's missing:** No integration tests for TUI rendering, keyboard event handling, search UX, or dialog interactions. Blocked by lack of headless asciimatics testing infrastructure (noted in TODO comments across all view/widget files).

---

## Fully Missing Features

### 4.1.5 Revision/Inheritance Handling
PRD specifies:
- Show values inherited from parent keyboard folders as read-only context
- Clear visual indicator distinguishing inherited vs. locally-defined values (e.g., `(inherited)` label)
- Edit only the exact keyboard path specified

**Current state:** The editor loads and edits a single file. No parent folder config loading or inheritance display exists. The `FileManager` loads one file only. The PRD mockup (section 4.2.2) shows an `(inherited)` label next to fields — this is not implemented anywhere.

**Context for implementation:** QMK keyboards use folder hierarchy inheritance (e.g., `keyboards/planck/info.json` inherits from `keyboards/planck/rev6/info.json`). Showing inherited values requires loading the merged config from parent folders and comparing against the leaf file to determine which values are local vs. inherited.

### Help Dialog (PRD 4.3.1 / 4.2)
PRD specifies a `?` keybinding that shows a help dialog with keybinding reference. The `?` key is mapped in `Keymap` as the `help` action, but no `HelpDialog` view exists and no handler processes the `help` action. The `views/` directory has no `help_dialog.py` file.

### Tab Key — Next Field in Edit Mode (PRD 4.3.1)
PRD specifies Tab to move to the next field while in edit mode. No Tab handling is implemented. Currently Tab behavior falls through to default asciimatics widget handling (which may or may not cycle focus depending on context).

### Force Save Option (PRD 4.1.7)
PRD specifies "Prevent save if validation fails (with option to force)." The current implementation hard-blocks save on validation failure with no force bypass. A force option would allow saving despite schema errors, which can be useful during development.

### Layouts Array Editing (PRD 4.1.6) (additionalProperties more generally)
The `layouts` property contains keyboard layout definitions (pin position arrays). The editor currently shows a read-only message: "Layout editing not yet supported" when navigating to `layouts.*.layout`. This is an explicit deferral noted in the Phase 3 design doc, not a bug.

### (Weston) Shitty behavior for validation and no ad-hoc validation 
Right no we only validate on save and the behavior is ass. 

NOTE: disabling the TAB key depends on this because right now it's the only way

### (Weston) Display type information about a field
Somehow help users. Otherwise they need to know the schema, somewhat defeating the purpose of the tool.

---

## Out of Scope (PRD Section 8)

These are explicitly excluded from v1.0 per the PRD:
- Creating new keyboards from scratch
- Editing `config.h` or `rules.mk` files
- Editing keymap files
- Inline help/documentation beyond schema
- Integration with compile/flash/lint commands
- Diff view before save
- Vim-style keybindings (basic vim j/k navigation IS implemented, but full vim modal editing is not)
- Undo/redo within editor session
- Multi-file editing
- Remote/API-based editing

---

## Implementation Priority Suggestion

For planning the next features, roughly ordered by user impact:

0.0 **Fix bugs** - TODO-SHIT.md
0.1 **Validation** - fix behavior and perhaps have adhoc validation
0.2 **Type info** - somehow display type info for a field
1. **Help Dialog** — Low complexity, high discoverability value. The keybinding already exists.
2. **Force Save Option** — Low complexity. Add a button to the error dialog or a CLI flag.
3. **Inheritance Display** — High complexity, high value for keyboard designers. Requires loading parent configs and diffing against leaf file.
4. **Keybinding Config File** — Medium complexity. Load JSON/INI from `~/.config/qmk/edit.conf` and pass as Keymap overrides.
5. **Tab Next Field** — Low complexity. Wire Tab to advance focus in edit mode.
6. **Layouts Editing (additionalProperties)** — High complexity. Requires a specialized UI for position arrays.
