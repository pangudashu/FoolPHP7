#ifndef PHP_FOOL_CONTROLLER_H
#define PHP_FOOL_CONTROLLER_H

#define FOOL_CONTROLLER_PROPERTY_NAME_INSTANCE "_fool_controller_obj"
#define FOOL_CONTROLLER_PROPERTY_NAME_VIEW     "view"
#define FOOL_CONTROLLER_PROPERTY_NAME_REQUEST  "request"

extern zend_class_entry* fool_controller_ce;

//注册class
PHP_FOOLPHP7_API void register_class_controller(void);

PHP_FOOLPHP7_API zval* fool_controller_instance();
PHP_FOOLPHP7_API zval* fool_controller_init(zval* controller, zval* view, zval* request);

#endif
