#include "util/assert.h"

namespace util {

void Assert(bool condition) {
  if (!condition) {
    while(1) {
    }
  }
}

void StopExecutionHere() {
  while(1) {
  }
}

}  // util
