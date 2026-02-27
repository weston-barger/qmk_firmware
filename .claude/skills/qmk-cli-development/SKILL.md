---
name: qmk-cli-development
description: QMK CLI subcommand development guide. Use when creating, modifying, or reviewing files in `lib/python/qmk/cli/`. Covers MILC framework usage, argument handling, configuration, logging, and testing for QMK CLI subcommands.
---

# QMK CLI Development

QMK CLI uses a subcommand architecture built on [MILC](https://github.com/clueboard/milc). Subcommands reside in `lib/python/qmk/cli/`.

## Subcommand Structure

```python
"""QMK Python Hello World

This is an example QMK CLI script.
"""
from milc import cli


@cli.argument('-n', '--name', default='World', help='Name to greet.')
@cli.subcommand('QMK Hello World.')
def hello(cli):
    """Log a friendly greeting.
    """
    cli.log.info('Hello, %s!', cli.config.hello.name)
```

Key points:
- `@cli.argument()` defines flags and creates config variables
- `@cli.subcommand()` registers the function as a subcommand
- Subcommand name derives from function name
- Return shell exit code or None

## User Interaction

### Output Methods

| Method | Purpose |
|--------|---------|
| `cli.log.info()` | General-purpose printing (user-controllable) |
| `cli.log.debug()` | Debug output (shown with `qmk -v`) |
| `cli.log.warning()` | Warnings |
| `cli.log.error()` | Errors |
| `cli.echo()` | Fixed output outside logging system |

Use printf-style formatting with arguments:
```python
cli.log.info('Hello, %s!', name)  # Good
cli.log.info('Hello, %s!' % name)  # Bad
```

### Log Level Emojis

| Level | Emoji |
|-------|-------|
| critical | `{bg_red}{fg_white}¬_¬{style_reset_all}` |
| error | `{fg_red}☒{style_reset_all}` |
| warning | `{fg_yellow}⚠{style_reset_all}` |
| info | `{fg_blue}Ψ{style_reset_all}` |
| debug | `{fg_cyan}☐{style_reset_all}` |

### Color Tokens

Insert in strings: `{fg_red}`, `{bg_blue}`, `{style_bright}`, `{style_reset_all}`

Colors: Black, Blue, Cyan, Green, Magenta, Red, White, Yellow

## Arguments and Configuration

Arguments automatically populate `cli.config.<subcommand>.<argument>`.

Priority: command-line > config file > decorator default

### Reading Config

```python
# Attribute style
value = cli.config.hello.name

# Dictionary style
value = cli.config['hello']['name']

# Iteration
for section in cli.config:
    for key in cli.config[section]:
        cli.log.info('%s.%s: %s', section, key, cli.config[section][key])
```

### Setting Config

```python
cli.config.section.key = value
# or
cli.config['section']['key'] = value
```

### Arguments Not Saved to Config

Use `arg_only=True` for transient arguments:

```python
@cli.argument('-o', '--output', arg_only=True, help='File to write to')
@cli.argument('filename', arg_only=True, help='Configurator JSON file')
@cli.subcommand('Create a keymap.c from a QMK Configurator export.')
def json_keymap(cli):
    cli.log.info('Reading from %s and writing to %s', cli.args.filename, cli.args.output)
```

Access via `cli.args` instead of `cli.config`.

## Testing and Formatting

### Commands

```bash
qmk pytest          # Run tests and linting
qmk format-python   # Auto-format code
```

### Test Location

Tests in `lib/python/qmk/tests/`. Uses [nose2](https://nose2.readthedocs.io/).

### Incomplete Test Coverage

Add TODO comment:
```python
# TODO(unassigned/<your_github_username>): Write <unit|integration> tests
```

### Tools

- **yapf**: Auto-formatting (config in `setup.cfg`)
- **flake8**: Linting (must pass before PR)

## Developer Mode

Enable to access all subcommands:

```bash
qmk config user.developer=True
python3 -m pip install -r requirements-dev.txt
```
