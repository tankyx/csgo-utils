package main

import ("fmt"
        "os/exec"
        "syscall"
        "bytes"
        "csgo-util/routine")

func isProcRunning(name string) (bool) {
  if len(name) == 0 {
      return false
  }

  cmd := exec.Command("tasklist.exe", "/fo", "csv", "/nh")
  cmd.SysProcAttr = &syscall.SysProcAttr{HideWindow: true}
  out, err := cmd.Output()
  if err != nil {
      return false
  }

  if bytes.Contains(out, []byte(name)) {
      return true
  }
  return false
}

func main() {
  fmt.Printf("Waiting for Counter Strike to be launched ... Dummy = %d", routine.ReturnDummyValue())
  for !isProcRunning("csgo.exe") {}
  fmt.Printf("\a") //ring the bell
}
