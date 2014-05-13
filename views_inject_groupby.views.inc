<?php
/**
 * @file
 * Implements the display extender to add additional setting to views displays.
 */

/**
 * Implements hook_views_plugins().
 */
function views_inject_groupby_views_plugins() {
  $path = drupal_get_path('module', 'views_custom_conditions');
  $plugins = array();
  $plugins['display_extender']['views_inject_groupby'] = array(
    'title' => t('Extender textarea'),
    'help' => t('Add textarea.'),
    'path' => $path,
    'handler' => 'views_inject_groupby_plugin_display_extender_code',
  );
  return $plugins;
}

/**
 * Implements hook_views_query_substitutions().
 */
function views_inject_groupby_views_query_substitutions($query) {
}

/**
 * Implements hook_views_query_alter().
 */
function views_inject_groupby_views_query_alter(&$view, &$query) {
  // $view_name = $view->name;
  // $view_current_display = $view->current_display;
  // if (variable_get('views_inject_groupby_' . $view_name . '_' . $view_current_display)) {
  // }
}

function views_inject_groupby_views_post_build(&$view) {
  $view_name = $view->name;
  $view_current_display = $view->current_display;
  if (variable_get('views_inject_groupby_' . $view_name . '_' . $view_current_display)) {
    $query_snippet = variable_get('views_inject_groupby_' . $view_name.'_'.$view_current_display);
    $query_snippet = Database::getConnection()->prefixTables($query_snippet);

    $query = $view->build_info['query'];
    if( is_object($query) ){
      $oldgroupby =& $query->getGroupBy();
      $oldgroupby = array();
      $query->groupBy( $query_snippet );
    }


    $query_count = $view->build_info['query_count'];
    if( is_object($query_count) ){
      $oldgroupby =& $query_count->getGroupBy();
      $oldgroupby = array();
      $query_count->groupBy( $query_snippet );
    }
  }
}

function views_inject_groupby_views_pre_execute(&$view) {
  $view_name = $view->name;
  $view_current_display = $view->current_display;
  if (variable_get('views_inject_groupby_' . $view_name . '_' . $view_current_display)) {
  }
}

/**
 * Implements hook_views_pre_execute().
 */
function views_inject_groupby_views_pre_render(&$view) {
  // if (variable_get('views_inject_groupby_' . $view->name.'_'.$view->current_display)) {
  //   print _views_inject_groupby_get_query_string($view);
  // }
}

/**
 * Implements hook_query_alter().
 */
function views_inject_groupby_query_alter(&$query) {
  if( isset($query->alterMetaData['view']) ){
    $view_name = $query->alterMetaData['view']->name;
    $view_current_display = $query->alterMetaData['view']->current_display;
    if (variable_get('views_inject_groupby_' . $view_name . '_' . $view_current_display)) {
      $query_snippet = variable_get('views_inject_groupby_' . $view_name.'_'.$view_current_display);

      $oldgroupby = $query->getGroupBy(); $oldgroupby = array();
      $query->groupBy( $query_snippet );
    }
  }
}

function _views_inject_groupby_get_query_string($view) {
  $query= $view->build_info['query'];
  $string = (string) $query;
  $arguments = $query->arguments();

  if (!empty($arguments) && is_array($arguments)) {
    foreach ($arguments as $placeholder => &$value) {
      if (is_string($value)) {
        $value = "'$value'";
      }elseif($value == null){
        $value = 'NULL';
      }
    }

    $string = strtr($string, $arguments);
  }

  $string = preg_replace('/\{(.*?)\}/','$1',$string);

  return $string;
}
