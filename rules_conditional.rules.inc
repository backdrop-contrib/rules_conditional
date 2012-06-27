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
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalEmptyUI',
        ),
      ),
    ),
    'if' => array(
      'class' => 'RulesConditionalIf',
      'embeddable' => 'RulesConditional',
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
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalEmptyUI',
        ),
      ),
    ),
    'switch' => array(
      'class' => 'RulesConditionalSwitch',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesPluginUI',
        ),
      ),
    ),
    'case' => array(
      'class' => 'RulesConditionalCase',
      'embeddable' => 'RulesConditionalSwitch',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesPluginUI',
        ),
      ),
    ),
    'default case' => array(
      'class' => 'RulesConditionalDefaultCase',
      'embeddable' => 'RulesConditionalSwitch',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalEmptyUI',
        ),
      ),
    ),
  );
}
