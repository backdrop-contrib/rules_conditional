<?php
/**
 * @file
 * Testing implementations.
 */

/**
 * Test base conditional.
 */
class RulesConditionalTestPlugin extends RulesConditionalPlugin {
  protected $itemName = 'test_conditional';

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
  }
}

/**
 * Test branch.
 */
class RulesConditionalTestBranchPlugin extends RulesConditionalBranchPlugin {
  protected $itemName = 'test_conditional_branch';

  /**
   * Determines whether this branch can be evaluated.
   */
  public function canEvaluate(RulesState $state) {
    // TODO: Implement canEvaluate() method.
  }
}
