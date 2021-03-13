#ifndef NCURSESPLUSPLUS_SYSTEM_DETAIL_EVENT_QUEUE_HPP
#define NCURSESPLUSPLUS_SYSTEM_DETAIL_EVENT_QUEUE_HPP
#include <memory>
#include <mutex>
#include <set>
#include <utility>
#include <variant>
#include <vector>

#include "ncursesplusplus/common/lockable.hpp"
#include "ncursesplusplus/system/event.hpp"
#include "ncursesplusplus/system/system.hpp"
#include "ncursesplusplus/widget/widget.hpp"

#include "absl/synchronization/mutex.h"
#include "absl/types/variant.h"

namespace npp {
inline auto operator<(PaintEvent const &x, PaintEvent const &y) -> bool {
  return std::addressof(x.receiver.get()) < std::addressof(y.receiver.get());
}
}  // namespace npp

namespace npp {
namespace detail {

class PaintQueue {
 public:
  void append(PaintEvent event) {
    absl::MutexLock ml(&mutex_);
    paints_.insert(event);
  }

  void send_all() {
    absl::MutexLock ml(&mutex_);
    for (auto &paint : paints_)
      System::send_event(paint);
    paints_.clear();
  }

 private:
  std::set<PaintEvent> paints_;
  absl::Mutex mutex_;
};

class DeleteQueue {
 public:
  void append(DeleteEvent e) {
    absl::MutexLock ml(&mutex_);
    deletes_.push_back(std::move(e));
  }

  void send_all() {
    absl::MutexLock ml(&mutex_);
    for (auto &d : deletes_)
      System::send_event(std::move(d));
    deletes_.clear();
  }

 private:
  std::vector<DeleteEvent> deletes_;
  absl::Mutex mutex_;
};

// Mutex/Threading Notes
// The main thread is the only thread that can call EventQueue::SendAll()
// Events are removed from the queue which a lock, then processed without a lock
// This does not require a recursive mutex because event is processed w/o lock.
// Paint and Delete Events should not be accessing functions that access the
// queue, so they can remain locked while processing.

class BasicQueue {
 public:
  void append(Event e) {

    basics_.push_back(std::move(e));
  }

  void send_all() {
    // Allows for send(e) appending to the queue and invalidating iterators.
    for (auto index = this->get_begin_index(); index != -1uL;
         index = this->increment_index(index)) {
      auto event = this->get_event(index);
      System::send_event(std::move(event));
    }
    absl::MutexLock ml(&mutex_);
    basics_.Clear();
  }

 private:
  auto get_event(std::size_t index) -> Event {
    absl::MutexLock ml(&mutex_);
    return std::move(basics_[index]);
  }

  auto increment_index(std::size_t current) -> std::size_t {
    absl::MutexLock ml(&mutex_);
    return current + 1uL < basics_.size() ? current + 1uL : -1uL;
  }

  auto get_begin_index() -> std::size_t {
    absl::MutexLock ml(&mutex_);
    return basics_.empty() ? -1ul : 0ul;
  }
 private:
  std::vector<Event> basics_;
  absl::Mutex mutex_;

};

class EventQueue {
 public:
  explicit EventQueue()
      : helper_(this) {
  }
  /// Adds the given event with priority for the concrete event type.
  void Append(Event e) {
    absl::visit(helper_, e);
  }

  void SendAll() {
    basics_.send_all();
    paints_.send_all();
    deletes_.send_all();
  }

 private:
  struct AppendHelper {
    explicit AppendHelper(EventQueue *queue_in)
        : queue(queue_in) {
    }
    template<class E>
    void operator()(E &&e) {
      queue->template AddToQueue(std::forward<decltype(e)>(e));
    }
    EventQueue *queue;
  };

  template<typename E>
  void AddToQueue(E e) { basics_.append(std::move(e)); }
  void AddToQueue(PaintEvent e) { paints_.append(e); }
  void AddToQueue(DeleteEvent e) { deletes_.append(std::move(e)); }

  AppendHelper helper_;
  BasicQueue basics_;
  PaintQueue paints_;
  DeleteQueue deletes_;
};

}
}  // namespace npp::detail
#endif  // NCURSESPLUSPLUS_SYSTEM_DETAIL_EVENT_QUEUE_HPP
