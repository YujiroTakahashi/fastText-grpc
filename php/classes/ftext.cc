#include <iterator>
#include <sstream>
#include <vector>

#include "ftext.h"

/* {{{ proto void fasttext::__construct()
 */
PHP_METHOD(fasttext, __construct)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *address;
	size_t address_len;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &address, &address_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);

    croco::fTextClient *client = new croco::fTextClient(
        grpc::CreateChannel(
            std::string(address),
            grpc::InsecureChannelCredentials()
        )
    );
	ft_obj->handle = static_cast<FastTextHandle>(client);
}
/* }}} */

/* {{{ proto void fasttext::__destruct()
 */
PHP_METHOD(fasttext, __destruct)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);

    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
	delete client;
}
/* }}} */

/* {{{ proto long fasttext::load(String filename)
 */
PHP_METHOD(fasttext, load)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *model;
	size_t model_len;
	zend_long res;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &model, &model_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);

    res = client->LoadModel(std::string(model));

	RETURN_LONG(res);
}
/* }}} */

/* {{{ proto long fasttext::getWordRows()
 */
PHP_METHOD(fasttext, getWordRows)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	zend_long res;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    res = client->WordRows();

	RETURN_LONG(res);
}
/* }}} */

/* {{{ proto long fasttext::getWordRows()
 */
PHP_METHOD(fasttext, getLabelRows)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	zend_long res;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    res = client->LabelRows();

	RETURN_LONG(res);
}
/* }}} */

/* {{{ proto long fasttext::getWordId(String word)
 */
PHP_METHOD(fasttext, getWordId)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;
	zend_long id;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    id = client->WordID(std::string(word));

	RETURN_LONG(id);
}
/* }}} */

/* {{{ proto long fasttext::getSubwordId(String word)
 */
PHP_METHOD(fasttext, getSubwordId)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;
	zend_long id;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    id = client->SubwordID(std::string(word));

	RETURN_LONG(id);
}
/* }}} */

/* {{{ proto mixed fasttext::getWord(int id])
 */
PHP_METHOD(fasttext, getWord)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	zend_long id;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::string word = client->Word(id);

	ZVAL_STRING(return_value, word.c_str());
}
/* }}} */

/* {{{ proto mixed fasttext::getLabel(int id])
 */
PHP_METHOD(fasttext, getLabel)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	zend_long id;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::string label = client->Label(id);

	ZVAL_STRING(return_value, label.c_str());
}
/* }}} */

/* {{{ proto mixed fasttext::getWordVectors(String word)
 */
PHP_METHOD(fasttext, getWordVectors)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::vector<float> vec = client->WordVectors(std::string(word));

	array_init(return_value);
    zend_ulong idx = 0;
    for (auto &value : vec) {
        zval rowVal;
        ZVAL_DOUBLE(&rowVal, value);
        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getSubwordVector(String word)
 */
PHP_METHOD(fasttext, getSubwordVector)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::vector<float> vec = client->SubwordVectors(std::string(word));

	array_init(return_value);
    zend_ulong idx = 0;
    for (auto &value : vec) {
        zval rowVal;
        ZVAL_DOUBLE(&rowVal, value);
        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getSentenceVectors(String sentence)
 */
PHP_METHOD(fasttext, getSentenceVectors)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *sentence;
	size_t sentence_len;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &sentence, &sentence_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::vector<float> vec = client->SentenceVectors(std::string(sentence));

	array_init(return_value);
    zend_ulong idx = 0;
    for (auto &value : vec) {
        zval rowVal;
        ZVAL_DOUBLE(&rowVal, value);
        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getPredict(String word[, int k])
 */
PHP_METHOD(fasttext, getPredict)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;
	zend_long k = 0;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::vector<croco::fTextClient::sPredict> res = client->Predict(std::string(word), k);

	array_init(return_value);
    zend_ulong idx = 0;
    for (auto &node : res) {
        zval rowVal, probVal, labelVal;

        array_init(&rowVal);
        ZVAL_DOUBLE(&probVal, node.prob);
        ZVAL_STRING(&labelVal, node.label.c_str());
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "prob", sizeof("prob")-1, &probVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "label", sizeof("label")-1, &labelVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getNgrams(String word)
 */
PHP_METHOD(fasttext, getNgrams)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::vector<croco::fTextClient::sNgram> res = client->Ngrams(std::string(word));

	array_init(return_value);
    zend_ulong idx = 0;
    for (auto &node : res) {
        zval rowVal, vecsVal, vecVal, wordVal;

        array_init(&vecsVal);
        zend_ulong vidx = 0;
        for (auto &value : node.vectors) {
            zval vecVal;
            ZVAL_DOUBLE(&vecVal, value);
            add_index_zval(&vecsVal, vidx, &vecVal);
            vidx++;
        }
        ZVAL_STRING(&wordVal, node.word.c_str());

        array_init(&rowVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "vectors", sizeof("vectors")-1, &vecsVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "word", sizeof("word")-1, &wordVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */


/* {{{ proto mixed fasttext::getNN(String word[, int k])
 */
PHP_METHOD(fasttext, getNN)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;
	zend_long k = 0;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::vector<croco::fTextClient::sScore> res = client->NN(std::string(word), k);

	array_init(return_value);
    zend_ulong idx = 0;
    for (auto &node : res) {
        zval rowVal, scoreVal, labelVal;

        array_init(&rowVal);
        ZVAL_DOUBLE(&scoreVal, node.score);
        ZVAL_STRING(&labelVal, node.label.c_str());
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "score", sizeof("score")-1, &scoreVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "label", sizeof("label")-1, &labelVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getAnalogies(String word[, int k])
 */
PHP_METHOD(fasttext, getAnalogies)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;
	zend_long k = 0;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
    croco::fTextClient *client = static_cast<croco::fTextClient*>(ft_obj->handle);
    std::vector<croco::fTextClient::sScore> res = client->NN(std::string(word), k);

	array_init(return_value);
    zend_ulong idx = 0;
    for (auto &node : res) {
        zval rowVal, scoreVal, labelVal;

        array_init(&rowVal);
        ZVAL_DOUBLE(&scoreVal, node.score);
        ZVAL_STRING(&labelVal, node.label.c_str());
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "score", sizeof("score")-1, &scoreVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "label", sizeof("label")-1, &labelVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */
