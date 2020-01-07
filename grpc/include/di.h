#pragma once

#include <cassert>
#include <mutex>
#include <string>

namespace croco {

/**
 * Di後処理クラス
 *
 * @package     Croco
 * @author      Yujiro Takahashi <yujiro@cro-co.co.jp>
 */
class DiFinalizer {
public:
    typedef void(*FinalizerFunc)();

public:
    static void addFinalizer(FinalizerFunc func);
    static void finalize();

}; // class DiFinalizer

const  int kMaxFinalizersSize = 256;
static int gNumFinalizersSize = 0;
static DiFinalizer::FinalizerFunc gFinalizers[kMaxFinalizersSize];

/**
 * Diクラス
 *
 * @package     Croco
 * @author      Yujiro Takahashi <yujiro@cro-co.co.jp>
 */
template <typename T>
class Di final {
public:
    static std::once_flag initFlag;
    static T* instance;

public:
    static T& get()
    {
        std::call_once(initFlag, create);
        assert(instance);
        return *instance;
    }

private:
    static void create() {
        instance = new T;
        DiFinalizer::addFinalizer(&Di<T>::destroy);
    }

    static void destroy() {
        delete instance;
        instance = nullptr;
    }
}; // class Di final

/**
 * インスタンス（開放用）の追加
 *
 * @access public
 * @param  FinalizerFunc func
 * @return void
 */
inline void DiFinalizer::addFinalizer(FinalizerFunc func)
{
    assert(gNumFinalizersSize < kMaxFinalizersSize);
    gFinalizers[gNumFinalizersSize++] = func;
}

/**
 * 格納インスタンスの開放
 *
 * @access public
 * @return void
 */
inline void DiFinalizer::finalize()
{
    for (int i = gNumFinalizersSize - 1; i >= 0; --i) {
        (*gFinalizers[i])();
    }
    gNumFinalizersSize = 0;
}


template <typename T> std::once_flag Di<T>::initFlag;
template <typename T> T* Di<T>::instance = nullptr;

} // namespace croco
