#include <iostream>
#include <map>
#include<string>
#include<optional>
#include "php.h"
#include<vector>
#include "zend_exceptions.h"
static std::map<std::string, std::map<std::string, zif_handler>> handlers;

extern "C" {
    ZEND_DLEXPORT void register_spem_function(const char* class_name, const char* function_name, zif_handler handler) {
        handlers[class_name][function_name] = handler;
    }
}
void ZEND_FASTCALL zif_spem(zend_execute_data *execute_data, zval *return_value) {
    zend_execute_data *prev_execute_data = execute_data->prev_execute_data;
    if (!prev_execute_data->func || !prev_execute_data->func->common.function_name) {
        zend_throw_exception_ex(nullptr, 0, "Illegal invocation of spem");
        return;
    }
    std::string class_name = prev_execute_data->func->common.scope ? ZSTR_VAL(prev_execute_data->func->common.scope->name) : "";
    std::string function_name = ZSTR_VAL(prev_execute_data->func->common.function_name);
    if (!handlers[class_name].contains(function_name)) {
        if (class_name.empty()) {
            zend_throw_exception_ex(nullptr, 0, "Unbound spem function %s invoked", function_name.c_str());
        } else {
            zend_throw_exception_ex(nullptr, 0, "Unbound spem method %s::%s invoked", class_name.c_str(), function_name.c_str());
        }
        return;
    }
    handlers[class_name][function_name](prev_execute_data, return_value);
}
static constexpr zend_internal_arg_info arginfo_spem[] = {
    {  },
};

static constexpr zend_function_entry functions[] = {
    {
        .fname = "spem",
        .handler = zif_spem,
        .arg_info = arginfo_spem
    },
    {}
};
#ifndef ZEND_DEBUG
#define ZEND_DEBUG 0
#endif
zend_module_entry module_entry = {
    .size = sizeof(zend_module_entry),
    .zend_api = ZEND_MODULE_API_NO,
    .zend_debug = ZEND_DEBUG,
    .zts = USING_ZTS,
    .name = "spem",
    .functions = functions,
    .version = "0.0.0",
    .build_id = ZEND_MODULE_BUILD_ID
};
extern "C" {
    ZEND_DLEXPORT zend_module_entry *get_module(void) {
        return &module_entry;
    }
}
