#include <queue>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdio.h>
#include <atomic>

std::queue<int> jobs;
std::mutex jobs_mutex;
std::condition_variable jobs_cond;
std::vector<std::thread> threads;

std::mutex done_mutex;
std::atomic<int> done;
std::atomic<bool> kill;
int total;

void worker(int id) {
  for(;;) {
    int job;
    {
      std::unique_lock<std::mutex> lk(jobs_mutex);
      jobs_cond.wait(lk, [&]() -> bool {return jobs.size() != 0 || kill;});
      if(kill) {
        return;
      }
      job = jobs.front();
      jobs.pop();
    }

    fprintf(stderr, "working %d: %d\n", id, job); 
    usleep(1000 * 100);
    if(++done == total) {
      done_mutex.unlock();
    }
  }
}

int main() {
  int workers = 20;
  for(int i = 0; i < workers; i++) {
    threads.emplace_back(worker, i);
  }

//  sleep(1);
  done_mutex.lock();

  for(int step = 0; step < 5; step++) {

    done = 0;
    total = 100000;
    for(int i = 0; i < total; i++) {
        std::unique_lock<std::mutex> lk(jobs_mutex);
        jobs.emplace(i + 1000 * step);
        jobs_cond.notify_one();
    }

    fprintf(stderr, "waiting to finish....\n");
    done_mutex.lock();
  }

  kill = true;
  jobs_cond.notify_all();

  for(auto &t: threads) {
    t.join();
  }

}
