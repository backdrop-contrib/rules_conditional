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
 * UI for configuring the predicate in a conditional branch.
 */
class RulesConditionalPredicateUI extends RulesContainerPluginUI {
  /**
   * Hides the actions form.
   */
  public function form(&$form, &$form_state, $options = array()) {
    // Add label.
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => empty($options['init']) ? $this->property('label') : '',
      '#required' => TRUE,
      '#description' => t('A user-friendly short description of the conditions for this "@plugin".', array('@plugin' => $this->element->plugin())),
    );
    // Build form for predicate.
    $this->property('predicate')->form($form, $form_state);
    $form['elements']['#access'] = empty($options['init']);
    $form['negate']['#access'] = empty($options['init']);
    // Store state for redirecting when initializing.
    if (!empty($options['button'])) {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => !empty($options['init']) ? t('Continue') : t('Save changes'),
        '#weight' => 10,
        '#options_init' => !empty($options['init']),
      );
    }
  }

  public function form_extract_values($form, &$form_state) {
    $form_values = RulesPluginUI::getFormStateValues($form, $form_state);
    if (!empty($form_values['label'])) {
      $label = &$this->property('label');
      $label = $form_values['label'];
    }
    $this->property('predicate')->extender('RulesConditionContainerUI')->form_extract_values($form, $form_state);
    if (!empty($form_state['triggering_element']['#options_init'])) {
      $redirect = RulesPluginUI::path($this->element->root()->name, 'edit', $this->element);
      $form_state['redirect'] = $redirect;
    }
  }

  public function addOperations() {
    $ops = parent::addOperations();
    return $ops;
  }
}
