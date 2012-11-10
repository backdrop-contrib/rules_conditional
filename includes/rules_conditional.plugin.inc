<?php
/**
 * @file
 * Rules plugin implementation.
 */

/**
 * Default if-else conditional statement.
 *
 * @method RulesConditional if() if($predicate, array $settings = array()) Adds an "if" statement.
 * @method RulesConditional elseIf() elseIf($predicate, array $settings = array()) Adds an "else if" statement. This is an alias for self::if().
 * @method RulesConditional else() else() Adds an "else" statement.
 * @method RulesConditional action() action($name, array $settings = array()) Adds an action to the currently active statement. Pass arguments as rules_action() would need.
 */
class RulesConditional extends RulesConditionalContainer {
  protected $itemName = 'conditional';

  /**
   * Intercepts calls to "if" and "else".
   * @var array
   */
  protected $interceptMethods = array('if', 'elseIf', 'else');

  public function __construct() {
    parent::__construct();
  }

  /**
   * Adds an "if" statement, for use with magic call.
   */
  protected function call_if($predicate, array $settings = array()) {
    $this->fluentElement = $element = rules_conditional_if($predicate, $settings);
    $element->setParent($this);
    return $this;
  }

  /**
   * Adds an "if" as an "else if" statement, for use with magic call.
   */
  protected function call_elseIf($predicate, array $settings = array()) {
    return $this->call_if($predicate, $settings);
  }

  /**
   * Adds an "else" statement, for use with magic call.
   */
  protected function call_else() {
    $this->fluentElement = $element = rules_conditional_else();
    $element->setParent($this);
    return $this;
  }

  /**
   * Selects the branches to evaluate for this conditional.
   *
   * @param RulesState $state
   *   Rules state to use.
   * @return RulesConditionalElement[]
   *   An array of branches to evaluate.
   */
  protected function selectBranches(RulesState $state) {
    /** @var $branch RulesConditionalElement */
    foreach ($this->children as $branch) {
      // Select the first matched branch.
      if ($branch->canEvaluate($state)) {
        return array($branch);
      }
    }
    // Return no branch if none matched.
    return array();
  }

  protected function exportFlat() {
    return TRUE;
  }
}

/**
 * The "if" clause.
 */
class RulesConditionalIf extends RulesConditionalPredicateElement {
  protected $itemName = 'if';

  public function pluginLabel() {
    $previous = $this->getPreviousSibling();
    if ($previous && $previous instanceof RulesConditionalIf) {
      return t('(Else) If', array(), array('context' => 'conditional rules'));
    }
    else {
      return parent::pluginLabel();
    }
  }

  /**
   * Imports predicate.
   */
  protected function importPredicate($export, $key = NULL) {
    $plugin = strtoupper($this->plugin());
    $pluginInfo = $this->pluginInfo();
    $keys = !empty($pluginInfo['import keys']) ? $pluginInfo['import keys'] : array($plugin);
    foreach ($keys as $key) {
      if (isset($export[$key])) {
        parent::importPredicate($export, $key);
        break;
      }
    }
  }

  /**
   * Exports predicate.
   */
  protected function exportPredicate($key = NULL) {
    if ($this->getPreviousSibling()) {
      $key = 'ELSE IF';
    }
    return parent::exportPredicate($key);
  }
}

/**
 * The "else" clause.
 */
class RulesConditionalElse extends RulesConditionalElement {
  protected $itemName = 'else';

  public function __construct() {
    parent::__construct();
  }

  public function isDefault() {
    return TRUE;
  }

  protected function importChildren($export, $key = NULL) {
    RulesContainerPlugin::importChildren($export);
  }

  protected function exportChildren($key = NULL) {
    return RulesContainerPlugin::exportChildren();
  }

  protected function exportFlat() {
    return TRUE;
  }
}

/**
 * Switch conditional container.
 *
 * @method RulesConditionalSwitch case() case($value, $fallThrough = FALSE, $valueIsSelector = FALSE) Adds a "case" statement.
 * @method RulesConditionalSwitch defaultCase() defaultCase() Adds a "default case" statement.
 * @method RulesConditionalSwitch action() action($name, array $settings = array()) Adds an action to the currently active statement. Pass arguments as rules_action() would need.
 */
class RulesConditionalSwitch extends RulesConditionalContainer {
  protected $itemName = 'switch';

  /**
   * Intercepts calls to "case" and "defaultCase".
   * @var array
   */
  protected $interceptMethods = array('case', 'defaultCase');

  public function __construct($dataSelector = NULL) {
    parent::__construct();
    if (isset($dataSelector)) {
      $this->settings['data:select'] = $dataSelector;
    }
  }

  /**
   * Adds a "case" statement, for use with magic call.
   */
  protected function call_case($settings = array(), $fallThrough = FALSE) {
    $this->fluentElement = $element = rules_conditional_case($settings, $fallThrough);
    $element->setParent($this);
    return $this;
  }

  /**
   * Adds a "defaultCase" statement, for use with magic call.
   */
  protected function call_defaultCase() {
    $this->fluentElement = $element = rules_conditional_default_case();
    $element->setParent($this);
    return $this;
  }

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
   * @return RulesConditionalElement[]
   *   An array of branches to evaluate.
   */
  protected function selectBranches(RulesState $state) {
    $branches = array();
    // Collect all cases to be evaluated.
    $fallThrough = FALSE;
    foreach ($this->children as $case) {
      /** @var $case RulesConditionalCase */
      if ($case->canEvaluate($state)) {
        $branches[] = $case;
      }
    }
    return $branches;
  }

  protected function importChildren($export, $key = NULL) {
    parent::importChildren($export, 'DO');
  }

  protected function exportChildren($key = NULL) {
    return parent::exportChildren('DO');
  }
}

/**
 * Switch case.
 */
class RulesConditionalCase extends RulesConditionalElement {
  protected $itemName = 'case';

  /**
   * @var RulesPlugin
   */
  protected $condition;

  /**
   * Evaluated condition results.
   * @var array
   */
  protected $conditionResultCache = array();

  public function __construct($settings = array(), $fallThrough = FALSE) {
    parent::__construct();
    if (isset($settings['value'])) {
      $this->settings['value'] = $settings['value'];
    }
    elseif (isset($settings['value:select'])) {
      $this->settings['value:select'] = $settings['value:select'];
    }
    $this->settings += array('fall_through' => $fallThrough);
  }

  public function __destruct() {
    unset($this->condition);
    unset($this->conditionResultCache);
  }

  public function forceSetUp() {
    parent::forceSetUp();
    $this->setUpCondition();
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
        'description' => t('Fall through to next case when complete. If this option is checked, the next case is automatically executed (regardless of the case value) when this case is finished. If not, the switch will terminate when the case is finished.'),
        'optional' => TRUE,
        'default value' => FALSE,
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

  public function stateVariables($element = NULL) {
    $this->forceSetUp();
    if (!isset($element) || $element === $this->condition) {
      return parent::stateVariables();
    }
    else {
      // Add assertions from the condition.
      $variables = parent::stateVariables($element);
      if (isset($this->condition) && $assertions = $this->condition->call('variableInfoAssertions')) {
        $variables = RulesData::addMetadataAssertions($variables, $assertions);
      }
      return $variables;
    }
  }

  protected function setUpCondition() {
    if (!isset($this->condition) && isset($this->parent)) {
      // Prepare settings for 'data_is' condition.
      $settings = array(
        'data:select' => $this->parent->settings['data:select'],
        'op' => '==',
      );
      if (isset($this->settings['value:select'])) {
        $settings['value:select'] = $this->settings['value:select'];
      }
      elseif (isset($this->settings['value'])) {
        $settings['value'] = $this->settings['value'];
      }
      else {
        // Abort if settings are incomplete.
        return;
      }
      // Set up 'data_is'.
      $this->condition = rules_condition('data_is', $settings);
      $this->condition->parent = $this;
      $this->condition->processSettings();
    }
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
    $this->forceSetUp();
    // Check if this element has fallen through.
    if ($previous = $this->getPreviousSibling()) {
      /** @var $previous self */
      if ($previous instanceof self && $previous->fallThrough() && $previous->canEvaluate($state)) {
        return TRUE;
      }
    }
    // Evaluate condition for the given state once.
    $this->conditionResultCache += array('state' => array(), 'result' => array());
    if (empty($this->conditionResultCache['state']) || !$cacheKey = array_search($state, $this->conditionResultCache['state'], TRUE)) {
      $cacheKey = count($this->conditionResultCache['state']);
      $this->conditionResultCache['state'][$cacheKey] = $state;
      $this->conditionResultCache['result'][$cacheKey] = $this->condition->evaluate($state);
    }
    return !empty($this->conditionResultCache['result'][$cacheKey]);
  }

  public function resetInternalCache() {
    parent::resetInternalCache();
    $this->condition = NULL;
    $this->conditionResultCache = array();
  }
}

/**
 * Switch default case.
 */
class RulesConditionalDefaultCase extends RulesConditionalElement {
  protected $itemName = 'default case';

  public function __construct() {
    parent::__construct();
  }

  public function isDefault() {
    return TRUE;
  }

  protected function importChildren($export, $key = NULL) {
    RulesContainerPlugin::importChildren($export);
  }

  protected function exportChildren($key = NULL) {
    return RulesContainerPlugin::exportChildren();
  }

  protected function exportFlat() {
    return TRUE;
  }
}

/**
 * While loop.
 */
class RulesConditionalWhile extends RulesConditionalPredicateElement {
  protected $itemName = 'while';

  public function providesVariables() {
    return array();
  }

  public function evaluate(RulesState $state) {
    $iteration = 0;
    $maxIterations = variable_get('rules_conditional_max_iterations', RULES_CONDITIONAL_MAX_ITERATIONS);
    while ($iteration < $maxIterations && $this->canEvaluate($state)) {
      // Use a separate state so variables are available in the loop only.
      $clonedState = clone $state;
      parent::evaluate($clonedState);
      $iteration ++;

      // Retrieve variables.
      foreach ($state->variables as $key => &$value) {
        if (array_key_exists($key, $clonedState->variables)) {
          $value = $clonedState->variables[$key];
        }
      }
    }
  }

  /**
   * Deletes the element and its children.
   */
  public function delete($keep_children = TRUE) {
    parent::delete($keep_children);
  }
}
