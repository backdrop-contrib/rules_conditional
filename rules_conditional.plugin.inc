<?php
/**
 * @file
 * Rules plugin implementation.
 */

/**
 * Base conditional statement plugin implementation.
 */
abstract class RulesConditionalPlugin extends RulesContainerPlugin {
  /**
   * Evaluates the conditional statement.
   */
  public function evaluate(RulesState $state) {
    // TODO: Implement evaluate() method.
  }
}

/**
 * Base conditional branch plugin implementation, to be contained within a
 * RulesConditionalPlugin.
 */
abstract class RulesConditionalBranchPlugin extends RulesPlugin {
  /**
   * Evaluates the conditional statement.
   */
  public function evaluate(RulesState $state) {
    // TODO: Implement evaluate() method.
  }

  /**
   * Execute the configuration by passing arguments in a single array.
   */
  public function executeByArgs($args = array()) {
    // TODO: Implement executeByArgs() method.
  }

  /**
   * Evaluates the branch condition to determine an execution state.
   */
  abstract public function evaluateCondition(RulesState $state);
}

/**
 * Default if-else conditional statement.
 */
class RulesConditional extends RulesConditionalPlugin {
  // TODO
}

/**
 * Embedded rule.
 */
class RulesConditionalIf extends RulesConditionalBranchPlugin {
  protected $itemName = 'if';

  /**
   * Creates an instance of the 'if' element.
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Evaluates the condition.
   */
  public function evaluateCondition(RulesState $state) {
    // TODO: Implement evaluate() method.
  }
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
