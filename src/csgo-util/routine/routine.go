package routine

import ("csgo-util/offsets"
        "github.com/mitchellh/go-ps")

func ReturnDummyValue() int {
  return offsets.GetDummyValue()
}
