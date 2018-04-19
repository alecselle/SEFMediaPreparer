#ifndef WORKER_HPP
#define WORKER_HPP

#include <QFuture>
#include <QTime>

namespace SuperEpicFuntime {
enum WorkerType { SCAN = 1, ENCODE = 2, CLOSE = 0 };
class Worker {
  private:
	WorkerType type;
	QFuture<void> worker;
	QTime timeStamp;

  public:
	Worker(WorkerType type);
	void run();
};
} // namespace SuperEpicFuntime
#endif // WORKER_HPP