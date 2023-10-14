package main

import (
	"bufio"
	"os"
	"strconv"
	"strings"
	"unicode"
)

var filenameIn = ""
var filenameOut = ".palette"

package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"unicode"
)

// main reads a file containing RGB color values and writes them to a binary file.
// Usage: palettegen [input file] [-o output file]
// If no output file is specified, the output file will be named [input file].palette.
// The input file should contain up to 16 lines of RGB color values in the format "R,G,B".
// Each color value should be an integer between 0 and 255.
func main() {
	var filenameIn, filenameOut string

	if len(os.Args) == 1 {
		fmt.Println("Usage: palettegen [input file] [-o output file]")
		return
	}

	filenameIn = os.Args[1]
	for index, arg := range os.Args {
		if arg == "-o" {
			filenameOut = os.Args[index+1]
		}
	}
	if filenameOut == "" {
		filenameOut = filenameIn[:strings.LastIndex(filenameIn, ".")] + ".palette"
	}

	in, err := os.Open(filenameIn)
	if err != nil {
		panic(err)
	}
	out, err := os.Create(filenameOut)
	if err != nil {
		panic(err)
	}

	trimFunc := func(r rune) bool {
		return !unicode.IsDigit(r)
	}

	scanner := bufio.NewScanner(in)
	i := 0
	for scanner.Scan() {
		if i == 16 {
			break
		}
		i++
		line := scanner.Text()
		tmp := strings.Split(line, ",")
		r, err := strconv.Atoi(strings.TrimFunc(tmp[0], trimFunc))
		if err != nil {
			panic(err)
		}
		g, err := strconv.Atoi(strings.TrimFunc(tmp[1], trimFunc))
		if err != nil {
			panic(err)
		}
		b, err := strconv.Atoi(strings.TrimFunc(tmp[2], trimFunc))
		if err != nil {
			panic(err)
		}
		out.Write([]byte{uint8(r), uint8(g), uint8(b)})
	}
}
