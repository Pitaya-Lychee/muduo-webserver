#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_


//该类的目的是禁止对象被复制

namespace pitaya{

class NonCopyAble {
 protected:
  NonCopyAble() {}
  ~NonCopyAble() {}
 
 private:
  NonCopyAble(const NonCopyAble&) = delete;
  NonCopyAble& operator=(const NonCopyAble&) = delete;
};

} 
/*
该类的派生类可以使用默认的构造函数和析构函数，但无法进行复制操作，
即无法通过拷贝构造函数或赋值运算符将一个对象的值复制给另一个对象。
这样可以防止意外的对象复制行为，从而确保代码的正确性和安全性
*/



#endif
