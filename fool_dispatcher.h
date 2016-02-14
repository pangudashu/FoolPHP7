#ifndef PHP_FOOL_DISPATCHER_H
#define PHP_FOOL_DISPATCHER_H

#define FOOL_DISPATCHER_PROPERTY_NAME_INSTANCE "_fool_dispatcher_obj"

extern zend_class_entry* fool_dispatcher_ce;

//注册class
PHP_FOOLPHP7_API void register_class_dispatcher(void);

PHP_FOOLPHP7_API int fool_dispatcher_load_classmap(char *module);
PHP_FOOLPHP7_API zend_class_entry* fool_dispatcher_get_controller_entry(char* module, char *controller);
PHP_FOOLPHP7_API zval* fool_dispatcher_instance();
PHP_FOOLPHP7_API int fool_dispatcher_dispatch(void);

#endif
