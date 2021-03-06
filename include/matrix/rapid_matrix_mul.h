#pragma once
#include "rsearch_def.h"
#include <bits/stdc++.h>
#include "matrix/matrix_mul.h"
#include "utils/utils.h"
#include "utils/avx2_asm.h"
#include "utils/topk_select.h"
namespace rsearch{

using std::vector;
using std::pair;
template<typename T>
class rapid_matrix_mul : public matrix_mul<T>{
public:
    rapid_matrix_mul():matrix_mul<T>(){
        this->value = NULL;
        this->topk_value = NULL;
        this->topk_index = NULL;
        this->res = NULL;
    }
    virtual ~rapid_matrix_mul(){
        if (this->value != NULL)
            free(this->value);
        if (this->topk_value != NULL)
            free(this->topk_value);
        if (this->topk_index != NULL)
            free(this->topk_index);
        if (this->res != NULL)
            free(this->res);
    }
    using Tout = typemap_t<T>;
    virtual int set(int32_t dimension, int32_t topk, int32_t max_batch, int32_t max_block);
    virtual int mul(const T* const A, const T* const B, const Tout* const offset, int batch, int block, pair<Tout, idx_t> **res);
private:
    Tout* value;
    Tout* topk_value;
    idx_t* topk_index;
    pair<Tout, idx_t>* res;
    int32_t max_batch, max_block, dimension, topk;
    //std::mutex mtx;
};
}
