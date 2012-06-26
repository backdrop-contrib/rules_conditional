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

  /**
   * Adds a branch element.
   */
  public function addBranch(RulesConditionalBranchPlugin $branch) {
    $this->children[] = $branch;
    $this->resetInternalCache();
  }

  /**
   * Removes a branch element.
   */
  public function removeBranch(RulesConditionalBranchPlugin $branch) {
    if (FALSE !== $key = array_search($branch, $this->children)) {
      unset($this->children[$key]);
      $this->resetInternalCache();
    }
  }

  /**
   * Selects the branches to evaluate for this conditional.
   *
   * @param RulesState $state
   *   Rules state to use.
   * @return array
   *   An array element ids to evaluate.
   */
  abstract public function selectBranches(RulesState $state);
}

/**
 * Base conditional branch plugin implementation, to be contained within a
 * RulesConditionalPlugin.
 */
abstract class RulesConditionalBranchPlugin extends RulesActionContainer implements RulesActionInterface {
  /**
   * The parent conditional.
   * @var RulesConditionalPlugin
   */
  protected $parent = NULL;

  public function setParent(RulesContainerPlugin $parent) {
    if ($this->parent == $parent) {
      return;
    }
    // Remove from current parent.
    if (isset($this->parent)) {
      $this->parent->removeBranch($this);
    }
    // Check parent class against the compatible class.
    if ($parent instanceof $this->getCompatibleConditionalClass()) {
      $this->parent = $parent;
    }
    else {
      throw new RulesEvaluationException('This branch element is incompatible with the given conditional.', array(), $this, RulesLog::ERROR);
    }
  }

  /**
   * Gets the container class for this branch plugin. This must be a subclass of
   * RulesConditionalPlugin.
   */
  public function getCompatibleConditionalClass() {
    return 'RulesConditionalPlugin';
  }

  /**
   * Determines whether this branch can be evaluated.
   */
  abstract public function isEvaluable(RulesState $state);

  /**
   * Evaluates the conditional statement.
   */
  public function evaluate(RulesState $state) {
    // TODO: Implement evaluate() method.
  }

  // TODO Implement sibling lookup methods.
}

/**
 * Default if-else conditional statement.
 */
class RulesConditional extends RulesConditionalPlugin {
  protected $itemName = 'conditional';

  /**
   * Selects the branches to evaluate for this conditional.
   *
   * @param RulesState $state
   *   Rules state to use.
   * @return array
   *   An array element ids to evaluate.
   */
  public function selectBranches(RulesState $state) {
    // TODO: Implement selectBranches() method.
    return array();
  }
}

/**
 * The "if" clause.
 */
class RulesConditionalIf extends RulesConditionalBranchPlugin {
  protected $itemName = 'if';

  public function getCompatibleConditionalClass() {
    return 'RulesConditional';
  }

  /**
   * Determines whether this branch can be evaluated.
   */
  public function isEvaluable(RulesState $state) {
    return $this->evaluateCondition($state);
  }

  /**
   * Evaluates the condition.
   */
  public function evaluateCondition(RulesState $state) {
    // TODO: Implement evaluate() method.
    return FALSE;
  }
}

/**
 * The "else if" clause.
 */
class RulesConditionalElseIf extends RulesConditionalIf {
  protected $itemName = 'else if';
}

/**
 * The "else" clause.
 */
class RulesConditionalElse extends RulesConditionalBranchPlugin {
  protected $itemName = 'else';

  public function getCompatibleConditionalClass() {
    return 'RulesConditional';
  }

  /**
   * Determines whether this branch can be evaluated.
   */
  public function isEvaluable(RulesState $state) {
    return TRUE;
  }
}

/**
 * Switch conditional container.
 */
class RulesConditionalSwitch extends RulesConditionalPlugin {
  protected $itemName = 'switch';

  /**
   * Selects the branches to evaluate for this conditional.
   *
   * @param RulesState $state
   *   Rules state to use.
   * @return array
   *   An array element ids to evaluate.
   */
  public function selectBranches(RulesState $state) {
    // TODO: Implement selectBranches() method.
    return array();
  }
}

/**
 * Switch case.
 */
class RulesConditionalCase extends RulesConditionalBranchPlugin {
  protected $itemName = 'case';

  public function getCompatibleConditionalClass() {
    return 'RulesConditionalSwitch';
  }

  /**
   * Determines whether this branch can be evaluated.
   */
  public function isEvaluable(RulesState $state) {
    // TODO: Implement isEvaluable() method.
  }
}

/**
 * Switch default case.
 */
class RulesConditionalDefaultCase extends RulesConditionalCase {
  protected $itemName = 'default case';

  /**
   * Determines whether this branch can be evaluated.
   */
  public function isEvaluable(RulesState $state) {
    return TRUE;
  }
}
