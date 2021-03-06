#pragma once
#include "rsearch_def.h"
#include "probe/rsearch_probe.h"
#include "matrix/rapid_matrix_mul.h"
#include "gallery/pqivf_gallery.h"
#include "utils/utils.h"
#include "matrix/rapid_matrix_la.h"

namespace rsearch{
using std::pair;
using std::vector;
using std::make_pair;
template<typename T,
        DistanceType dist_type>
class pqivf_probe : public probe<T>{
public:
    pqivf_probe(int dimension, int topk);
    ~pqivf_probe();
    using Tout = typemap_t<T>;
    virtual int create_gallery(gallery<T> ** ga_ptr) override;
    virtual int query(const T * const x, const int n, gallery<T> * ga, Tout *sims, idx_t *idx) override;
    virtual int query_with_uids(const T* const x, const int n, gallery<T> * ga, idx_t *uids, const int m, Tout *sims, idx_t *idx) override;
private:
    matrix_mul<T>* cq_mm;
    //matrix_mul<T>* pq_mm;
    matrix_la<Tout>* mtx_la;
    int32_t max_batch, max_block, topk, dimension;

    int cq_num;
    int select_cq;
    int pq_dimension;
    int pq_num;
    int code_len;
    int res_cache_size;
    int codebook_size;

    //pair<Tout, idx_t>* res;
    pair<Tout, idx_t>* merge_cache;
    vector<pair<Tout, idx_t> > ans;

    Tout* code_book;
    int32_t* prefix;
    uint32_t nprocs;

    vector<T> x_tmp;
    vector<T> x_tmp_div;
    vector<Tout> x_offset;
};

}
