# Plan: Consolidate Unit Tests into Integration Tests

## Background

The previous commit (`b7a8519`) introduced `test_edit.py` with 8 integration tests
using the headless Canvas pattern. The current commit (`847be5e`) added bug fixes to
the source code and introduced 7 new unit test files that test internal/private
methods with mock objects.

**Goal:** Delete the 7 unit test files and add 2 new integration tests to
`test_edit.py` that cover the user-facing behaviors changed in this commit.

---

## Task 1: Delete the 7 unit test files

Delete these files from `lib/python/qmk/tests/`:

```
test_edit_array_item_row.py
test_edit_constraint_formatter.py
test_edit_field_widget_properties.py
test_edit_numeric_field.py
test_edit_schema_loader.py
test_edit_state.py
test_edit_status_display.py
```

**How to verify:** Run `ls lib/python/qmk/tests/test_edit*.py` — only `test_edit.py`
should remain.

---

## Task 2: Add `test_array_move_item` to `test_edit.py`

This tests the array move-up/move-down feature with focus tracking, added in
`array_controller.py` in this commit.

### What the test does

1. Creates an array with 3 items: `['alpha', 'beta', 'gamma']`
2. Navigates into the array
3. Tabs to focus item at index 1 (`beta`)
4. Presses `J` (move down) — verifies the array becomes `['alpha', 'gamma', 'beta']`
5. Presses `K` (move up) — verifies the array returns to `['alpha', 'beta', 'gamma']`
6. Presses `K` again at index 0 — verifies the array is unchanged (boundary no-op)

### Where to add it

Insert after `test_array_delete_item` (after line 219) and before the
`make_controller_view` helper (line 222).

### Code to add

```python
def test_array_move_item():
    """Move-up and move-down reorder items; boundary moves are no-ops."""
    data = copy.deepcopy(_DEFAULT_DATA)
    data['community_layouts'] = ['alpha', 'beta', 'gamma']

    view, state, canvas, scene = _make_array_test_view(data, 'community_layouts')

    # Tab twice to focus item at index 1 (beta).
    process_keys(view, [Screen.KEY_TAB, Screen.KEY_TAB])

    # Move down: beta swaps with gamma.
    process_keys(view, [ord('J')])
    assert state.working_data['community_layouts'] == ['alpha', 'gamma', 'beta']
    assert state.is_modified is True

    # Move up: beta swaps back with gamma.
    process_keys(view, [ord('K')])
    assert state.working_data['community_layouts'] == ['alpha', 'beta', 'gamma']

    # Move up again: beta is now at index 0 after the swap above,
    # so move up from index 1 puts it at 0; one more move up is a no-op.
    process_keys(view, [ord('K')])
    assert state.working_data['community_layouts'] == ['beta', 'alpha', 'gamma']

    process_keys(view, [ord('K')])
    assert state.working_data['community_layouts'] == ['beta', 'alpha', 'gamma']
```

### Key details

- `ord('J')` triggers the `move_down` keybinding (uppercase J = 74).
- `ord('K')` triggers the `move_up` keybinding (uppercase K = 75).
- These keybindings are defined in `lib/python/qmk/cli/edit/keymap.py` lines 37-38.
- Move at boundaries (first item up, last item down) silently no-ops — see
  `array_controller.py` `move_item_up` (returns early when `focused_index == 0`)
  and `move_item_down` (returns early when `focused_index >= array_length - 1`).
- Focus tracking after moves was added in this commit: after each move, the
  controller calls `focus_on_array_index()` to keep focus on the moved item.

### Debugging tips

If the test fails, the most likely cause is focus not landing on the expected
array item. The `_make_array_test_view` helper starts with no widget focused.
The first `TAB` focuses item 0, the second focuses item 1. After a move
operation, `focus_on_array_index()` moves focus to the destination index
automatically, so subsequent move keys operate on the same logical item.

To inspect focus during debugging, add:
```python
print('focused index:', view._focus_mgr.focused_array_index())
print('array state:', state.working_data['community_layouts'])
```

---

## Task 3: Add `test_edit_numeric_array_item` to `test_edit.py`

This tests the ArrayItemRow numeric delegation feature — array items with integer
schema now delegate to `NumericField` for type coercion and validation, instead of
using a raw `Text` widget with a string-only `_on_change`.

### What the test does

1. Creates data with a nested integer array: `bootmagic.matrix = [0, 1]`
2. Navigates two levels deep: first into `bootmagic`, then into `matrix`
3. Tabs to focus the first array item
4. Clears the field and types `42`
5. Verifies state stores the value as an integer `42` (not string `'42'`)
6. Types invalid input and verifies state is not corrupted

### Where to add it

Insert immediately after `test_array_move_item` (from Task 2), still before the
`make_controller_view` helper.

### Why `_make_array_test_view` can't be used directly

The existing `_make_array_test_view(data, array_path)` helper calls
`state.navigate_into(array_path)` with a single string key. For `bootmagic.matrix`
we need to navigate two levels: into `bootmagic` (an object), then into `matrix`
(the array). Rather than modify the shared helper, the test inlines the setup
with two `navigate_into` calls.

### Code to add

```python
def test_edit_numeric_array_item():
    """Typing a number into an integer array item stores an int in state."""
    data = copy.deepcopy(_DEFAULT_DATA)
    data['bootmagic'] = {'matrix': [0, 1]}

    screen = MagicMock(spec=Screen)
    screen.height = 30
    screen.width = 100
    screen.colours = 8
    screen.unicode_aware = False
    canvas = Canvas(screen, 30, 100, 0, 0)

    schema_loader = SchemaLoader()
    state = EditorState(data, schema_loader)
    registry = create_default_registry()

    state.navigate_into('bootmagic')
    state.navigate_into('matrix')

    view = FieldListView(canvas, state, 'handwired/pytest/basic', registry)
    scene = Scene([view], -1)
    view.reset()

    # Tab to focus first array item (index 0, value=0).
    process_keys(view, [Screen.KEY_TAB])

    # Select all existing text and replace with '42'.
    # Ctrl+A selects all, then typing replaces.
    process_keys(view, [Screen.ctrl('a'), '42'])

    assert state.working_data['bootmagic']['matrix'][0] == 42
    assert isinstance(state.working_data['bootmagic']['matrix'][0], int)
    assert state.is_modified is True

    # Type non-numeric input: value should revert to 42 (not corrupted).
    process_keys(view, [Screen.ctrl('a'), 'abc'])
    assert state.working_data['bootmagic']['matrix'][0] == 42
```

### Key details

- `bootmagic.matrix` is defined in the QMK keyboard schema as a 2-element integer
  array with `minimum: 0`. Schema location: the root `bootmagic` property contains
  a `matrix` property of type `array` with `items: {type: integer, minimum: 0}`.
- The `NumericField._on_change` method (in `widgets/numeric_field.py`) parses the
  text input via `_parse_value()`. If parsing fails (returns `None`) or validation
  fails, it reverts the widget display to the last valid value and does **not**
  update state. The `_updating` reentrancy guard (added in this commit) prevents
  the revert from triggering another `_on_change` cycle.
- `Screen.ctrl('a')` sends Ctrl+A to select all text in the asciimatics Text widget,
  allowing the next typed characters to replace the existing value.

### Debugging tips

If the integer assertion fails (value is a string instead of int), the delegation
from `ArrayItemRow` to `NumericField` isn't working. Check that `ArrayItemRow.
as_asciimatics_widget()` creates a `NumericField` delegate for integer schema types
(see `_create_delegate()` in `widgets/array_item_row.py`).

If the invalid input test fails (state is corrupted with `None` or `'abc'`), the
`NumericField._on_change` revert logic isn't firing. Check that `_parse_value('abc')`
returns `None` and that the `else` branch restores the widget value.

---

## Task 4: Run tests and verify

Run the full edit test suite and linter:

```bash
.venv/bin/python -m nose2 -v -s lib/python qmk.tests.test_edit 2>&1 | tail -5
```

**Expected:** `Ran 10 tests in ...` / `OK` (8 existing + 2 new).

Then run flake8:

```bash
.venv/bin/python -m flake8 lib/python/qmk/tests/test_edit.py
```

**Expected:** No output (no lint errors).

### If tests fail

- **ImportError:** Make sure no imports were removed that are still needed. The
  existing import block should not need changes.
- **AssertionError on array order:** Focus may not be on the expected item. Add
  print statements to check `view._focus_mgr.focused_array_index()`.
- **AssertionError on integer type:** The `NumericField` delegate may not be created.
  Check that `bootmagic.matrix` items schema resolves to `{type: integer}`.
- **Flake8 errors:** Ensure blank lines between test functions (2 blank lines
  between top-level definitions), and lines are under 120 characters.

---

## Summary

| Task | Action | Files |
|------|--------|-------|
| 1 | Delete 7 unit test files | `lib/python/qmk/tests/test_edit_*.py` |
| 2 | Add `test_array_move_item` | `lib/python/qmk/tests/test_edit.py` |
| 3 | Add `test_edit_numeric_array_item` | `lib/python/qmk/tests/test_edit.py` |
| 4 | Run tests + linter | Terminal |
