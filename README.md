# Conditional Rules

This module provides Rules plugins for conditionally altering execution flow
directly inside a group of actions. Provided plugins include:

- Conditional
  - If/(Else) If
  - Else
- Switch
  - Case
  - Default case
- While

Using these plugins, rules with relatively easy-to-read flows can be built without
creating numerous trivial components (with conditions) merely for branching actions.
This module also provides a condition set (rule), which evaluates as a condition
based on results from a set of actions. This is useful if a condition set
requires the results from a rule component.

Note that for elements with a condition (i.e. "if" and "while"), only a simple
condition can be configured. It is recommended that complex conditions be
constructed as a condition set and then used in the condition elements.

## Dependencies

- Rules

## Installation

- Install this module using the
  [official Backdrop CMS instructions](https://docs.backdropcms.org/documentation/extend-with-modules)

## Issues

Bugs and feature requests should be reported in the
[issue queue](https://github.com/backdrop-contrib/conditional_rules/issues).

## Maintainers

- [indigoxela](https://github.com/indigoxela)
- Seeking additional maintainers

## Credits

Drupal maintainers:

* [Alan Lobo (alansaviolobo)](https://www.drupal.org/u/alansaviolobo)
* [Jason Zhang (zhangtaihao)](https://www.drupal.org/u/zhangtaihao)

## License

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
