package routine

import (
	"csgo-util/go-ps"
	"fmt"
	"log"
)

var OFFSET_ENTITY_LIST 0x04A14B54 //location of entity list in memory
var ENTITY_SEPERATION 0x10 //16-bytes between each entities in entity list

type playerStruct struct {
	og1 [3]float32 // origines - 0x0134

	t1, t2 int32 // 0xF0 : 0xF4 1 -> Spec, 2 -> T, 3 -> CT

	hp    int32 //0xFC
	flags int32 //0x0100  0 - Jump, 1 - Stay, 7 - Sitting

}

func getLocalPlayer(proc Process) playerStruct {

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
			break
		}
	}
	fmt.Scanf("%d", &i)
	return i
}
