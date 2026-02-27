# License Headers

## When Required

All `.c` and `.h` source files must have valid license headers.

## When NOT Required

Simple assignment-only `rules.mk` files typically don't need headers. Logic-heavy `rules.mk` files may require them.

## Recommended Licenses

GPL2 or GPL3 recommended for consistency with QMK. Other GPL-compatible licenses are permitted if they allow redistribution, but different licenses may delay merging. Missing headers prevent merge due to license ambiguity.

## Full GPL2+ Header Template

```c
/* Copyright 2024 Your Name (@yourgithub)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
```

## SPDX Short Form

```c
// Copyright 2024 Your Name (@yourgithub)
// SPDX-License-Identifier: GPL-2.0-or-later
```

## Notes

- Update the year and author fields appropriately.
- The SPDX short form is acceptable and increasingly common.
- Other GPL-compatible licenses are allowed but may delay merging while reviewers verify compatibility.
