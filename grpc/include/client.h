#pragma once

#include <string>
#include <vector>
#include <memory>
#include <grpcpp/grpcpp.h>

#include "ftext.pb.h"
#include "ftext.grpc.pb.h"

namespace croco {

class fTextClient {
public:
    typedef struct _Predict {
        float prob;
        std::string label;
    } sPredict;
    typedef struct _Ngram {
        std::vector<float> vectors;
        std::string word;
    } sNgram;
    typedef struct _Score {
        float score;
        std::string label;
    } sScore;
private:
    std::unique_ptr<croco::fText::Stub> stub_;

public:
    fTextClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(croco::fText::NewStub(channel)) {}

    uint32_t LoadModel(std::string model);
    uint32_t WordRows(void);
    uint32_t LabelRows(void);
    uint32_t WordID(std::string word);
    uint32_t SubwordID(std::string word);
    std::string Word(uint32_t id);
    std::string Label(uint32_t id);
    std::vector<sPredict> Predict(std::string word, uint32_t k);
    std::vector<float> WordVectors(std::string word);
    std::vector<float> SubwordVectors(std::string word);
    std::vector<float> SentenceVectors(std::string word);
    std::vector<sNgram> Ngrams(std::string word);
    std::vector<sScore> NN(std::string word, uint32_t k);
    std::vector<sScore> Analogies(std::string word, uint32_t k);
};

} // namespace croco
