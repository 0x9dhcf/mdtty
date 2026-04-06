# Heading level 1 — bold bright magenta

## Heading level 2 — bold bright blue

### Heading level 3 — bold cyan-green

#### Heading level 4 — bold green

##### Heading level 5 — bold yellow

###### Heading level 6 — dim gray, no bold

Each heading above should be bold-colored with a distinct hue, grading from
the brightest (h1 magenta) down to the dimmest (h6 gray). No underline —
color alone differentiates levels.

---

## Inline spans

A paragraph with **bold text**, *italic text*, and `inline code` mixed
together. You can also use __bold with underscores__ and _italic with
underscores_.

Bold is checked before italic, so **this whole phrase is bold** and not two
stray italic markers. Inside **bold you cannot nest** further spans in this
minimal renderer — the second `*` pair closes the first.

Backslash escapes let you emit literal markers: \*not italic\*, \`not code\`,
\**not bold\**, and a literal backslash \\ followed by text.

Inline code spans are opaque: `**not bold inside code**` and `*not italic*`
should appear verbatim in the code color.

An open span at end of line should auto-close so ANSI never leaks across
lines — **this bold never sees a closing marker
and the next line is plain again.

## Blockquotes

> This is a single-line blockquote.

> A blockquote with **bold** and *italic* and `code` inside it. The gutter
> character should appear on every quoted line and the dim-italic style
> should wrap the whole thing.

## Lists

Unordered list, flat:

- first item
- second item with **bold**
- third item with `code`

Unordered list, nested (two-space indent per level):

- level 0 with a bullet •
  - level 1 with a bullet ◦
    - level 2 with a bullet ▸
      - level 3 falls back to the deepest bullet
- back to level 0

Alternate markers work too:

* star bullet
+ plus bullet
- dash bullet

Ordered list:

1. first
2. second with *italic*
3. third with `inline code`
10. double-digit index

## Horizontal rules

Three dashes:

---

Three equals:

===

Three stars with spaces:

* * *

Long dashed rule:

----------------------------------------

## Fenced code blocks

A fenced block with a language tag:

```python
def greet(name: str) -> str:
    # inline markers like **bold** and *italic* must be ignored here
    return f"hello, {name}"
```

A fenced block with no language tag:

```
plain preformatted text
    with preserved    spacing
and `backticks` that are not inline code
```

A fenced block containing what looks like other markdown:

```markdown
# this heading is not rendered as a heading
> this quote is not rendered as a quote
- this list item is not rendered as a list
```

## Edge cases

A line with only spaces below this one should behave like a blank line:
   
Trailing text after fenced block.

A paragraph immediately followed by a heading
## without a blank line in between

A blockquote that contains an escape: > \*literal asterisks\* inside quote.

Escaped hash at line start: \# not a heading.

End of fixture — if you can read this line in a normal paragraph style, the
stream survived every span, block, and chunk boundary along the way.
