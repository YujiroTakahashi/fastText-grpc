#pragma once

#include <map>
#include <fstream>
#include <mutex>
#include <string>


namespace croco {

/**
 * 設定ファイル読み込みクラス
 *
 * @package     Croco
 * @author      Yujiro Takahashi <yujiro@cro-co.co.jp>
 */
class config {
public:
    typedef std::map<std::string, std::string> values_t;

private:
    values_t _values;
    bool _loaded;

public:
    bool load(const char* filename);
    bool loaded();
    const char* get(std::string key);
    const int value(std::string key);

private:
    std::string _strReplace(std::string search, std::string replace, std::string subject);
    void _parse(std::string subject);

}; // class config


/**
 * 設定ファイル読み込み
 *
 * @access public
 * @param  char* filename
 * @return bool
 */
inline bool config::load(const char* filename)
{
    std::ifstream ifs(filename);
    std::string line;

    _loaded = false;

    while (std::getline(ifs, line)) {
        if (line[0] == '[' || line[0] == '#' || line[0] == ';' || line.empty()) {
            continue;
        }

        line = _strReplace("\t", "", line);
        line = _strReplace(" ", "", line);

        _parse(line);
    }

    return _loaded = true;
}

/**
 * 設定ファイル読み込み済みチェック
 *
 * @access public
 * @return bool
 */
inline bool config::loaded()
{
    return _loaded;
}

/**
 * データの取得
 *
 * @access public
 * @param  string key
 * @return string
 */
inline const char* config::get(std::string key)
{
    return _values[key].c_str();
}

/**
 * Int型数値の取得
 *
 * @access public
 * @param  string key
 * @return int
 */
inline const int config::value(std::string key)
{
    return std::atoi(_values[key].c_str());
}

/**
 * 置換処理
 *
 * @access private
 * @param  string search
 * @param  string replace
 * @param  string subject
 * @return string
 */
inline std::string config::_strReplace(std::string search, std::string replace, std::string subject)
{
    std::string::size_type  pos(subject.find(search));

    while (pos != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos = subject.find(search, pos + replace.length());
    }

    return subject;
}

/**
 * キーと値の格納
 *
 * @access private
 * @param  string subject
 * @return void
 */
inline void config::_parse(std::string subject)
{
    std::string::size_type  pos(subject.find("="));

    if (pos != std::string::npos) {
        _values[subject.substr(0, pos)] = subject.substr(pos + 1);
    }
}


} // namespace croco::logger
