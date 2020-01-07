#include "client.h"

namespace croco {

/**
 * LoadModel
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
uint32_t fTextClient::LoadModel(std::string model)
{
    croco::ModelRequest request;
    croco::DoneReply reply;
    grpc::ClientContext context;

    request.set_model(model);
    grpc::Status status = stub_->LoadModel(&context, request, &reply);

    uint32_t result = 0;
    if (!status.ok()) {
        return result;
    }
    result = reply.done();

    return result;
}

/**
 * WordRows
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
uint32_t fTextClient::WordRows(void)
{
    google::protobuf::Empty request;
    croco::RowsReply reply;
    grpc::ClientContext context;

    grpc::Status status = stub_->WordRows(&context, request, &reply);

    uint32_t result = 0;
    if (!status.ok()) {
        return result;
    }
    result = reply.rows();

    return result;
}

/**
 * LabelRows
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
uint32_t fTextClient::LabelRows(void)
{
    google::protobuf::Empty request;
    croco::RowsReply reply;
    grpc::ClientContext context;

    grpc::Status status = stub_->LabelRows(&context, request, &reply);

    uint32_t result = 0;
    if (!status.ok()) {
        return result;
    }
    result = reply.rows();

    return result;
}

/**
 * WordID
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
uint32_t fTextClient::WordID(std::string word)
{
    croco::WordRequest request;
    croco::IDReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    grpc::Status status = stub_->WordID(&context, request, &reply);

    uint32_t result = 0;
    if (!status.ok()) {
        return result;
    }
    result = reply.id();

    return result;
}

/**
 * SubwordID
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
uint32_t fTextClient::SubwordID(std::string word)
{
    croco::WordRequest request;
    croco::IDReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    grpc::Status status = stub_->SubwordID(&context, request, &reply);

    uint32_t result = 0;
    if (!status.ok()) {
        return result;
    }
    result = reply.id();

    return result;
}

/**
 * Word
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::string fTextClient::Word(uint32_t id)
{
    croco::IDRequest request;
    croco::WordReply reply;
    grpc::ClientContext context;

    request.set_id(id);
    grpc::Status status = stub_->Word(&context, request, &reply);

    std::string result("");
    if (!status.ok()) {
        return result;
    }
    result = reply.word();

    return result;
}

/**
 * Label
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::string fTextClient::Label(uint32_t id)
{
    croco::IDRequest request;
    croco::WordReply reply;
    grpc::ClientContext context;

    request.set_id(id);
    grpc::Status status = stub_->Label(&context, request, &reply);

    std::string result("");
    if (!status.ok()) {
        return result;
    }
    result = reply.word();

    return result;
}

/**
 * Predict
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::vector<fTextClient::sPredict> fTextClient::Predict(std::string word, uint32_t k)
{
    croco::WordKRequest request;
    croco::PredictReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    request.set_k(k);
    grpc::Status status = stub_->Predict(&context, request, &reply);

    std::vector<sPredict> result;
    if (!status.ok()) {
        return result;
    }

    result.clear();
    for (int idx = 0; idx < reply.predicts_size(); idx++) {
        auto &predict = reply.predicts(idx);
        sPredict node = {
            predict.prob(),
            predict.label()
        };
        result.push_back(node);
    }

    return result;
}

/**
 * WordVectors
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::vector<float> fTextClient::WordVectors(std::string word)
{
    croco::WordRequest request;
    croco::VectorsReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    grpc::Status status = stub_->WordVectors(&context, request, &reply);

    std::vector<float> result;
    if (!status.ok()) {
        return result;
    }

    for (int idx = 0; idx < reply.vectors_size(); idx++) {
        float node = reply.vectors(idx);
        result.push_back(node);
    }

    return result;
}

/**
 * SubwordVectors
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::vector<float> fTextClient::SubwordVectors(std::string word)
{
    croco::WordRequest request;
    croco::VectorsReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    grpc::Status status = stub_->SubwordVectors(&context, request, &reply);

    std::vector<float> result;
    if (!status.ok()) {
        return result;
    }

    for (int idx = 0; idx < reply.vectors_size(); idx++) {
        float node = reply.vectors(idx);
        result.push_back(node);
    }

    return result;
}

/**
 * SentenceVectors
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::vector<float> fTextClient::SentenceVectors(std::string word)
{
    croco::WordRequest request;
    croco::VectorsReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    grpc::Status status = stub_->SentenceVectors(&context, request, &reply);

    std::vector<float> result;
    if (!status.ok()) {
        return result;
    }

    for (int idx = 0; idx < reply.vectors_size(); idx++) {
        float node = reply.vectors(idx);
        result.push_back(node);
    }

    return result;
}

/**
 * Ngrams
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::vector<fTextClient::sNgram> fTextClient::Ngrams(std::string word)
{
    croco::WordRequest request;
    croco::NgramsReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    grpc::Status status = stub_->Ngrams(&context, request, &reply);

    std::vector<sNgram> result;
    if (!status.ok()) {
        return result;
    }

    result.clear();
    for (int idx = 0; idx < reply.ngrams_size(); idx++) {
        auto &ngram = reply.ngrams(idx);

        std::vector<float> vectors;
        for (int vidx = 0; vidx < ngram.vectors_size(); vidx++) {
            float value = ngram.vectors(vidx);
            vectors.push_back(value);
        }
    
        sNgram node = {
            vectors,
            ngram.word()
        };
        result.push_back(node);
    }

    return result;
}

/**
 * NN
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::vector<fTextClient::sScore> fTextClient::NN(std::string word, uint32_t k)
{
    croco::WordKRequest request;
    croco::ScoreReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    request.set_k(k);
    grpc::Status status = stub_->NN(&context, request, &reply);

    std::vector<sScore> result;
    if (!status.ok()) {
        return result;
    }

    result.clear();
    for (int idx = 0; idx < reply.scores_size(); idx++) {
        auto &score = reply.scores(idx);
        sScore node = {
            score.score(),
            score.word()
        };
        result.push_back(node);
    }

    return result;
}

/**
 * Analogies
 *
 * @access public
 * @param  std::string sentence
 * @return std::string
 */
std::vector<fTextClient::sScore> fTextClient::Analogies(std::string word, uint32_t k)
{
    croco::WordKRequest request;
    croco::ScoreReply reply;
    grpc::ClientContext context;

    request.set_word(word);
    request.set_k(k);
    grpc::Status status = stub_->Analogies(&context, request, &reply);

    std::vector<sScore> result;
    if (!status.ok()) {
        return result;
    }
    return result;
}

} // namespace croco
