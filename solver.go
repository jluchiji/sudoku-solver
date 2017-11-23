package main
import (
	"math/bits"
	"fmt"
	"os"
	"strconv"
)

type State struct {
	solved   int
	grid     [81]uint8
	rows     [9]uint16
	blocks   [9]uint16
	columns  [9]uint16
}

func NewState() *State {
	return &State{}
}

func (this *State) Clone() *State {
	clone := NewState()
	clone.solved = this.solved
	copy(clone.grid[:], this.grid[:])
	copy(clone.rows[:], this.rows[:])
	copy(clone.blocks[:], this.blocks[:])
	copy(clone.columns[:], this.columns[:])
	return clone;
}

func (this *State) Candidates(i uint8) uint16 {
	if (this.grid[i] != 0) { return 0 }
	return ^(this.rows[i / 9] | this.columns[i % 9] | this.blocks[(i / 27) * 3 + (i % 9) / 3]) & 0x1FF
}

func (this *State) Move(i uint8, move uint8) *State {
	if (this.Candidates(i) & (1 << (move - 1)) == 0) { panic("Invalid Move") }
	
	this.solved += 1
	this.grid[i] = move
	this.rows[i / 9] |= 1 << (move - 1)
	this.columns[i % 9] |= 1 << (move - 1)
	this.blocks[(i / 27) * 3 + (i % 9) / 3] |= 1 << (move - 1)
	return this;
}

func (this *State) Solve() *State {
	if (this.solved == 81) { return this }

	min := 9
	var next, z uint8 = 0, 0
	for z = 0; z < 81; z++ {
		len := bits.OnesCount16(this.Candidates(z))
		if (len > 0 && len < min) {
			next, min = z, len
		}
	}

	var j uint8
	for j = 0; j < 9; j++ {
		if (this.Candidates(next) & (1 << j) == 0) { continue; }
		result := this.Clone().Move(next, j + 1).Solve()
		if (result != nil) { return result }
	}
	return nil
}

func (this *State) Print() {
	for i := 0; i < 27; i++ {
		chunk := this.grid[i * 3:i * 3 + 3]
		if (i != 0 && i % 9 == 0) {
			fmt.Println("-------+-------+-------");
		}
		fmt.Printf(" %d %d %d ", chunk[0], chunk[1], chunk[2])
		if ((i + 1) % 3 != 0) {
			fmt.Printf("|");
		} else {
			fmt.Printf("\n")
		}
	}
}

func main() {
	if (len(os.Args) < 2) {
		fmt.Printf("Usage:\n\tsolver <puzzle>")
		return
	}
	input := os.Args[1]

	state := NewState();
	for i, v := range input {
		value := uint(v - '0')
		if (v == '.') { value = 0; }
		if (value == 0) { continue }
		if (value <= 9) { state.Move(uint8(i), uint8(value)) }
		if (value > 9) {
			fmt.Printf("Unexpected character: %c\n", v)
			return
		}
	}
	state.Solve().Print()
}
