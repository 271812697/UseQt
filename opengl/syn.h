#ifndef SYN_H
#define SYN_H

#include"pch.h"
class Sync {
   private:
     GLuint id;
     GLsync sync = nullptr;

   public:
     Sync(GLuint id);
     ~Sync();

     Sync(const Sync&) = delete;
     Sync(Sync&& other) = delete;
     Sync& operator=(const Sync&) = delete;
     Sync& operator=(Sync&& other) = delete;

     bool Signaled();
     void ClientWaitSync(GLuint64 timeout = 1e5);  // 0.1 ms
     void ServerWaitSync();

     static GLint64 GetServerTimeout();
     static void WaitFlush();
     static void WaitFinish();
 };


#endif // SYN_H
