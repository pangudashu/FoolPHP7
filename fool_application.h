#ifndef PHP_FOOL_APPLICATON_H
#define PHP_FOOL_APPLICATON_H


#define FOOL_APPLICATION_PROPERTY_NAME_INSTANCE "_fool_application_obj"


extern zend_class_entry* fool_application_ce;

PHP_FOOLPHP7_API void register_class_applcation(void);

#endif
