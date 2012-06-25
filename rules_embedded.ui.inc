<?php
/**
 * @file
 * Plugin UI implementation.
 */

/**
 * UI for embedded rule.
 */
class RulesEmbeddedRuleUI extends RulesRuleUI {
  public function form(&$form, &$form_state, $options = array()) {
    parent::form($form, $form_state, $options);

    // Make label optional.
    $form['label']['#required'] = FALSE;
    $form['label']['#description'] = t('A user-friendly name for describing the embedded rule. If not specified, a generic label will be used.');
    if ($form['label']['#default_value'] == t('unlabeled')) {
      $form['label']['#default_value'] = '';
    }

    // TODO
  }

  /**
   * Applies the embedded rule label regardless.
   */
  function form_extract_values($form, &$form_state) {
    parent::form_extract_values($form, $form_state);
    $form_values = RulesPluginUI::getFormStateValues($form, $form_state);

    // Apply label.
    if (isset($form_values['label'])) {
      $this->element->label = $form_values['label'];
    }
  }

  public function buildContent() {
    $content = parent::buildContent();

    // TODO

    return $content;
  }
}
