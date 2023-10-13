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

func main() {
	filenameIn = os.Args[1]
	for index, arg := range os.Args {
		if arg == "-o" {
			filenameOut = os.Args[index+1]
		}
	}
	if filenameOut == ".palette" {
		filenameOut = filenameIn[:strings.LastIndex(filenameIn, ".")] + filenameOut
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
