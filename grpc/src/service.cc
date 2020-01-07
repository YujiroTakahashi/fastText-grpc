#include <iostream>
#include <sstream>

#include "di.h"
#include "config.h"
#include "service.h"

namespace croco {


/**
 * LoadModel
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::LoadModel(grpc::ServerContext* context, const croco::ModelRequest* request, croco::DoneReply* reply)
{
    std::string model = request->model();
    croco::config& cfg = croco::Di<croco::config>::get();
    std::string path = cfg.get("model_path");

    path.append("/");
    path.append(model);

    try {
        fasttext_->loadModel(path);
    } catch (const std::invalid_argument& e) {
        gpr_log(GPR_ERROR, "File opening failed. :: [%s]", path.c_str());
        return grpc::Status::CANCELLED;
    }

    return grpc::Status::OK;
}

/**
 * WordRows
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::WordRows(grpc::ServerContext* context, const google::protobuf::Empty* request, croco::RowsReply* reply)
{
    std::shared_ptr<const fasttext::Dictionary> dict = fasttext_->getDictionary();
    int32_t rows = dict->nwords();
    reply->set_rows(rows);

    return grpc::Status::OK;
}

/**
 * LabelRows
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::LabelRows(grpc::ServerContext* context, const google::protobuf::Empty* request, croco::RowsReply* reply)
{
    std::shared_ptr<const fasttext::Dictionary> dict = fasttext_->getDictionary();
    int32_t rows = dict->nlabels();
    reply->set_rows(rows);

    return grpc::Status::OK;
}

/**
 * WordID
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::WordID(grpc::ServerContext* context, const croco::WordRequest* request, croco::IDReply* reply)
{
    std::string word = request->word();
    int32_t id = fasttext_->getWordId(word);
    reply->set_id(id);

    return grpc::Status::OK;
}

/**
 * SubwordID
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::SubwordID(grpc::ServerContext* context, const croco::WordRequest* request, croco::IDReply* reply)
{
    std::string word = request->word();
    int32_t id = fasttext_->getSubwordId(word);
    reply->set_id(id);

    return grpc::Status::OK;
}

/**
 * Word
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::Word(grpc::ServerContext* context, const croco::IDRequest* request, croco::WordReply* reply)
{
    int32_t id = request->id();
    std::shared_ptr<const fasttext::Dictionary> dict = fasttext_->getDictionary();
    std::string word = dict->getWord(id);

    return grpc::Status::OK;
}

/**
 * Label
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::Label(grpc::ServerContext* context, const croco::IDRequest* request, croco::WordReply* reply)
{
    int32_t id = request->id();
    std::shared_ptr<const fasttext::Dictionary> dict = fasttext_->getDictionary();
    std::string word = dict->getLabel(id);

    return grpc::Status::OK;
}

/**
 * Predict
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::Predict(grpc::ServerContext* context, const croco::WordKRequest* request, croco::PredictReply* reply)
{
    std::string word = request->word();
    int32_t k = request->k();

    std::vector<std::pair<fasttext::real, std::string>> predictions;
    try {
        predictions = fasttext_->getPredict(k, word);
        for (auto &p : predictions) {
            auto* node = reply->add_predicts();
            node->set_prob(std::exp(p.first));
            node->set_label(p.second);
        }
    } catch (const std::invalid_argument& e) {
        gpr_log(GPR_ERROR, "%s!", e.what());
    }

    return grpc::Status::OK;
}

/**
 * WordVectors
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::WordVectors(grpc::ServerContext* context, const croco::WordRequest* request, croco::VectorsReply* reply)
{
    std::string word = request->word();
    fasttext::Vector vec(fasttext_->getDimension());

    fasttext_->getWordVector(vec, word);
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        std::stringstream svec("");
        svec << vec[idx];
        reply->add_vectors(std::stof(svec.str()));
    }

    return grpc::Status::OK;
}

/**
 * SubwordVectors
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::SubwordVectors(grpc::ServerContext* context, const croco::WordRequest* request, croco::VectorsReply* reply)
{
    fasttext::Vector vec(fasttext_->getDimension());
    std::string word = request->word();

    fasttext_->getSubwordVector(vec, word);
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        std::stringstream svec("");
        svec << vec[idx];
        reply->add_vectors(std::stof(svec.str()));
    }

    return grpc::Status::OK;
}

/**
 * SentenceVectors
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::SentenceVectors(grpc::ServerContext* context, const croco::WordRequest* request, croco::VectorsReply* reply)
{
    fasttext::Vector vec(fasttext_->getDimension());
    std::string word = request->word();
    std::stringstream ioss(word);

    fasttext_->getSentenceVector(ioss, vec);
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        std::stringstream svec("");
        svec << vec[idx];
        reply->add_vectors(std::stof(svec.str()));
    }

    return grpc::Status::OK;
}

/**
 * Ngrams
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::Ngrams(grpc::ServerContext* context, const croco::WordRequest* request, croco::NgramsReply* reply)
{
    std::vector<std::pair<std::string, fasttext::Vector>> ngramVectors;
    std::string word = request->word();

    try {
        ngramVectors = fasttext_->getNgramVectors(std::string(word));
        for (const auto& value : ngramVectors) {
            auto *node = reply->add_ngrams();
            node->set_word(value.first);
            for (int64_t idx = 0; idx < value.second.size(); idx++) {
                node->add_vectors(value.second[idx]);
            }
        } // for (const auto& value : ngramVectors)
    } catch (const std::invalid_argument& e) {
        gpr_log(GPR_ERROR, "%s!", e.what());
    }

    return grpc::Status::OK;
}

/**
 * NN
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::NN(grpc::ServerContext* context, const croco::WordKRequest* request, croco::ScoreReply* reply)
{
    std::shared_ptr<const fasttext::Dictionary> dict = fasttext_->getDictionary();
    std::vector<std::pair<fasttext::real, std::string>> result;
    std::string word = request->word();
    int32_t k = request->k();
    if (0 >= k) {
        k = fasttext_->getK();
    }

    try {
        result = fasttext_->getNN(word, k);
        for (auto &value : result) {
            auto *node = reply->add_scores();
            node->set_score(value.first);
            node->set_word(value.second);
        }
    } catch (const std::invalid_argument& e) {
        gpr_log(GPR_ERROR, "%s!", e.what());
    }

    return grpc::Status::OK;
}

/**
 * Analogies
 *
 * @access public
 * @param  grpc::ServerContext* context
 * @param  const croco::SearchRequest* request
 * @param  croco::SearchReply* reply
 * @return grpc::Status
 */
grpc::Status fTextServiceImpl::Analogies(grpc::ServerContext* context, const croco::WordKRequest* request, croco::ScoreReply* reply)
{
    std::vector<std::pair<fasttext::real, std::string>> result;
    std::string word = request->word();
    int32_t k = request->k();
    if (0 >= k) {
        k = fasttext_->getK();
    }

    try {
        result = fasttext_->getAnalogies(k, word);
        for (auto &value : result) {
            auto *node = reply->add_scores();
            node->set_score(value.first);
            node->set_word(value.second);
        }
    } catch (const std::invalid_argument& e) {
        gpr_log(GPR_ERROR, "%s!", e.what());
    }

    return grpc::Status::OK;
}

} // namespace croco