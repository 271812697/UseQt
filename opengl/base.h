#ifndef BASE_H
#define BASE_H
#include <memory>
#include<log.h>


//qDebug() << "\033[32m" <<"Hello!";
//#define CORE_ERROR(InFo)  qDebug()<<"\033[31m"<<InFo<<"\033[0m";;
//#define CORE_INFO(InFo)  qDebug()<<"\033[32m"<<InFo<<"\033[0m";

//#define CORE_WARN(InFo)  qDebug()<<"\033[34m"<<InFo<<"\033[0m";
//#define CORE_DEBUG(InFo) qDebug()<<"\033[35m"<<InFo<<"\033[0m";
//#define CORE_TRACE(InFo) qDebug()<<"\033[32m"<<InFo<<"\033[0m";
template<typename T>
inline constexpr bool const_false = false;

template<typename T> using asset_ref = std::shared_ptr<T>;
template<typename T> using asset_tmp = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr asset_ref<T> MakeAsset(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr asset_tmp<T> WrapAsset(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif // BASE_H
