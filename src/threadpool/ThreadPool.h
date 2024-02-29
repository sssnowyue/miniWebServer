#pragma once

#include <functional>
#include <future>
#include <queue>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(size_t);

  // Add tasks to the thread pool
  template <class F, class... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

  ~ThreadPool();

 private:
  // queue of tasks that will be added to the thread pool
  std::queue<std::function<void()>> tasks;
  // list of threads in threadpool
  std::vector<std::thread> workers;
  // Mutex, used to ensure thread safety
  std::mutex queueMutex;
  // Condition variables, used to implement thread synchronization
  std::condition_variable condition;
  // Flag indicating whether the thread pool is stopped
  bool stop;
};

template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<decltype(f(args...))> {
  using return_type = decltype(f(args...));
  auto callableBind = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
  auto task = std::make_shared<std::packaged_task<return_type()>>(callableBind);
  std::future<return_type> res = task->get_future();

  {
    std::unique_lock<std::mutex> lock(queueMutex);
    if (stop)
      throw std::runtime_error("enqueue on stopped ThreadPool");
    auto taskFunction = [task]() {
      (*task)();
    };
    tasks.emplace(taskFunction);
  }

  condition.notify_one();

  return res;
}