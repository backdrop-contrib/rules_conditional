<?php
/**
 * @file
 * Testing implementations.
 */

/**
 * Test base conditional.
 */
class RulesConditionalTestPlugin extends RulesConditionalPlugin {
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
  /**
   * Determines whether this branch can be evaluated.
   */
  public function canEvaluate(RulesState $state) {
    // TODO: Implement canEvaluate() method.
  }
}
