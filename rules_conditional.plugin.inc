<?php
/**
 * @file
 * Rules plugin implementation.
 */

/**
 * Embedded rule.
 */
class RulesConditionalIf extends RulesActionContainer {
  protected $itemName = 'if';
}

/**
 * Embedded rule.
 */
class RulesConditionalElseIf extends RulesConditionalIf {
  protected $itemName = 'else if';
}

/**
 * Embedded rule.
 */
class RulesConditionalElse extends RulesActionContainer {
  protected $itemName = 'else';
}

/**
 * Embedded rule.
 */
class RulesConditionalSwitch extends Rule {
  protected $itemName = 'switch';
}

/**
 * Embedded rule.
 */
class RulesConditionalCase extends Rule {
  protected $itemName = 'case';
}
