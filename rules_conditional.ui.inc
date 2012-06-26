<?php
/**
 * @file
 * Plugin UI implementation.
 */

/**
 * UI for configuring the propositional antecedent in a conditional element.
 */
class RulesConditionalAntecedentUI extends RulesRuleUI {
  /**
   * Hides the actions form.
   */
  public function form(&$form, &$form_state, $options = array()) {
    parent::form($form, $form_state, $options);
    // Hide actions.
    $form['elements']['#access'] = FALSE;
    // Tweak label.
    $form['label']['#description'] = t('A user-friendly short description of the conditions for this "@plugin".', array('@plugin' => $this->element->plugin()));
  }

  public function addOperations() {
    $ops = parent::addOperations();
    dpm($ops);
    return $ops;
  }
}
