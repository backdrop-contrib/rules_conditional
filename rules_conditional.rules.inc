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
    'if' => array(
      'class' => 'RulesConditionalIf',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionContainerUI',
        ),
      ),
    ),
    'else if' => array(
      'class' => 'RulesConditionalElseIf',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionContainerUI',
        ),
      ),
    ),
    'else' => array(
      'class' => 'RulesConditionalElse',
      'embeddable' => 'RulesActionContainer',
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
