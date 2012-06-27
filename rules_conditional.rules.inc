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
      'label' => t('Conditional', array(), array('context' => 'conditional rules')),
      'class' => 'RulesConditional',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalEmptyUI',
        ),
      ),
    ),
    'if' => array(
      'label' => t('If', array(), array('context' => 'conditional rules')),
      'class' => 'RulesConditionalIf',
      'embeddable' => 'RulesConditional',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalPredicateUI',
        ),
      ),
    ),
    'else if' => array(
      'label' => t('Else if', array(), array('context' => 'conditional rules')),
      'class' => 'RulesConditionalElseIf',
      'embeddable' => 'RulesConditional',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalPredicateUI',
        ),
      ),
    ),
    'else' => array(
      'label' => t('Else', array(), array('context' => 'conditional rules')),
      'class' => 'RulesConditionalElse',
      'embeddable' => 'RulesConditional',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesConditionalEmptyUI',
        ),
      ),
    ),
    'switch' => array(
      'label' => t('Switch', array(), array('context' => 'conditional rules')),
      'class' => 'RulesConditionalSwitch',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesPluginUI',
        ),
      ),
    ),
    'case' => array(
      'label' => t('Case', array(), array('context' => 'conditional rules')),
      'class' => 'RulesConditionalCase',
      'embeddable' => 'RulesConditionalSwitch',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesPluginUI',
        ),
      ),
    ),
    'default case' => array(
      'label' => t('Default case', array(), array('context' => 'conditional rules')),
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
