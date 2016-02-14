#ifndef PHP_FOOL_REQUEST_H
#define PHP_FOOL_REQUEST_H

#define FOOL_REQUEST_PROPERTY_NAME_INSTANCE "_fool_request_obj"

extern zend_class_entry* fool_request_ce;

//注册class
PHP_FOOLPHP7_API void register_class_request(void);

PHP_FOOLPHP7_API zval* fool_request_instance();
PHP_FOOLPHP7_API int fool_request_parse_method(void);

#endif
