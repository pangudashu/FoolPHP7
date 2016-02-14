#ifndef PHP_FOOL_VIEW_H
#define PHP_FOOL_VIEW_H

#define FOOL_VIEW_PROPERTY_NAME_INSTANCE "_fool_view_obj"
#define FOOL_VIEW_PROPERTY_NAME_TPL_DATA "_fool_view_tpl_data"
#define FOOL_VIEW_PROPERTY_NAME_TPL_ROOT "_fool_view_tpl_root"

#define FOOL_VIEW_TPL_SUFFIX   ".tpl.php"
#define FOOL_VIEW_VAR_TPL_ROOT "VIEW_ROOT"

extern zend_class_entry* fool_view_ce;

//注册class
PHP_FOOLPHP7_API void register_class_view(void);

PHP_FOOLPHP7_API zval* fool_view_instance();
PHP_FOOLPHP7_API int fool_view_assign(zval* instance, char* key, zval* value);

#endif
