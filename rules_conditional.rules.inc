<?php
/**
 * @file
 * Rules integration file.
 */

/**
 * Implements hook_rules_plugin_info().
 */
function rules_conditional_rules_plugin_info() {
  return array(
    'conditional' => array(
      'class' => 'RulesConditional',
      'embeddable' => 'RulesActionContainer',
    ),
    'if' => array(
      'class' => 'RulesConditionalIf',
      'embeddable' => 'RulesConditionalBranchContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalAntecedentUI',
        ),
      ),
    ),
    'else if' => array(
      'class' => 'RulesConditionalElseIf',
      'embeddable' => 'RulesConditional',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalAntecedentUI',
        ),
      ),
    ),
    'else' => array(
      'class' => 'RulesConditionalElse',
      'embeddable' => 'RulesConditional',
    ),
    'switch' => array(
      'class' => 'RulesConditionalSwitch',
      'embeddable' => 'RulesActionContainer',
    ),
    'when' => array(
      'class' => 'RulesConditionalCase',
      'embeddable' => 'RulesActionContainer',
    ),
  );
}
