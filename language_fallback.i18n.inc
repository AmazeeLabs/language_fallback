<?php

/**
 * Implements hook_i18n_string_info_alter().
 */
function language_fallback_i18n_string_info_alter(&$object_info) {
  foreach ($object_info as $type => &$info) {
    if (!isset($info['class']) || $info['class'] == 'i18n_string_textgroup_default') {
      $info['class'] = 'language_fallback_i18n_string_textgroup_default';
    }
  }
}

/**
 * Extends the default i18n string textgroup class providing language fallback.
 */
class language_fallback_i18n_string_textgroup_default extends i18n_string_textgroup_default {

  /**
   * {@inheritdoc}
   */
  public static function load_translation($i18nstring, $langcode) {
    parent::load_translation($i18nstring, $langcode);
  }

  /**
   * {@inheritdoc}
   */
  protected function multiple_translation_load($conditions, $langcode) {
    return parent::multiple_translation_load($conditions, $langcode);
  }
}
