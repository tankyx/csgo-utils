package main

import (
	"csgo-util/go-ps"
	"fmt"
	"log"
)

var OFFSET_ENTITY_LIST uintptr =0x04A1D3A4 //location of entity list in memory
var ENTITY_SEPERATION=0x10 //16-bytes between each entities in entity list

type playerStruct struct {
	og1 [3]float32 // origines - 0x0134

	t1, t2 int32 // 0xF0 : 0xF4 1 -> Spec, 2 -> T, 3 -> CT

	hp    int32 //0xFC
	flags int32 //0x0100  0 - Jump, 1 - Stay, 7 - Sitting
}

func getLocalPlayer(proc ps.Process) *playerStruct {
	var pStruct = new(playerStruct)
	return pStruct
}

func Start() int {
	var i int

	processes, err := ps.Processes()

	if err != nil {
		log.Fatal(err)
	}
	for _, proc := range processes {
		if proc.Executable() == "csgo.exe" {
			fmt.Printf("%s\n", proc.Executable())
			fmt.Printf("%d\n", proc.Pid())
			fmt.Printf("%d\n", proc.ReadMemoryAt(OFFSET_ENTITY_LIST))
			break
		}
	}
	fmt.Scanf("%d", &i)
	return i
}
