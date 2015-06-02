package main

import ("fmt"
        "os/exec"
        "syscall"
        "bytes"
        "log"
        "csgo-util/routine"
        "csgo-util/go-ps")

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
  var ok int

  fmt.Printf("Waiting for Counter Strike to be launched ... Dummy %d\n", routine.ReturnDummyValue())

  for !isProcRunning("firefox.exe") {}
  fmt.Printf("\a") //ring the bell
  processes, err := ps.Processes()

  if err != nil {
    log.Fatal(err)
  }
  for _,proc := range processes {
    fmt.Printf("%s - %d\n", proc.Executable(), proc.ReadMemoryAt(0))
  }

  fmt.Scanf("%d", &ok)
}
