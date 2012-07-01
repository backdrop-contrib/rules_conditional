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
    // Check parent class against the compatible class.
    $compatibleClass = $this->getCompatibleConditionalClass();
    if (!$parent instanceof $compatibleClass) {
      throw new RulesEvaluationException('This branch element is incompatible with the given conditional.', array(), $this, RulesLog::ERROR);
    }
    else {
      // Remove from current parent.
      if (isset($this->parent)) {
        $this->parent->removeBranch($this);
      }
      $this->parent = $parent;
      $this->parent->addBranch($this);
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

  public function integrityCheck() {
    parent::integrityCheck();
    $this->checkSiblings();
    return $this;
  }

  protected function checkSiblings() {}
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
    /** @var $branch RulesConditionalBranchPlugin */
    foreach ($this->children as $branch) {
      // Select the first matched branch.
      if ($branch->canEvaluate($state)) {
        return array($branch);
      }
    }
    // Return no branch if none matched.
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
      $predicate = is_object($predicate) && $predicate instanceof RulesConditionInterface ? $predicate : rules_condition($predicate, $settings);
      $this->setPredicate($predicate);
    }
  }

  /**
   * Sets a condition as predicate.
   */
  protected function setPredicate($predicate) {
    $this->predicate = $predicate;
    $this->predicate->parent = $this;
    // Set up variables with the new parent.
    $this->resetInternalCache();
  }

  public function resetInternalCache() {
    parent::resetInternalCache();
    if (isset($this->predicate)) {
      $this->predicate->resetInternalCache();
    }
  }

  public function __sleep() {
    $array = parent::__sleep();
    $array['predicate'] = 'predicate';
    return $array;
  }

  public function getCompatibleConditionalClass() {
    return 'RulesConditional';
  }

  public function label() {
    if (!isset($this->predicate)) {
      return parent::label();
    }
    else {
      return $this->predicate->label();
    }
  }

  public function integrityCheck() {
    if (!isset($this->predicate)) {
      throw new RulesIntegrityException(t('The conditional "%plugin" does not have a valid predicate.', array('%plugin' => $this->plugin())), $this);
    }
    parent::integrityCheck();
    return $this;
  }

  protected function checkSiblings() {
    if ($this->getPreviousSibling()) {
      throw new RulesIntegrityException(t('The conditional "%plugin" cannot be preceded by another element.', array('%plugin' => $this->plugin())), $this);
    }
  }

  /**
   * Adds predicate assertions to state.
   */
  protected function stateVariables($element = NULL) {
    if (!isset($element) || $element === $this->predicate) {
      return parent::stateVariables();
    }
    else {
      // Add assertions from the predicate.
      $variables = parent::stateVariables($element);
      if (isset($this->predicate) && $assertions = $this->predicate->call('variableInfoAssertions')) {
        $variables = RulesData::addMetadataAssertions($variables, $assertions);
      }
      return $variables;
    }
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

  /**
   * Imports predicate.
   */
  protected function importChildren($export, $key = NULL) {
    $plugin = strtoupper($this->plugin());
    if (isset($export[$plugin])) {
      $predicate = rules_plugin_factory('condition');
      $this->setPredicate($predicate);
      $predicate->import($export[$plugin]);
    }
    parent::importChildren($export, 'DO');
  }

  /**
   * Exports predicate with actions.
   */
  protected function exportChildren($key = NULL) {
    $export = array();
    if (isset($this->predicate)) {
      $plugin = strtoupper($this->plugin());
      $export[$plugin] = $this->predicate->export();
    }
    return $export + parent::exportChildren('DO');
  }
}

/**
 * The "else if" clause.
 */
class RulesConditionalElseIf extends RulesConditionalIf {
  protected $itemName = 'else if';

  protected function checkSiblings() {
    if (!($sibling = $this->getPreviousSibling()) || !$sibling instanceof RulesConditionalIf) {
      throw new RulesIntegrityException(t('The conditional "%plugin" must be preceded by "if".', array('%plugin' => $this->plugin())), $this);
    }
  }
}

/**
 * The "else" clause.
 */
class RulesConditionalElse extends RulesConditionalBranchPlugin {
  protected $itemName = 'else';

  public function getCompatibleConditionalClass() {
    return 'RulesConditional';
  }

  protected function checkSiblings() {
    if ($this->getNextSibling()) {
      throw new RulesIntegrityException(t('The conditional "%plugin" cannot precede another element.', array('%plugin' => $this->plugin())), $this);
    }
    if (!($sibling = $this->getPreviousSibling()) || !$sibling instanceof RulesConditionalIf) {
      throw new RulesIntegrityException(t('The conditional "%plugin" must be preceded by "if" or "else if".', array('%plugin' => $this->plugin())), $this);
    }
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

  public function pluginParameterInfo() {
    $parameterInfo = array(
      'data' => array(
        'type' => '*',
        'label' => t('Data to match cases against'),
        'description' => t('The data to be compared, specified by using a data selector, e.g. "node:author:name".'),
        'restriction' => 'selector',
        'allow null' => TRUE,
      ),
    );
    return $parameterInfo;
  }

  /**
   * Selects the branches to evaluate for this conditional.
   *
   * @param RulesState $state
   *   Rules state to use.
   * @return RulesConditionalBranchPlugin[]
   *   An array of branches to evaluate.
   */
  protected function selectBranches(RulesState $state) {
    $branches = array();
    /** @var $case RulesConditionalCase */
    foreach ($this->children as $case) {
      if ($case->canEvaluate($state)) {
        $branches[] = $case;
        if (!$case->fallThrough()) {
          break;
        }
      }
    }
    return $branches;
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

  public function pluginParameterInfo() {
    $parameterInfo = array(
      'value' => array(
        'type' => '*',
        'label' => t('Data value'),
        'description' => t('The value to compare the data with.'),
        'allow null' => TRUE,
      ),
      'fall_through' => array(
        'type' => 'boolean',
        'label' => t('Fall through'),
        'description' => t('Fall through to next case when complete. If this option is not checked, the switch will terminate.'),
        'restriction' => 'input',
      ),
    );
    // Derive parameter info from switch variable selector.
    $dataSelector = isset($this->parent->settings['data:select']) ? $this->parent->settings['data:select'] : NULL;
    if ($wrapper = $this->applyDataSelector($dataSelector)) {
      $parameterInfo['value']['type'] = $wrapper->type();
    }
    return $parameterInfo;
  }

  /**
   * Returns whether this case should fall through.
   */
  public function fallThrough() {
    return !empty($this->settings['fall_through']);
  }

  /**
   * Determines whether this branch can be evaluated.
   */
  public function canEvaluate(RulesState $state) {
    // Prepare settings for 'data_is' condition.
    $settings = array(
      'data:select' => $this->parent->settings['data:select'],
      'op' => '==',
    );
    if (isset($this->settings['value:select'])) {
      $settings['value:select'] = $this->settings['value:select'];
    }
    else {
      $settings['value'] = $this->settings['value'];
    }
    // Evaluate 'data_is'.
    $condition = rules_condition('data_is', $settings);
    $condition->parent = $this;
    return $condition->evaluate($state);
  }
}

/**
 * Switch default case.
 */
class RulesConditionalDefaultCase extends RulesConditionalCase {
  protected $itemName = 'default case';

  public function pluginParameterInfo() {
    return array();
  }

  protected function checkSiblings() {
    if ($this->getNextSibling()) {
      throw new RulesIntegrityException(t('The switch "%plugin" cannot precede another element.', array('%plugin' => $this->plugin())), $this);
    }
    if (!$this->getPreviousSibling()) {
      throw new RulesIntegrityException(t('The switch "%plugin" must be preceded by "case".', array('%plugin' => $this->plugin())), $this);
    }
  }

  /**
   * Determines whether this branch can be evaluated.
   */
  public function canEvaluate(RulesState $state) {
    return TRUE;
  }
}
