---
name: qmk-coding-conventions
description: QMK Python coding conventions and style guidelines. Use when writing, reviewing, or modifying Python code in `lib/python/qmk/` or any QMK Python modules. Ensures code follows QMK's PEP8-based standards with project-specific modifications.
---

# QMK Python Coding Conventions

Follow PEP8 with these QMK-specific modifications. Target Python 3.9. Use YAPF for formatting (config in `setup.cfg`).

## Key Principles

- Four spaces for indentation (soft tabs)
- Liberal comments explaining decisions and features
- Useful docstrings for all functions
- Lines can be long; wrap at 76 columns if desired
- Prioritize approachability for non-Python developers
- No type annotations (may change in future)
- Avoid threading/multiprocessing without strong justification

## Imports

- One module per line
- Group: system, third-party, local
- Never use `from foo import *`
- Avoid `as` keyword except for compatibility

```python
# Good - specific import
from qmk.keymap import compile_firmware
compile_firmware()

# Good - module import when names might be ambiguous
from qmk import effects
effects.echo()
```

## Naming

| Type | Convention |
|------|------------|
| Modules/packages | `snake_case` |
| Classes/Exceptions | `PascalCase` |
| Functions/methods/variables | `snake_case` |
| Constants | `UPPER_SNAKE_CASE` |

Avoid: single chars (except `i`, `j`, `e`), dashes in module names, dunder names.

## Docstrings

Use markdown formatting. Always triple-quote with line break after opening:

```python
def simple_function():
    """Return the current timestamp.
    """
    return int(time.time())

def complex_function(start=None, offset=0):
    """Return seconds since epoch with optional adjustments.

    Extended description if needed.

    Args:
        start
            Override the start time

        offset
            Seconds to subtract from result

    Returns:
        An integer timestamp.

    Raises:
        ValueError
            When inputs are not positive numbers
    """
```

## Format Strings

Prefer printf-style for consistency with logging and C familiarity:

```python
name = 'World'
print('Hello, %s!' % (name,))
```

## Common Patterns

**Default arguments** - Use immutable defaults:
```python
# Bad
def my_func(foo={}):
    pass

# Good
def my_func(foo=None):
    if not foo:
        foo = {}
```

**Boolean evaluation** - Use implicit:
```python
# Bad
if foo == True:
if bar == False:

# Good
if foo:
if not bar:
```

**Properties** - Use instead of getters/setters:
```python
class Foo(object):
    def __init__(self):
        self._bar = None

    @property
    def bar(self):
        return self._bar

    @bar.setter
    def bar(self, bar):
        self._bar = bar
```

**Tuples** - Include trailing comma in single-item tuples:
```python
single = (item,)
```

## Avoid

- Explicit True/False comparisons
- Mutable default arguments
- Threading/multiprocessing
- Custom metaclasses, bytecode manipulation, dynamic inheritance
- Import hacks, reflection, system introspection
- Functions exceeding ~40 lines
- Overly complex comprehensions (use for loops instead)

## Exceptions

- Use for exceptional situations, not flow control
- Catch-all exceptions require logging via `cli.log`
- Keep try/except blocks minimal

## FIXMEs

Format with GitHub username:
```
FIXME(username): Revisit when the frob feature is done.
```

## Testing

Tests in `lib/python/qmk/tests/`. Run with `qmk pytest`.

- **Integration tests**: `test_cli_commands.py` - verifies CLI behavior via subprocess
- **Unit tests**: `test_*.py` files for individual functions (no mocking currently)
