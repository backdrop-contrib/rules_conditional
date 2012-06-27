<?php
/**
 * @file
 * Plugin UI implementation.
 */

/**
 * Empty UI for doing nothing with the plugin.
 */
class RulesConditionalEmptyUI extends RulesContainerPluginUI {
  public function form(&$form, &$form_state, $options = array()) {
    // Save element.
    $this->element->save();
    // Redirect right away.
    $path = RulesPluginUI::defaultRedirect($this->element);
    drupal_goto($path);
  }

  public function operations() {
    $ops = parent::operations();
    // Get rid of the 'edit' link.
    if (isset($ops['#links']['edit'])) {
      unset($ops['#links']['edit']);
    }
    return $ops;
  }
}

/**
 * UI for configuring the propositional antecedent in a conditional element.
 */
class RulesConditionalAntecedentUI extends RulesContainerPluginUI {
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
    return $ops;
  }
}
