#pragma once

#include <string>
#include <grpcpp/grpcpp.h>

#include "cfasttext.h"
#include "ftext.pb.h"
#include "ftext.grpc.pb.h"

namespace croco {

/**
 * fTextServiceImpl
 *
 * @package     fText-server
 * @author      Yujiro Takahashi <yujiro@cro-co.co.jp>
 */
class fTextServiceImpl final : public croco::fText::Service {
public:
    const float THRESHOLD = 0.04;
    croco::CFastText *fasttext_;
    
public:
    grpc::Status LoadModel(grpc::ServerContext* context, const croco::ModelRequest* request, croco::DoneReply* reply) override ;
    grpc::Status WordRows(grpc::ServerContext* context, const google::protobuf::Empty* request, croco::RowsReply* reply) override ;
    grpc::Status LabelRows(grpc::ServerContext* context, const google::protobuf::Empty* request, croco::RowsReply* reply) override ;
    grpc::Status WordID(grpc::ServerContext* context, const croco::WordRequest* request, croco::IDReply* reply) override ;
    grpc::Status SubwordID(grpc::ServerContext* context, const croco::WordRequest* request, croco::IDReply* reply) override ;
    grpc::Status Word(grpc::ServerContext* context, const croco::IDRequest* request, croco::WordReply* reply) override ;
    grpc::Status Label(grpc::ServerContext* context, const croco::IDRequest* request, croco::WordReply* reply) override ;
    grpc::Status Predict(grpc::ServerContext* context, const croco::WordKRequest* request, croco::PredictReply* reply) override ;
    grpc::Status WordVectors(grpc::ServerContext* context, const croco::WordRequest* request, croco::VectorsReply* reply) override ;
    grpc::Status SubwordVectors(grpc::ServerContext* context, const croco::WordRequest* request, croco::VectorsReply* reply) override ;
    grpc::Status SentenceVectors(grpc::ServerContext* context, const croco::WordRequest* request, croco::VectorsReply* reply) override ;
    grpc::Status Ngrams(grpc::ServerContext* context, const croco::WordRequest* request, croco::NgramsReply* reply) override ;
    grpc::Status NN(grpc::ServerContext* context, const croco::WordKRequest* request, croco::ScoreReply* reply) override ;
    grpc::Status Analogies(grpc::ServerContext* context, const croco::WordKRequest* request, croco::ScoreReply* reply) override ;
}; // class fTextServiceImpl

} // namespace croco