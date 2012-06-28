<?php
/**
 * @file
 * Rules plugin implementation.
 */

/**
 * Base conditional statement plugin implementation.
 */
abstract class RulesConditionalPlugin extends RulesContainerPlugin implements RulesActionInterface {
  public function label() {
    $info = $this->pluginInfo();
    $label = isset($info['label']) ? $info['label'] : t('unlabeled');
    return $label;
  }

  /**
   * Evaluates the conditional statement.
   */
  public function evaluate(RulesState $state) {
    // Evaluate selected branches.
    $branches = $this->selectBranches($state);
    foreach ($branches as $branch) {
      $branch->evaluate($state);
    }
  }

  /**
   * Asserts no variables (since a conditional is *conditionally* evaluated).
   */
  protected function variableInfoAssertions() {
    return array();
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
   * @return RulesConditionalBranchPlugin[]
   *   An array of branches to evaluate.
   */
  abstract protected function selectBranches(RulesState $state);
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

  public function label() {
    $info = $this->pluginInfo();
    $label = isset($info['label']) ? $info['label'] : t('unlabeled');
    return $label;
  }

  public function setParent(RulesContainerPlugin $parent) {
    if ($this->parent == $parent) {
      return;
    }
    // Remove from current parent.
    if (isset($this->parent)) {
      $this->parent->removeBranch($this);
    }
    // Check parent class against the compatible class.
    $compatibleClass = $this->getCompatibleConditionalClass();
    if ($parent instanceof $compatibleClass) {
      $this->parent = $parent;
      $this->parent->addBranch($this);
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
  abstract public function canEvaluate(RulesState $state);

  /**
   * Gets the previous sibling element.
   *
   * @return RulesPlugin
   */
  public function getPreviousSibling() {
    if (isset($this->parent)) {
      $previous = NULL;
      foreach ($this->parent->getIterator() as $element) {
        if ($element === $this) {
          return $previous;
        }
        $previous = $element;
      }
    }
    // Otherwise, return nothing if no previous sibling is applicable.
    return NULL;
  }

  /**
   * Gets the next sibling element.
   *
   * @return RulesPlugin
   */
  public function getNextSibling() {
    if (isset($this->parent)) {
      $previous = NULL;
      foreach ($this->parent->getIterator() as $element) {
        if ($previous === $this) {
          return $element;
        }
        $previous = $element;
      }
    }
    // Otherwise, return nothing if no next sibling is applicable.
    return NULL;
  }
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
   * @return RulesConditionalBranchPlugin[]
   *   An array of branches to evaluate.
   */
  protected function selectBranches(RulesState $state) {
    // TODO: Implement selectBranches() method.
    return array();
  }
}

/**
 * The "if" clause.
 */
class RulesConditionalIf extends RulesConditionalBranchPlugin {
  protected $itemName = 'if';

  protected $predicate;

  public function __construct($predicate = NULL, $settings = array()) {
    parent::__construct();
    if (isset($predicate)) {
      $this->predicate = is_object($predicate) && $predicate instanceof RulesConditionInterface ? $predicate : rules_condition($predicate, $settings);
      $this->predicate->parent = $this;
    }
  }

  protected function setUp() {
    parent::setUp();
    if (isset($this->predicate)) {
      // Use predicate settings as the branch's own.
      $this->settings = $this->predicate->settings;
    }
  }

  public function __sleep() {
    $array = parent::__sleep();
    $array['predicate'] = 'predicate';
    unset($array['settings']);
    return $array;
  }

  public function getCompatibleConditionalClass() {
    return 'RulesConditional';
  }

  public function label() {
    $pluginLabel = parent::label();
    if (!isset($this->predicate)) {
      return $pluginLabel;
    }
    else {
      return $this->predicate->label();
    }
  }

  /**
   * Uses predicate parameters as the branch's own.
   */
  public function pluginParameterInfo() {
    $this->forceSetUp();
    if (isset($this->predicate)) {
      return $this->predicate->pluginParameterInfo();
    }
    else {
      return parent::pluginParameterInfo();
    }
  }

  /**
   * Adds predicate assertions to state.
   */
  protected function stateVariables($element = NULL) {
    $this->forceSetUp();
    $variables = parent::stateVariables($element);
    // Add assertions from the predicate.
    if (isset($this->predicate) && $assertions = $this->predicate->call('variableInfoAssertions')) {
      $variables = RulesData::addMetadataAssertions($variables, $assertions);
    }
    return $variables;
  }

  /**
   * Determines whether this branch can be evaluated.
   */
  public function canEvaluate(RulesState $state) {
    return $this->evaluateCondition($state);
  }

  /**
   * Evaluates the condition.
   */
  public function evaluateCondition(RulesState $state) {
    return $this->predicate->evaluate($state);
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
  public function canEvaluate(RulesState $state) {
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
   * @return RulesConditionalBranchPlugin[]
   *   An array of branches to evaluate.
   */
  protected function selectBranches(RulesState $state) {
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
  public function canEvaluate(RulesState $state) {
    // TODO: Implement canEvaluate() method.
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
  public function canEvaluate(RulesState $state) {
    return TRUE;
  }
}
