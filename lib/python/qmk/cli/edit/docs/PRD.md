# Product Requirements Document: QMK Keyboard Editor TUI

## Document Information
- **Product Name**: `qmk edit`
- **Version**: 1.0
- **Last Updated**: 2026-02-04
- **Status**: Draft

---

## 1. Executive Summary

### 1.1 Problem Statement
QMK keyboard configuration is currently fragmented and complex. Users must manually edit JSON files, often without knowing what fields are available or valid. The official documentation acknowledges QMK is "more powerful, but harder to use." Common pain points include:

- Steep learning curve for configuration options
- No discoverability of available settings
- Easy to introduce syntax errors in JSON
- Documentation gaps between schema and user-facing docs
- Configuration scattered across multiple file formats historically

### 1.2 Proposed Solution
A terminal-based user interface (TUI) for editing keyboard configuration files (`info.json`/`keyboard.json`), invoked via `qmk edit -kb <keyboard>`. The interface is schema-driven, meaning it automatically reflects all valid configuration options defined in `data/schemas/keyboard.jsonschema` without requiring code changes when the schema evolves.

### 1.3 Success Metrics
- **Adoption**: 20% of active QMK contributors use the editor within 6 months
- **Error Reduction**: 50% reduction in lint errors related to info.json in PRs
- **Discoverability**: Users can find and configure features without consulting external docs

---

## 2. Background & Research

### 2.1 QMK Framework Overview
QMK (Quantum Mechanical Keyboard) is open-source firmware supporting 3,000+ keyboards across Atmel AVR and ARM processors. It provides extensive customization including:

- Key remapping and layers
- Macros and tap-dance
- RGB lighting (matrix and underglow)
- Rotary encoders
- Split keyboard communication
- Audio and haptic feedback
- Mouse keys and joystick emulation

### 2.2 User Personas

#### Keyboard Designers
- Porting new hardware to QMK
- Need to configure: processor, bootloader, GPIO pins, matrix definition, development board
- Pain points: Understanding pin naming conventions, bootloader selection, matrix wiring

#### Power Users
- Tweaking existing keyboards
- Need to configure: RGB settings, tapping behavior, debounce timing, feature toggles
- Pain points: Discovering available options, understanding valid value ranges

### 2.3 Competitive Analysis

| Tool | Strengths | Weaknesses |
|------|-----------|------------|
| **raspi-config** | Simple menu navigation, persistent changes | Limited to predefined options, no schema |
| **make menuconfig** | Schema-driven (Kconfig), hierarchical | Complex dependency system, steep learning curve |
| **QMK Configurator** | Web-based, visual keymap editing | Only keymaps, not hardware config |
| **VIA** | Real-time changes, no reflash | Limited to VIA-enabled keyboards |

### 2.4 Technical Constraints
- Must work across Linux, macOS, and Windows (limited to Windows 10+ for ANSI support)
- Minimal additional dependencies (asciimatics for TUI)
- Must integrate with existing QMK CLI infrastructure
- Schema is the single source of truth

---

## 3. Target Users

### 3.1 Primary Users
Both keyboard designers and power users are primary targets with equal priority.

### 3.2 User Stories

**As a keyboard designer**, I want to:
- See all available configuration options for my processor type
- Set matrix pin assignments with validation
- Configure split keyboard communication settings
- Understand what bootloader options are available for my MCU

**As a power user**, I want to:
- Discover what RGB animations are available
- Adjust tapping term and behavior settings
- Enable/disable features without editing raw JSON
- See what my current configuration values are

---

## 4. Functional Requirements

### 4.1 Core Features

#### 4.1.1 Keyboard Selection
- **Command**: `qmk edit -kb <keyboard>`
- **Behavior**: Load the specified keyboard's `info.json` or `keyboard.json`
- **Validation**: Verify keyboard exists before launching editor
- **Error handling**: Clear error message if keyboard not found

#### 4.1.2 Schema-Driven UI Generation
- **Source**: `data/schemas/keyboard.jsonschema`
- **Behavior**: Automatically generate UI elements for all schema-defined fields
- **Field types supported**:
  - `string` → Text input field
  - `boolean` → Toggle/checkbox
  - `integer`/`number` → Numeric input with min/max validation
  - `enum` → Dropdown picker
  - `array` → List view with add/remove/edit
  - `object` → Nested category/section
- **Updates**: New schema fields appear in editor without code changes

#### 4.1.3 Navigation Structure
- **Layout**: Flat field list with search/filter capability
- **Categories**: Derived directly from top-level schema properties in schema definition order
  - Each top-level property in `keyboard.jsonschema` becomes a category
  - No hardcoded category names or groupings - fully schema-driven
  - Simple properties (string, boolean, number) displayed directly
  - Object properties become navigable sub-categories
- **Search**: Filter categories and fields by typing search terms
- **Order**: Categories appear in the same order as defined in the JSON schema file

#### 4.1.4 Field Display
- **Visibility**: Show all fields defined in schema, regardless of whether they exist in current file
- **Current values**: Display current value from file, or blank for undefined fields
- **Dependencies**: No conditional hiding (show all fields always)
- **Unknown fields**: Preserve in file but do not display in editor

#### 4.1.5 Revision/Inheritance Handling
- **Specified level**: Edit the exact keyboard path specified
- **Inherited values**: Show values inherited from parent folders as read-only context
- **Visual distinction**: Clear indicator for inherited vs. locally-defined values

#### 4.1.6 Input Types

**Text Input**
- Free-form text entry
- Schema pattern validation on save

**Boolean Toggle**
- Visual toggle between true/false/unset
- Clear indication of current state

**Numeric Input**
- Text entry with numeric validation
- Respect schema minimum/maximum constraints
- Support for hex notation where applicable (USB VID/PID)

**Enum Picker (Dropdown)**
- Scrollable list of valid options
- Arrow key navigation
- Enter to select
- Escape to cancel
- Show all enum values from schema

**Array Editor (List View)**
- Display each element on its own line
- Add new element (append)
- Remove element (with confirmation for complex objects)
- Edit element (inline for simple types, sub-screen for objects)
- Reorder elements (move up/down)

**Object Editor**
- Navigate into nested object as sub-category
- Breadcrumb showing current path (e.g., "split > transport > sync")
- Back navigation to parent

#### 4.1.7 Validation
- **Timing**: Validate on save only
- **Schema validation**: Check against JSON schema
- **Error display**: Show all validation errors with field paths
- **Save blocking**: Prevent save if validation fails (with option to force)

#### 4.1.8 Persistence
- **Save action**: Explicit save (Ctrl+S or menu option)
- **Unsaved indicator**: Visual indicator when changes exist
- **Exit behavior**: Prompt if exiting with unsaved changes
- **File format**: QMK's InfoJSONEncoder format (4-space indent, sorted keys)
- **Comment handling**: Comments in source file will be stripped on save (matches `qmk format-json` behavior)

### 4.2 User Interface

#### 4.2.1 Screen Layout
```
┌─────────────────────────────────────────────────────────────┐
│ QMK Edit: keyboards/planck/rev6                    [Modified]│
├─────────────────────────────────────────────────────────────┤
│ Search: [____________]                                       │
├─────────────────────────────────────────────────────────────┤
│ ▸ keyboard_name          [Planck]                           │
│   maintainer             [jackhumbert]                      │
│   manufacturer           [OLKB]                             │
│   url                    [https://olkb.com]                 │
│   processor              [STM32F303]                        │
│   bootloader             [stm32-dfu]                        │
│   usb                    [→]                                │
│   features               [→]                                │
│   matrix_pins            [→]                                │
│   ...                                                       │
├─────────────────────────────────────────────────────────────┤
│ ↑↓ Navigate  Enter Select  / Search  Ctrl+S Save  q Quit   │
└─────────────────────────────────────────────────────────────┘
```
Note: Categories/fields appear in schema definition order, not grouped by topic.

#### 4.2.2 Category Detail View
```
┌─────────────────────────────────────────────────────────────┐
│ QMK Edit: keyboards/planck/rev6 > USB              [Modified]│
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   vid                    [0x03A8]                           │
│   pid                    [0x0001]                           │
│   device_version         [1.0.0 ]                           │
│   max_power              [500   ]                           │
│   polling_interval       [1     ]                           │
│   no_startup_check       [ ] (inherited)                    │
│   wait_for_enumeration   [ ]                                │
│ ▸ shared_endpoint        [→]                                │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│ ↑↓ Navigate  Enter Edit  Esc Back  Ctrl+S Save  q Quit     │
└─────────────────────────────────────────────────────────────┘
```

#### 4.2.3 Enum Picker
```
┌─────────────────────────────────────────────────────────────┐
│ Select bootloader:                                          │
├─────────────────────────────────────────────────────────────┤
│   atmel-dfu                                                 │
│   caterina                                                  │
│ ▸ lufa-dfu                                                  │
│   qmk-dfu                                                   │
│   rp2040                                                    │
│   stm32-dfu                                                 │
│   tinyuf2                                                   │
│   uf2boot                                                   │
├─────────────────────────────────────────────────────────────┤
│ ↑↓ Navigate  Enter Select  Esc Cancel                      │
└─────────────────────────────────────────────────────────────┘
```

#### 4.2.4 List Editor
```
┌─────────────────────────────────────────────────────────────┐
│ Edit: matrix_pins > cols                                    │
├─────────────────────────────────────────────────────────────┤
│   [0] GP0                                                   │
│   [1] GP1                                                   │
│ ▸ [2] GP2                                                   │
│   [3] GP3                                                   │
│   [4] GP4                                                   │
│   [+] Add new item                                          │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│ ↑↓ Navigate  Enter Edit  d Delete  + Add  Esc Back         │
└─────────────────────────────────────────────────────────────┘
```

### 4.3 Keybindings

#### 4.3.1 Default Keybindings
| Key | Action |
|-----|--------|
| ↑/↓ | Navigate up/down |
| Enter | Select/Edit |
| Esc | Back/Cancel |
| Tab | Next field (in edit mode) |
| Ctrl+S | Save |
| / | Focus search |
| q | Quit (prompts if unsaved) |
| ? | Show help (keybinding reference) |

#### 4.3.2 List-Specific Keybindings
| Key | Action |
|-----|--------|
| + or a | Add new item |
| d or Delete | Delete selected item |
| Ctrl+↑ | Move item up |
| Ctrl+↓ | Move item down |

#### 4.3.3 Configuration
- Keybindings stored in config file (location TBD, likely `~/.config/qmk/edit.conf`)
- Format: INI or JSON
- Extensible for future vim-style bindings

---

## 5. Non-Functional Requirements

### 5.1 Performance
- Editor launch time: < 1 second
- UI response time: < 100ms for navigation actions
- Save operation: < 500ms

### 5.2 Compatibility
- Python 3.9+ (matching QMK requirements)
- Linux, macOS, Windows 10+
- Terminal emulators: xterm, iTerm2, Windows Terminal, GNOME Terminal
- Minimum terminal size: 80x24

### 5.3 Accessibility
- Full keyboard navigation (no mouse required)
- High contrast mode support (inherits terminal colors)
- Screen reader compatibility (best effort with asciimatics)

### 5.4 Maintainability
- Schema-driven design minimizes code changes when schema evolves
- Clear separation: UI components, schema parsing, file I/O
- Comprehensive unit tests for schema interpretation
- Integration tests for common workflows

---

## 6. Technical Architecture

### 6.1 Technology Stack
- **Language**: Python 3.9+
- **TUI Framework**: asciimatics (chosen for native Windows support)
- **Schema Parsing**: jsonschema library (already a QMK dependency)
- **File Loading**: hjson library (already a QMK dependency) - supports comments in source files
- **File Saving**: json module with QMK's InfoJSONEncoder - produces standard JSON output
- **JSON Encoders**: `qmk.json_encoders.InfoJSONEncoder` for keyboard config formatting

### 6.2 Component Overview

```
┌─────────────────────────────────────────────────────────────┐
│                      qmk edit CLI                           │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐ │
│  │   Schema    │  │    File     │  │      Config         │ │
│  │   Loader    │  │   Manager   │  │      Manager        │ │
│  └──────┬──────┘  └──────┬──────┘  └──────────┬──────────┘ │
│         │                │                     │            │
│         ▼                ▼                     ▼            │
│  ┌─────────────────────────────────────────────────────────┐│
│  │                    Editor Core                          ││
│  │  ┌───────────┐ ┌───────────┐ ┌───────────────────────┐ ││
│  │  │  State    │ │ Validator │ │   Category Builder    │ ││
│  │  │  Manager  │ │           │ │   (schema → UI)       │ ││
│  │  └───────────┘ └───────────┘ └───────────────────────┘ ││
│  └─────────────────────────────────────────────────────────┘│
│                          │                                  │
│                          ▼                                  │
│  ┌─────────────────────────────────────────────────────────┐│
│  │                    UI Layer (asciimatics)               ││
│  │  ┌───────────┐ ┌───────────┐ ┌───────────┐ ┌─────────┐ ││
│  │  │  Main     │ │ Category  │ │  Field    │ │ Dialogs │ ││
│  │  │  Menu     │ │  View     │ │  Widgets  │ │         │ ││
│  │  └───────────┘ └───────────┘ └───────────┘ └─────────┘ ││
│  └─────────────────────────────────────────────────────────┘│
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 6.3 Key Classes/Modules

#### `schema_loader.py`
- Load and parse `keyboard.jsonschema`
- Resolve `$ref` references
- Extract field metadata (type, enum values, constraints)

#### `file_manager.py`
- Load keyboard JSON files using hjson (supports comments)
- Handle inheritance (parent folder configs)
- Save using json + InfoJSONEncoder (QMK standard format)
- Track modified state
- Note: Comments in source files will not survive save operation

#### `config_manager.py`
- Load user configuration (keybindings, preferences)
- Provide defaults

#### `editor_core.py`
- Central state management
- Coordinate between schema, file, and UI
- Handle save workflow with validation

#### `category_builder.py`
- Transform schema into UI category structure
- Map schema types to widget types
- Generate field metadata for display

#### `widgets/`
- `text_field.py` - Text input widget
- `boolean_field.py` - Toggle widget
- `numeric_field.py` - Number input with validation
- `enum_picker.py` - Dropdown selection widget
- `list_editor.py` - Array editing widget
- `object_viewer.py` - Nested object navigation

#### `views/`
- `main_menu.py` - Field list with search
- `category_view.py` - Field list for a category
- `help_dialog.py` - Keybinding reference

### 6.4 Data Flow

1. **Startup**:
   - Parse CLI arguments (`-kb <keyboard>`)
   - Load schema from `data/schemas/keyboard.jsonschema`
   - Load keyboard JSON using hjson (with inheritance resolution)
   - Load user config
   - Build UI from schema (categories in schema property order)
   - Populate with current values

2. **Navigation**:
   - User selects category → Load category view with fields
   - User selects field → Show appropriate editor widget
   - User modifies value → Update in-memory state, mark modified

3. **Save**:
   - Validate all fields against schema
   - If valid: Write to file using InfoJSONEncoder, clear modified flag
   - Note: File will be reformatted to QMK standard (comments stripped, keys sorted)
   - If invalid: Show error list, do not write

4. **Exit**:
   - Check modified flag
   - If modified: Prompt save/discard/cancel
   - Clean up asciimatics resources

---

## 7. Implementation Phases

### Phase 1: Foundation
- CLI integration (`qmk edit -kb`)
- Schema loading and parsing
- Basic asciimatics setup with main menu
- File loading (single file only, inheritance deferred to Phase 5)
- Field list from schema top-level properties

### Phase 2: Core Editing
- Field widgets for all basic types (string, boolean, number, enum)
- Object detail view (navigating into nested objects)
- In-memory state management
- Modified indicator

### Phase 3: Complex Types
- Array/list editor widget
- Object/nested navigation
- Breadcrumb navigation

### Phase 4: Persistence
- Save functionality with validation
- Unsaved changes prompt
- Output formatting using QMK's InfoJSONEncoder

### Phase 5: Polish
- Search/filter functionality
- Inheritance display (read-only parent values)
- User config for keybindings
- Help dialog

### Phase 6: Hardening
- Comprehensive error handling
- Edge case testing (empty files, invalid JSON, missing schema)
- Performance optimization
- Documentation

---

## 8. Out of Scope

The following are explicitly not included in version 1.0:

- Creating new keyboards from scratch
- Editing `config.h` or `rules.mk` files
- Editing keymap files
- Inline help/documentation beyond schema
- Integration with compile/flash/lint commands
- Diff view before save
- Vim-style keybindings (planned for future)
- Undo/redo within editor session
- Multi-file editing
- Remote/API-based editing

---

## 9. Risks & Mitigations

| Risk | Impact | Likelihood | Mitigation |
|------|--------|------------|------------|
| Schema complexity causes UI issues | High | Medium | Thorough testing with real keyboard schemas; graceful fallback for unsupported types |
| Schema changes break editor | Medium | High | Schema-driven design; CI tests against current schema |
| User data loss on crash | High | Low | Atomic writes; backup before save |
| Adoption resistance | Medium | Medium | Good documentation; demonstration videos |

---

## 10. Resolved Design Decisions

The following questions were evaluated and resolved:

### 10.1 Schema Location
**Decision**: No custom schema paths. Always use `data/schemas/keyboard.jsonschema`.

**Rationale**: Keeps implementation simple, avoids user confusion. Testing schema changes requires modifying the repo schema directly.

### 10.2 Default Values Display
**Decision**: Show blank for undefined fields.

**Rationale**: Clear distinction between "not set" and "explicitly set to default". Matches actual file state without risk of confusion about what's saved vs. implied.

### 10.3 Comments Handling
**Decision**: Use hjson for loading (existing QMK dependency), but output standard JSON via QMK's InfoJSONEncoder.

**Rationale**: QMK already uses hjson for loading keyboard configs. However, saving uses `json.dumps()` with QMK's custom encoders, which produces standard JSON. **This means comments will be stripped on save** - this is consistent with `qmk format-json` behavior and expected by the QMK workflow.

**User Impact**: Users should be aware that editing a file with comments will result in those comments being removed upon save. This matches existing QMK tooling behavior.

### 10.4 JSON Formatting
**Decision**: Use QMK's `InfoJSONEncoder` formatting style (4-space indent, sorted keys).

**Rationale**: Ensures consistency with other QMK-generated files, produces clean diffs in PRs, and matches `qmk format-json` output. The entire file may be reformatted on first save.

### 10.5 Category Ordering
**Decision**: Use schema property order with search/filter capability.

**Rationale**: Schema order is self-maintaining (no separate order list to update), and search functionality compensates for any non-intuitive ordering. Categories appear in the same order as defined in `keyboard.jsonschema`.

### 10.6 TUI Framework
**Decision**: Use asciimatics instead of urwid.

**Rationale**: asciimatics provides native Windows support without requiring WSL or Cygwin workarounds. While urwid is more widget-focused and asciimatics is more animation-focused, asciimatics has a `widgets` module with forms, text inputs, and dropdowns that meet our needs. The simpler mental model (frame-based rather than reactive) is also a benefit for long-term maintainability.

**Trade-off**: asciimatics is a new dependency not currently in QMK's requirements.

---

## 11. Appendix

### A. Research Sources

- [QMK Documentation](https://docs.qmk.fm/)
- [QMK GitHub Repository](https://github.com/qmk/qmk_firmware)
- [info.json Reference](https://docs.qmk.fm/reference_info_json)
- [Data Driven Configuration](https://docs.qmk.fm/data_driven_config)
- [asciimatics Documentation](https://asciimatics.readthedocs.io/)
- [Raspberry Pi Configuration](https://www.raspberrypi.com/documentation/computers/configuration.html)
- [Kconfig Language](https://docs.kernel.org/kbuild/kconfig-language.html)

### B. Schema Top-Level Properties (Reference)

The following are the top-level properties in `keyboard.jsonschema` as of the last review. These will appear as fields/categories in the editor in schema definition order:

- `keyboard_name`, `keyboard_folder`, `maintainer`, `manufacturer`, `url`
- `development_board`, `pin_compatible`, `processor`
- `apa102`, `audio`, `backlight`, `battery`, `bluetooth`, `bootmagic`
- `board`, `bootloader`, `bootloader_instructions`, `build`
- `diode_direction`, `debounce`, `caps_word`, `combo`, `community_layouts`
- `dip_switch`, `dynamic_keymap`, `eeprom`, `encoder`, `features`
- `indicators`, `joystick`, `keycodes`, `layer_lock`, `layout_aliases`, `layouts`
- `haptic`, `host`, `leader_key`, `matrix_pins`, `modules`, `mouse_key`
- `oneshot`, `led_matrix`, `rgb_matrix`, `rgblight`, `secure`, `stenography`
- `ps2`, `split`, `tags`, `tapping`, `usb`, `qmk`, `qmk_lufa_bootloader`, `ws2812`

**Note**: This list is for reference only. The actual fields displayed are determined at runtime by reading the schema file. No hardcoded category mapping exists in the editor code.
