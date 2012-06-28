<?php
/**
 * @file
 * Plugin UI implementation.
 */

/**
 * Base UI for providing customized operation links.
 */
class RulesConditionalPluginUI extends RulesContainerPluginUI {
  public function operations() {
    $ops = parent::operations();
    foreach (rules_fetch_data('plugin_info') as $plugin => $info) {
      if (!empty($info['conditional predicate']) && isset($ops['#links']['add_' . $plugin])) {
        $ops['#links']['add_' . $plugin]['href'] = RulesPluginUI::path($this->element->root()->name, 'add-predicate', $this->element, $plugin);
      }
    }
    return $ops;
  }
}

/**
 * Empty UI for doing nothing with the plugin.
 */
class RulesConditionalEmptyUI extends RulesConditionalPluginUI {
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
   * Delegates the form to the predicate.
   */
  public function form(&$form, &$form_state, $options = array()) {
    /** @var $predicate RulesPlugin */
    $predicate = $this->property('predicate');
    if (!is_object($predicate) || $predicate instanceof RulesContainerPlugin) {
      // TODO: Find out how to eventually nest a conditional container UI.
      return;
    }
    // Build form for predicate.
    $predicate->form($form, $form_state, $options);
  }

  /**
   * Delegates the form validator to the predicate.
   */
  public function form_validate($form, &$form_state) {
    /** @var $predicate RulesPlugin */
    $predicate = $this->property('predicate');
    if (!is_object($predicate) || $predicate instanceof RulesContainerPlugin) {
      // TODO: Find out how to eventually nest a conditional container UI.
      return;
    }
    // Validate form for predicate.
    $predicate->form_validate($form, $form_state);
  }

  /**
   * Delegates the form submit handler to the predicate.
   */
  public function form_submit($form, &$form_state) {
    /** @var $predicate RulesPlugin */
    $predicate = $this->property('predicate');
    if (!is_object($predicate) || $predicate instanceof RulesContainerPlugin) {
      // TODO: Find out how to eventually nest a conditional container UI.
      return;
    }
    // Handle form submission for predicate.
    $predicate->form_submit($form, $form_state);
  }

  public function buildContent() {
    $content = parent::buildContent();
    $content['label']['#markup'] = t('@plugin %label', array(
      '@plugin' => drupal_ucfirst($this->element->plugin()),
      '%label' => $this->element->label(),
    ));
    return $content;
  }
}
