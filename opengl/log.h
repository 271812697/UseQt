#ifndef LOG_H
#define LOG_H
#include <spdlog/spdlog.h>
#define CORE_INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define CORE_DEBUG(...) Log::GetLogger()->debug(__VA_ARGS__)
#define CORE_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define CORE_ERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define CORE_ASERT(cond, ...) { if (!(cond)) { Log::GetLogger()->critical(__VA_ARGS__); __debugbreak(); } }
class Log {
  private:
    static std::shared_ptr<spdlog::logger> logger;

  public:
    static std::shared_ptr<spdlog::logger> GetLogger() { return logger; }

  public:
    static void Init();
    static void Shutdown();
};

#endif // LOG_H
