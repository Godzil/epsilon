#include <escher/run_loop.h>
#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

RunLoop::RunLoop() :
  m_time(0) {
}

int RunLoop::numberOfTimers() {
  return 0;
}

Timer * RunLoop::timerAtIndex(int i) {
  assert(false);
  return nullptr;
}

void RunLoop::run() {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg([](void * ctx){ ((Container *)ctx)->step(); }, this, 0, 1);
#else
  while (true) {
    step();
  }
#endif
}

void RunLoop::step() {
  // Fetch the event, if any
  int eventDuration = Timer::TickDuration;
  int timeout = eventDuration;
  Ion::Events::Event event = Ion::Events::getEvent(&timeout);
  eventDuration -= timeout;

  assert(eventDuration >= 0);
  assert(eventDuration <= Timer::TickDuration);

  /* At this point, eventDuration contains the time it took to retrieve the
   * event. It can be zero, and is at most equal to the timeout value, Timer::
   * TickDuration.  The event returned can be None if nothing worth taking care
   * of happened. In other words, getEvent is a blocking call with a timeout. */

  m_time += eventDuration;

  if (m_time >= Timer::TickDuration) {
    m_time -= Timer::TickDuration;
    for (int i=0; i<numberOfTimers(); i++) {
      Timer * timer = timerAtIndex(i);
      timer->tick();
    }
  }

#if ESCHER_LOG_EVENTS_BINARY
  char message[2] = { (char)event.id(), 0};
  ion_log_string(message);
#endif
#if ESCHER_LOG_EVENTS_NAME
  const char * name = event.name();
  if (name == nullptr) {
    name = "UNDEFINED";
  }
  ion_log_string("Ion::Events::");
  ion_log_string(name);
  ion_log_string("\n");
#endif

  if (event != Ion::Events::None) {
    dispatchEvent(event);
  }
}