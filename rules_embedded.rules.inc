<?php
/**
 * @file
 * Rules integration file.
 */

/**
 * Implements hook_rules_plugin_info().
 */
function rules_embedded_rules_plugin_info() {
  return array(
    'embedded rule' => array(
      'label' => t('Embedded rule'),
      'class' => 'RulesEmbeddedRule',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array (
        'RulesPluginUIInterface' => array(
          'class' => 'RulesEmbeddedRuleUI',
        ),
      ),
    ),
  );
}
