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
 * Implements hook_query_alter().
 */
function views_inject_groupby_query_alter(&$query) {
  if(isset($query->alterMetaData['view'])){
    $view_name = $query->alterMetaData['view']->name;
    $view_current_display = $query->alterMetaData['view']->current_display;
    if (variable_get('views_inject_groupby_' . $view_name . '_' . $view_current_display)) {
      $query_snippet = variable_get('views_inject_groupby_' . $view_name.'_'.$view_current_display);

      $oldgroupby = $query->getGroupBy(); $oldgroupby = array();
      $query->groupBy( $query_snippet );
    }
  }
}


/**
 * Implements hook_views_pre_execute().
 */
// function views_inject_groupby_views_pre_render(&$view) {
//   //var_dump('views_inject_groupby_' . $view->name.'_'.$view->current_display);
//   if (variable_get('views_inject_groupby_' . $view->name.'_'.$view->current_display)) {
//     print _views_inject_groupby_get_query_string($view);
//   }
// }


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
