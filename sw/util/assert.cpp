#include "util/assert.h"

namespace util {

void Assert(bool condition) {
  if (!condition) {
    StopExecutionHere();
  }
}

void StopExecutionHere() {
  while(1) {
  }
}

}  // util
