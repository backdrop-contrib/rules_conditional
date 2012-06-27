<?php
/**
 * @file
 * Testing implementations.
 */

/**
 * Test base conditional.
 */
class RulesConditionalTestStubPlugin extends RulesConditionalPlugin {
  protected $itemName = 'stub conditional';

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
class RulesConditionalTestStubBranchPlugin extends RulesConditionalBranchPlugin {
  protected $itemName = 'stub conditional branch';

  /**
   * Determines whether this branch can be evaluated.
   */
  public function canEvaluate(RulesState $state) {
    // TODO: Implement canEvaluate() method.
  }
}
