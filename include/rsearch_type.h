#pragma once
#include <bits/stdc++.h>
#include "configure.h"
#define CODEBOOK_LINE_SIZE 256

namespace rsearch{

// ---------------ERROR NO. -------------

typedef enum Errorno{
    INDEX_EXISTS = -1,
    INDEX_NO_FIND = -2,
    TRAINDATA_ERROR = -3,
    NO_TRAIN = -4,
    NO_SUPPORT_NUM_LT_K = -5,
    NO_SUPPORT = -6,
    SIZE_TOO_BIG = -7,
    LOAD_DATA_ERROR = -8,
}Errorno;
//using uint64_t = unsigned long long;
//using uint32_t = unsigned int;
//using int64_t = long long;
//using int32_t = int;
//using int8_t = signed char;
//using uint8_t = unsigned char;
typedef int32_t idx_t;
typedef enum DistanceType{
    COSINE = 0,
    EUCLIDEAN = 1,
} DistanceType;

typedef enum MethodType{
    DUMMY = 0,
    X86_RAPID = 1, 
    X86_PQIVF = 2,
    X86_RAPID_MULTI_THREAD = 3,
    X86_PQIVF_MULTI_THREAD = 4,
    FAISS_FLAT = 5,
    FAISS_LSH = 6,
    FAISS_HNSW = 7,
    FAISS_IVFPQ = 8,
    FAISS_IVF = 9,
    Con_index = 101,
} MethodType;


typedef enum GalleryType{
    CPU_BASE_GALLERY = 0,
    PQIVF_GALLERY = 1,
    SIMPLE_GALLERY = 2,
} GalleryType;

typedef enum VarType{
    FLOAT32 = 0,
    INT8 = 1,
    INT32 = 2,
    INT64 = 3,
}VarType;

template<typename T>
struct TMap{
    typedef float type;
};

template <>
struct TMap<int8_t>{
    typedef int type;
};

template <>
struct TMap<float>{
    typedef float type;
};

template<typename T>
using typemap_t = typename TMap<T>::type;

}