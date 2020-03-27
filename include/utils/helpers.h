#pragma once
#include "rsearch_type.h"
#include <cxxabi.h>
#include <bits/stdc++.h>

namespace rsearch{

inline std::string GetMethodName(MethodType dev) {

    char target_name[300];
    switch (dev)
    {
    case DUMMY:
        sprintf(target_name, "DUMMY");
        break;
    case X86_RAPID:
        sprintf(target_name, "X86_RAPID");
        break;
    case X86_PQIVF:
        sprintf(target_name, "X86_PQIVF");
        break;
    default:
        sprintf(target_name, "UN_KNOWN");
        break;
    }

    return std::string(target_name)    ;
}

inline std::string GetGalleryName(MethodType dev){
    char target_name[300];
    switch (dev)
    {
    case DUMMY:
        sprintf(target_name, "CPU_BASE_GALLEY");
        break;
    case X86_RAPID:
        sprintf(target_name, "CPU_BASE_GALLEY");
        break;
    case X86_PQIVF:
        sprintf(target_name, "PQIVF_GALLERY");
        break;
    default:
        sprintf(target_name, "UN_KNOWN");
        break;
    }

    return std::string(target_name)    ;
}

template<typename T> 
inline std::string GetTypeName() {
    int status;
    char * type_name = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status);
    std::string ret(type_name);
    free(type_name);
    return ret;
}

}