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
    if (!empty($i18nstring->lid)) {
      $query = db_select('locales_target', 't');
      $query->condition('t.lid', $i18nstring->lid);
    }
    else {
      $query = db_select('i18n_string', 's')->fields('s');
      $query->leftJoin('locales_target', 't', 's.lid = t.lid');
      $query->condition('s.textgroup', $i18nstring->textgroup);
      $query->condition('s.context', $i18nstring->context);
    }
    $query->fields('t', array('translation', 'i18n_status'));
    $query->condition('t.language', $langcode);
    $query->range(0, 1);
    return $query->execute()->fetchObject();
  }

  /**
   * {@inheritdoc}
   */
  protected function multiple_translation_load($conditions, $langcode) {
    $conditions += array(
      'language' => $langcode,
      'textgroup' => $this->textgroup
    );
    $query = db_select('i18n_string', 's')->fields('s');
    $query->leftJoin('locales_target', 't', 's.lid = t.lid');
    $query->fields('t', array('translation', 'language', 'i18n_status'));
    foreach ($conditions as $field => $value) {
      if (is_array($value) && count($value) == 1) {
        $value = reset($value);
      }
      if ($value === '*') {
        continue;
      }
      elseif ($field == 'language') {
        $query->condition('t.language', $value);
      }
      else {
        $query->condition('s.' . $field, $value);
      }
    }
    return $this->multiple_translation_build($query->execute()->fetchAll(), $langcode);
  }
}
