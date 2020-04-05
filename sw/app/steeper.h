#ifndef APP_STEEPER_H
#define APP_STEEPER_H

#include <cstdint>

#include <array>

#include "app/steeper_internal.h"
#include "system/teatotler.h"
#include "util/periodic_task.h"

namespace application {

class Steeper : public util::PeriodicTask {
 public:
  Steeper(sys::Teatotler* teatotler);
  virtual ~Steeper() = default;

 private:
  void DoRunIteration(uint32_t time_ms) override;

  steeper_internal::SteepParameters steep_parameters_;
  steeper_internal::SteepState state_;

  steeper_internal::SetSteepTime set_steep_time_state_;
  steeper_internal::SetDunkCount set_dunk_count_state_;
  //steeper_internal::Steep steep_state_;
  //steeper_internal::SteepComplete steep_complete_state_;
};

}  // namespace application

#endif /* APP_STEEPER_H */
