# Plan: Split keyboard-editor-tui into Reviewable Commit Stack

## Context

The `json-tool` branch has a single commit (`310c9e0`) containing the entire keyboard editor TUI implementation (~1,668 code lines + 192 lines of tests). This makes review difficult. We will create a new branch `keyboard-editor-tui` from `master` and cherry-pick the code in 5 smaller, layered commits using Strategy A (architectural layer, bottom-up).

**Excluded from migration:** `.claude/`, `CLAUDE.md`, `lib/python/qmk/cli/edit/docs/`

## Branch Setup

```bash
git checkout master
git checkout -b keyboard-editor-tui
```

## Commit Plan

### Commit 1: "Add data layer: state, schema, file I/O, and config" (~405 code lines)

**Files to add:**
- `requirements.txt` (1 line change — add `hjson`)
- `lib/python/qmk/cli/edit/__init__.py` — **empty `__init__.py` only** (just make it a package, no CLI wiring yet)
- `lib/python/qmk/cli/edit/config.py` (3 code lines)
- `lib/python/qmk/cli/edit/constraint_formatter.py` (38 code lines)
- `lib/python/qmk/cli/edit/state.py` (154 code lines)
- `lib/python/qmk/cli/edit/schema_loader.py` (106 code lines)
- `lib/python/qmk/cli/edit/file_manager.py` (56 code lines)
- `lib/python/qmk/cli/edit/keymap.py` (49 code lines)

**What this does NOT include:** `cli/__init__.py` change (the subcommand registration), the full `edit/__init__.py` with CLI decorators. Those come in commit 4 when the CLI entry point is wired up.

**For `edit/__init__.py` in this commit:** Create a minimal file that just makes the directory a Python package:
```python
"""Keyboard editor TUI — data and schema layer."""
```

**Commit message:**
```
edit: add data layer for keyboard editor TUI

Introduce pure-logic modules for the keyboard editor:
- EditorState: navigation, get/set, modification tracking
- SchemaLoader: JSON Schema traversal with $ref resolution
- FileManager: load/save/validate with hjson support
- EditorConfig: CLI flag configuration dataclass
- constraint_formatter: hint text generation from schema constraints
- keymap: key binding definitions

These modules have no UI dependencies and form the foundation
for the editor's data operations.
```

**Reviewer will see:** Pure Python data classes and logic. No asciimatics imports. They can evaluate state management, schema resolution, and validation independently. Questions they might ask: "How is EditorState consumed?" — answer: by the widget/view layer in commits 2-4.

---

### Commit 2: "Add widget system: base class, field widgets, and registry" (~284 code lines)

**Files to add:**
- `lib/python/qmk/cli/edit/widgets/__init__.py` (33 code lines)
- `lib/python/qmk/cli/edit/widgets/base.py` (42 code lines)
- `lib/python/qmk/cli/edit/widgets/text_field.py` (14 code lines)
- `lib/python/qmk/cli/edit/widgets/numeric_field.py` (67 code lines)
- `lib/python/qmk/cli/edit/widgets/boolean_field.py` (35 code lines)
- `lib/python/qmk/cli/edit/widgets/enum_field.py` (22 code lines)
- `lib/python/qmk/cli/edit/widgets/object_row.py` (19 code lines)
- `lib/python/qmk/cli/edit/widgets/array_row.py` (26 code lines)
- `lib/python/qmk/cli/edit/widgets/array_item_row.py` (65 code lines — uses TextField, NumericField from same commit)
- `lib/python/qmk/cli/edit/widget_registry.py` (23 code lines — uses widgets.text_field)

**Internal dependency note:** `base.py` imports `constraint_formatter` from commit 1. `array_item_row.py` imports `TextField` and `NumericField` from this same commit. `widget_registry.py` imports `TextField`. All dependencies satisfied.

**Commit message:**
```
edit: add widget system for keyboard editor TUI

Introduce the widget abstraction layer:
- FieldWidget base class with schema constraint rendering
- Concrete widgets: TextField, NumericField, BooleanField, EnumField
- Navigation widgets: ObjectRow, ArrayRow, ArrayItemRow
- WidgetRegistry for type-based widget creation
- create_default_registry() factory

Each widget wraps an asciimatics widget with schema-aware
validation and display hints.
```

**Reviewer will see:** The widget abstraction pattern — a base class, concrete implementations, and a factory. This is a standard pattern so the review should be quick. Reviewer can evaluate: Does the base class contract make sense? Are the widget types complete? Is the registry flexible enough?

---

### Commit 3: "Add view helpers: controllers, focus, search, and status" (~397 code lines)

**Files to add:**
- `lib/python/qmk/cli/edit/views/__init__.py` — **partial version** (only re-exports for the helpers being added in this commit, NOT field_list/dialogs/ui_builder yet)
- `lib/python/qmk/cli/edit/views/view_context.py` (7 code lines)
- `lib/python/qmk/cli/edit/views/view_callbacks.py` (10 code lines)
- `lib/python/qmk/cli/edit/views/item_scaffolder.py` (21 code lines)
- `lib/python/qmk/cli/edit/views/frame_helper.py` (31 code lines)
- `lib/python/qmk/cli/edit/views/status_display.py` (71 code lines)
- `lib/python/qmk/cli/edit/views/focus_manager.py` (54 code lines)
- `lib/python/qmk/cli/edit/views/search_controller.py` (44 code lines)
- `lib/python/qmk/cli/edit/views/array_controller.py` (56 code lines)
- `lib/python/qmk/cli/edit/views/event_router.py` (73 code lines)

**For `views/__init__.py` in this commit:** Create a version that only imports/re-exports the modules from this commit. In commit 4, we'll replace it with the full version that also exports field_list, dialogs, etc.

**Internal dependency note:** `focus_manager.py` imports from `widgets` (commit 2) and `frame_helper` (this commit). `event_router.py` has no internal deps. All others are standalone. All dependencies satisfied.

**Commit message:**
```
edit: add view controllers and helpers for keyboard editor TUI

Introduce view-layer helper modules:
- ViewContext/ViewCallbacks: data bags for dependency injection
- EventRouter: keyboard event dispatch logic
- FocusManager: widget focus tracking and field identification
- SearchController: search activation/deactivation state machine
- ArrayController: array item add/delete/move operations
- StatusDisplay: timed status message management
- ItemScaffolder: default value generation from schema types
- FrameHelper: asciimatics Frame query abstraction

These modules encapsulate reusable view logic consumed by
the main FieldListView in the next commit.
```

**Reviewer will see:** Small, focused helper classes. Most are under 50 code lines. Reviewer can evaluate each in isolation. Questions they might ask: "How do these plug together?" — answer: FieldListView in commit 4 composes them all.

---

### Commit 4: "Add main views, dialogs, and CLI entry point" (~400 code lines)

**Files to add / modify:**
- `lib/python/qmk/cli/edit/views/field_list.py` (83 code lines)
- `lib/python/qmk/cli/edit/views/ui_builder.py` (133 code lines)
- `lib/python/qmk/cli/edit/views/error_dialog.py` (35 code lines)
- `lib/python/qmk/cli/edit/views/confirm_dialog.py` (43 code lines)
- `lib/python/qmk/cli/edit/views/delete_confirm_dialog.py` (35 code lines)
- `lib/python/qmk/cli/edit/views/help_dialog.py` (53 code lines)
- `lib/python/qmk/cli/edit/editor.py` (155 code lines)
- `docs/cli_commands.md` — add `qmk edit` section (between `qmk doctor` and `qmk format-json`)

**Files to REPLACE (update from earlier commits):**
- `lib/python/qmk/cli/edit/views/__init__.py` — replace with the **full version** that re-exports all views (field_list, dialogs, ui_builder, plus all helpers from commit 3)
- `lib/python/qmk/cli/edit/__init__.py` — replace with the **full version** containing the `@cli.argument` / `@cli.subcommand` decorators and `edit()` function

**Files to modify (1-line change):**
- `lib/python/qmk/cli/__init__.py` — add the `'edit'` subcommand import

**Commit message:**
```
edit: add views, dialogs, editor, and CLI entry point

Complete the keyboard editor TUI implementation:
- FieldListView: main editor screen composing all view helpers
- UIBuilder: asciimatics layout construction for field rendering
- Dialog overlays: ErrorDialog, ConfirmDialog, DeleteConfirmDialog, HelpDialog
- EditorController: top-level orchestration (load → edit → save loop)
- CLI entry point: register `qmk edit` subcommand
- Documentation: add `qmk edit` section to docs/cli_commands.md

The editor is now functional via `qmk edit <keyboard>`.
```

**Reviewer will see:** The complete integration layer — how everything from commits 1-3 connects. This is the most complex commit but the reviewer already understands all the pieces. They can focus on: Is the composition correct? Are the dialogs consistent? Does the CLI entry point follow QMK conventions?

---

### Commit 5: "Add integration tests for keyboard editor" (~192 code lines)

**Files to add:**
- `lib/python/qmk/tests/test_edit.py` (192 code lines)

**Commit message:**
```
edit: add integration tests for keyboard editor TUI

Add headless integration tests using MagicMock(spec=Screen) + Canvas
to exercise FieldListView without a real terminal. Tests cover:
- Schema loading and field rendering
- Navigation into nested objects and arrays
- Field editing and modification tracking
- Array operations (add, delete, move)
- Search functionality
- Dialog display and interaction
- Save with validation
```

**Reviewer will see:** Tests that exercise the full stack. By this point the reviewer understands the architecture, so they can evaluate test coverage and correctness. This commit stands alone cleanly since tests are in a separate directory.

---

## Execution Steps (for the engineer)

### Step 1: Prepare
```bash
# Start from the json-tool branch (which has all the code)
git checkout json-tool

# Create the new branch from master
git checkout master
git checkout -b keyboard-editor-tui
```

### Step 2: Build each commit
For each commit (1 through 5), repeat this pattern:

```bash
# Copy files from json-tool into the working tree
git checkout json-tool -- <file1> <file2> ...

# For files that need a different version (like __init__.py stubs),
# edit them manually after checkout

# Stage and commit
git add <file1> <file2> ...
git commit -m "<commit message from above>"
```

**Specific per-commit instructions:**

#### Commit 1
```bash
# Copy the data layer files from json-tool
git checkout json-tool -- requirements.txt
git checkout json-tool -- lib/python/qmk/cli/edit/config.py
git checkout json-tool -- lib/python/qmk/cli/edit/constraint_formatter.py
git checkout json-tool -- lib/python/qmk/cli/edit/state.py
git checkout json-tool -- lib/python/qmk/cli/edit/schema_loader.py
git checkout json-tool -- lib/python/qmk/cli/edit/file_manager.py
git checkout json-tool -- lib/python/qmk/cli/edit/keymap.py

# Create a STUB __init__.py (not the full CLI entry point)
mkdir -p lib/python/qmk/cli/edit
echo '"""Keyboard editor TUI — data and schema layer."""' > lib/python/qmk/cli/edit/__init__.py

# Stage and commit
git add requirements.txt lib/python/qmk/cli/edit/
git commit  # use commit message from plan
```

#### Commit 2
```bash
# Copy widget files from json-tool
git checkout json-tool -- lib/python/qmk/cli/edit/widgets/
git checkout json-tool -- lib/python/qmk/cli/edit/widget_registry.py

# Stage and commit
git add lib/python/qmk/cli/edit/widgets/ lib/python/qmk/cli/edit/widget_registry.py
git commit  # use commit message from plan
```

#### Commit 3
```bash
# Copy view helper files from json-tool
git checkout json-tool -- lib/python/qmk/cli/edit/views/view_context.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/view_callbacks.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/item_scaffolder.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/frame_helper.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/status_display.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/focus_manager.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/search_controller.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/array_controller.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/event_router.py

# Create a PARTIAL views/__init__.py that only exports these helpers
# (NOT the full version — no field_list, dialogs, or ui_builder yet)
# Write a version that imports only the modules from this commit.
# Check the full __init__.py on json-tool for the exact import lines,
# then include only the imports for the 9 files above.

# Stage and commit
git add lib/python/qmk/cli/edit/views/
git commit  # use commit message from plan
```

**Important detail for `views/__init__.py`:** Do NOT use the version from `json-tool`. Instead, manually create this subset version:

```python
"""Views package for TUI screens.
"""
from qmk.cli.edit.views.view_context import ViewContext
from qmk.cli.edit.views.view_callbacks import ViewCallbacks
from qmk.cli.edit.views.item_scaffolder import ItemScaffolder
from qmk.cli.edit.views.search_controller import SearchController
from qmk.cli.edit.views.status_display import StatusDisplay
from qmk.cli.edit.views.focus_manager import FocusManager
from qmk.cli.edit.views.array_controller import ArrayController
from qmk.cli.edit.views.event_router import EventRouter
from qmk.cli.edit.views.frame_helper import FrameHelper

__all__ = [
    'ViewContext',
    'ViewCallbacks',
    'ItemScaffolder',
    'SearchController',
    'StatusDisplay',
    'FocusManager',
    'ArrayController',
    'EventRouter',
    'FrameHelper',
]
```

In commit 4, this file gets replaced with the full version via `git checkout json-tool -- lib/python/qmk/cli/edit/views/__init__.py`.

#### Commit 4
```bash
# Copy remaining view files and editor from json-tool
git checkout json-tool -- lib/python/qmk/cli/edit/views/field_list.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/ui_builder.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/error_dialog.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/confirm_dialog.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/delete_confirm_dialog.py
git checkout json-tool -- lib/python/qmk/cli/edit/views/help_dialog.py
git checkout json-tool -- lib/python/qmk/cli/edit/editor.py

# Replace __init__ files with FULL versions from json-tool
git checkout json-tool -- lib/python/qmk/cli/edit/views/__init__.py
git checkout json-tool -- lib/python/qmk/cli/edit/__init__.py

# Add the subcommand registration
git checkout json-tool -- lib/python/qmk/cli/__init__.py

# Add docs (insert `qmk edit` section into docs/cli_commands.md
# between `qmk doctor` and `qmk format-json`)
git checkout json-tool -- docs/cli_commands.md

# Stage and commit
git add lib/python/qmk/cli/ docs/cli_commands.md
git commit  # use commit message from plan
```

#### Commit 5
```bash
# Copy test file from json-tool
git checkout json-tool -- lib/python/qmk/tests/test_edit.py

# Stage and commit
git add lib/python/qmk/tests/test_edit.py
git commit  # use commit message from plan
```

### Step 3: Verify
```bash
# Confirm 5 commits on the branch
git log --oneline master..keyboard-editor-tui

# Verify final state matches json-tool (excluding excluded files)
# The diff should only show .claude/, CLAUDE.md, and docs/ differences
git diff keyboard-editor-tui..json-tool -- lib/python/qmk/cli/edit/ lib/python/qmk/tests/test_edit.py requirements.txt lib/python/qmk/cli/__init__.py docs/cli_commands.md
```

Expected: The only differences should be `lib/python/qmk/cli/edit/docs/` (excluded from migration).

## Verification

After execution, run:
```bash
# 1. Confirm commit count
git log --oneline master..keyboard-editor-tui  # should show 5 commits

# 2. Confirm no missing files (compare to json-tool, excluding docs/.claude/CLAUDE.md)
git diff --name-only keyboard-editor-tui..json-tool -- \
  lib/python/qmk/cli/edit/ \
  lib/python/qmk/tests/test_edit.py \
  requirements.txt \
  lib/python/qmk/cli/__init__.py \
  docs/cli_commands.md \
  ':!lib/python/qmk/cli/edit/docs'
# Should be empty

# 3. Run tests to confirm everything works
.venv/bin/python -m nose2 -v -s lib/python/qmk/tests test_edit

# 4. Verify line counts per commit
for sha in $(git log --reverse --format=%H master..keyboard-editor-tui); do
  echo "--- $(git log --oneline -1 $sha) ---"
  git diff --numstat ${sha}~1..${sha} | awk '{sum+=$1} END {print "diff lines:", sum}'
done
```
