package main

import (
	"bytes"
	"fmt"
	"os/exec"
	"syscall"
)

func isProcRunning(name string) bool {
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
	fmt.Printf("Waiting for Counter Strike to be launched ...")

	for !isProcRunning("csgo.exe") {
	}
	fmt.Printf("\a") //ring the bell
	Start()
}
