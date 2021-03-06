#include <rsearch_type.h>
#include <gallery/rsearch_gallery.h>
#include <probe/rsearch_probe.h>
#include <utils/utils.h>
#include <utils/helpers.h>
#include <sys/time.h>
#include "unit_test.h"

template<typename T,
        rsearch::DistanceType dist_type>
int test_query(const int N, const int K, const int Dimension, const rsearch::MethodType mt){
    struct timezone zone;
    struct timeval time1;
    struct timeval time2;
    using Tout = rsearch::typemap_t<T>;
    std::string target_name;
    std::string type_name;
    std::string file_name;
    target_name = rsearch::GetMethodName(mt);
    type_name = rsearch::GetTypeName<T>();

    file_name = std::string(DATA_DIR) + "/rsearch_gallery." + rsearch::GetGalleryName(mt) + "." + rsearch::GetDistancetypeName<dist_type>() + "." + std::to_string(N) + "." + std::to_string(Dimension) + "." + \
                type_name + ".bin";
    rsearch::probe<T>* probe = rsearch::create_probe<T>(Dimension, K, dist_type, mt);
    std::cout << "[test_query] target 1\n" << std::endl; 
    rsearch::gallery<T>* ga;
    int ret = probe->create_gallery(&ga);
    std::cout << "[test_query] target 2" << std::endl; 
    if (ret != 0){
        printf("Create probe failed.\n");
        return -1;
    }
    std::vector<T> x;
    rsearch::get_random_data<T, dist_type>(x, N, Dimension);
    const int batch = 128;
    rsearch::idx_t top_uids[K * batch];//, real_uids[K * batch];
    int target = 1123;
    //for (int i = 0; i < batch; ++i)
    //    real_uids[i] = target + i;

    std::vector<T> query_vec(batch * Dimension);
    memcpy(query_vec.data(), x.data() + target * Dimension, batch * Dimension * sizeof(T));
    
    
    /*Tout ans = rsearch::get_offset<T, dist_type>(query_vec.data() + Dimension, Dimension);
    std::cout << "ans0 = " << ans << std::endl;
    for (int i = 0; i < Dimension; ++i)
        ans += (query_vec[Dimension + i] + 64)* query_vec[Dimension + i];
    std::cout << "ans1 = " << ans << std::endl;
    ans = rsearch::dot_prod<T>(query_vec.data() + Dimension, query_vec.data() + Dimension, Dimension) - 2 * ans;
    std::cout << "ans2 = " << ans << std::endl;
    */
    
    if (ga->init() != 0){
        printf("Gallery init failed.\n");
        return -1;
    }
    std::cout << "[test_query] target 3" << std::endl; 
    //memset(query_vec.data(), 0, batch * Dimension * sizeof(T));
    
    if (rsearch::file_exist(file_name.c_str()) == false){
        ret = ga->add(x.data(), N);
        ga->store_data(file_name);
    }
    else{
        if (ga->load_data(file_name) != 0){
            ret = ga->add(x.data(), N);
            ga->store_data(file_name);
        }
    }
    std::cout << "[test_query] target 4 "<< std::endl; 

    Tout sims[K * batch];

    const int nIter = 10;
    gettimeofday(&time1, &zone);
    for (int i = 0; i < nIter; ++i)
        ret = probe->query(query_vec.data(), batch, ga, &sims[0], &top_uids[0]);
    gettimeofday(&time2, &zone);
    std::cout << "[test_query] target 5" << std::endl; 
    if (ret != 0){
        printf("Query Faieled\n");
        return -1;
    }
    float delta = (time2.tv_sec - time1.tv_sec) * 1000.0 + (time2.tv_usec - time1.tv_usec) / 1000.0;

    float QPS = 1000 / (delta / nIter) * batch;

    printf("[%s] <%s> Batch [%3d] Query On %8d Gallery, cost:%4fms, QPS: %.2f\n", target_name.c_str(), type_name.c_str(), batch, N, delta/nIter, QPS);
    int flag = 0;
    int correct = 0;
    int64_t TP= 0, FN, FP, TN;
    for (int i = 0; i < batch; ++i){
        for (int j = 0; j < K; ++j)
            if (top_uids[i * K + j] == target + i) ++TP;
        if (top_uids[i * K] != target + i){
            std::cout << "Expect uid: " << target + i << ", real uid: "  << top_uids[i * K] << ", real sims:" << sims[i * K]<< std::endl;
            flag = -1;
        }
        else{
            std::cout << "Expect uid: " << target + i << ", real uid: "  << top_uids[i * K] << ", real sims:" << sims[i * K] << std::endl;
        }
    }
    //std::cout << "correct : " << 1.0 * correct / batch << std::endl;
    x.clear();
    FN = batch * K - TP;
    FP = batch * K - TP;
    TN = 1LL * batch * N - FN;

    std::cout << "TP=" << TP << ", FN=" << FN << ",FP=" << FP << ",TN=" << TN << std::endl;
    std::cout << "accuracy rate: " << 1.0 * (TP + TN) / (batch * N) << std::endl;
    std::cout << "recall rate: " << 1.0 * TP / (TP + FN) << std::endl;

    delete probe;
    delete ga;
    return flag;
}

TEST_F(UnitTest, QueryPerfTest) {
    
    //EXPECT_EQ(0, (test_query<float, rsearch::COSINE>(30000, 128, 512, rsearch::DUMMY)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::COSINE>(30000, 128, 512, rsearch::DUMMY)) );
    //EXPECT_EQ(0, (test_query<float, rsearch::COSINE>(30000, 128, 512, rsearch::X86_RAPID)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::COSINE>(30000, 128, 512, rsearch::X86_RAPID)) );
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::X86_RAPID)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::X86_RAPID)) );
    //EXPECT_EQ(0, (test_query<float, rsearch::COSINE>(30000, 128, 512, rsearch::X86_PQIVF)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::COSINE>(30000, 128, 512, rsearch::X86_PQIVF)) );
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::X86_PQIVF)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::X86_PQIVF)) );
    
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::FAISS_FLAT)));
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::FAISS_LSH)));
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::FAISS_IVFPQ)));
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(30000, 128, 512, rsearch::FAISS_HNSW)));


    //EXPECT_EQ(0, (test_query<float, rsearch::COSINE>(1000000, 128, 512, rsearch::X86_PQIVF)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::EUCLIDEAN>(1000000, 128, 512, rsearch::X86_PQIVF)) );
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(5000000, 128, 512, rsearch::X86_PQIVF)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::EUCLIDEAN>(5000000, 128, 512, rsearch::X86_RAPID)) );
    //EXPECT_EQ(0, (test_query<int8_t, rsearch::EUCLIDEAN>(5000000, 128, 512, rsearch::X86_RAPID_MULTI_THREAD)) );
    EXPECT_EQ(0, (test_query<int8_t, rsearch::EUCLIDEAN>(1000000, 128, 512, rsearch::X86_RAPID)) );
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(1000000, 128, 512, rsearch::X86_RAPID_MULTI_THREAD)) );
    //EXPECT_EQ(0, (test_query<float, rsearch::EUCLIDEAN>(1000000, 128, 512, rsearch::FAISS_IVFPQ)));
}
