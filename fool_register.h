#ifndef PHP_FOOL_REGISTER_H
#define PHP_FOOL_REGISTER_H

#define FOOL_REGISTER_PROPERTY_CLASSMAP  "_fool_register_classmap"
#define FOOL_REGISTER_PROPERTY_CONFIG    "_fool_register_config"
#define FOOL_REGISTER_PROPERTY_REQUEST_MODULE "_fool_register_request_module"
#define FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER "_fool_register_request_controller"
#define FOOL_REGISTER_PROPERTY_REQUEST_ACTION "_fool_register_request_action"

extern zend_class_entry* fool_register_ce;

PHP_FOOLPHP7_API void register_class_register(void);
PHP_FOOLPHP7_API int fool_register_set(char* property,zval* value);
PHP_FOOLPHP7_API zval* fool_register_get(char* property);

#endif
