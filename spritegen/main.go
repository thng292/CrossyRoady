package main

import (
	"fmt"
	"image"
	"image/color"
	_ "image/gif"
	_ "image/jpeg"
	_ "image/png"
	"io"
	"math"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/nfnt/resize"
)

type ConsoleColor uint8

const (
	BLACK ConsoleColor = iota
	BLUE
	GREEN
	CYAN
	RED
	MAGENTA
	YELLOW
	WHITE
	GRAY
	LIGHT_BLUE
	LIGHT_GREEN
	LIGHT_CYAN
	LIGHT_RED
	LIGHT_MAGENTA
	LIGHT_YELLOW
	BRIGHT_WHITE
	C_TRANSPARENT
)

var consoleColorMap = [...]color.RGBA{
	{12, 12, 12, 1},
	{12, 12, 12, 1},
	{19, 161, 14, 1},
	{58, 150, 221, 1},
	{197, 15, 31, 1},
	{136, 23, 152, 1},
	{193, 156, 0, 1},
	{204, 204, 204, 1},
	{118, 118, 118, 1},
	{59, 120, 255, 1},
	{22, 198, 12, 1},
	{97, 214, 214, 1},
	{231, 72, 86, 1},
	{180, 0, 158, 1},
	{249, 241, 165, 1},
	{242, 242, 242, 1},
	{0, 0, 0, 0},
}

func (c ConsoleColor) ToRGBA() color.RGBA {
	return consoleColorMap[c]
}

func ToConsoleColor(rgb color.RGBA) ConsoleColor {
	if rgb.A == 0 {
		return C_TRANSPARENT
	}
	minDis := math.Inf(1)
	res := BLACK
	for idx, co := range consoleColorMap {
		if co.A == 0 {
			break
		}
		tmp := Distance(co, rgb)
		if tmp < minDis {
			minDis = tmp
			res = ConsoleColor(idx)
		}
	}
	return res

}

var filenameIn = ""
var filenameOut = ""
var width int8 = 0
var height int8 = 0
var watch = false
var algo = "Bicubic"
var outputExtention = ".sprite"
var resizeAlgo = resize.Bicubic

func main() {
	if len(os.Args) == 1 || os.Args[1] == "-h" {
		// Print help message
		fmt.Println("Usage: spritegen <filename> [options]")
		fmt.Println("Options:")
		fmt.Println("  -o <filename>   Output filename")
		fmt.Println("  -l              Watch for changes")
		fmt.Println("  -w <width>      Width of the sprite")
		fmt.Println("  -h <height>     Height of the sprite")
		fmt.Println("  -a <algorithm>  Algorithm to use for resizing")
		fmt.Println("                  NearestNeighbor, Bilinear, Bicubic(default), MitchellNetravali, Lanczos2, Lanczos3")
		return
	}

	GetConfig()

	if watch {
		fmt.Printf("Watching %v and outputing at %v.\n", filenameIn, filenameOut)
		ResizeImage()
		notifi := make(chan bool)
		go WatchFile(filenameIn, notifi)
		for <-notifi {
			time.Sleep(time.Millisecond * 500)
			ResizeImage()
		}
	} else {
		ResizeImage()
	}

}

func WatchFile(filePath string, notifi chan<- bool) {
	initialStat, err := os.Stat(filePath)
	if err != nil {
		panic(err)
	}

	for {
		stat, err := os.Stat(filePath)
		if err != nil {
			panic(err)
		}

		if stat.Size() != initialStat.Size() || stat.ModTime() != initialStat.ModTime() {
			initialStat = stat
			notifi <- true
		}

		time.Sleep(1 * time.Second)
	}
}

func ResizeImage() {
	fmt.Printf("[%v] Outputing...\n", time.Now().Format(time.TimeOnly))
	imgFile, err := os.Open(filenameIn)
	if err != nil {
		panic(err)
	}
	defer imgFile.Close()
	outFile, err := os.Create(filenameOut)
	if err != nil {
		panic(err)
	}
	defer outFile.Close()

	img, _, err := image.Decode(imgFile)
	if err != nil {
		panic(err)
	}

	imageRatio := float64(img.Bounds().Dx()) / float64(img.Bounds().Dy())
	// width / height
	if width <= 0 {
		width = int8(float64(height) * imageRatio)
	}
	if height <= 0 {
		height = int8(float64(width) / imageRatio)
	}

	resizedImg := resize.Resize(uint(width), uint(height), img, resizeAlgo)
	Myformat_Encode(outFile, resizedImg)
}

func Myformat_Encode(w io.Writer, img image.Image) {
	buffer := make([]byte, int(width)*int(height)+2)
	img.ColorModel()
	buffer[0] = byte(width)
	buffer[1] = byte(height)
	bb := img.Bounds()
	rows := bb.Dy()
	cols := bb.Dx()
	for i := 0; i < rows; i++ {
		for j := 0; j < cols; j++ {
			r, g, b, a := img.At(bb.Min.X+j, bb.Min.Y+i).RGBA()
			buffer[2+i*cols+j] = byte(ToConsoleColor(color.RGBA{
				uint8(r),
				uint8(g),
				uint8(b),
				uint8(a),
			}))
		}
	}
	// fmt.Println(buffer)
	written, err := w.Write(buffer)
	if err != nil {
		panic(err)
	}
	fmt.Printf("Written %v bytes\n", written)
}

func Distance(c1 color.RGBA, c2 color.RGBA) float64 {
	y1 := 0.299*float64(c1.R) + 0.587*float64(c1.G) + 0.114*float64(c1.B)
	u1 := -0.14713*float64(c1.R) - 0.28886*float64(c1.G) + 0.436*float64(c1.B)
	v1 := 0.615*float64(c1.R) - 0.51499*float64(c1.G) - 0.10001*float64(c1.B)

	y2 := 0.299*float64(c2.R) + 0.587*float64(c2.G) + 0.114*float64(c2.B)
	u2 := -0.14713*float64(c2.R) - 0.28886*float64(c2.G) + 0.436*float64(c2.B)
	v2 := 0.615*float64(c2.R) - 0.51499*float64(c2.G) - 0.10001*float64(c2.B)

	delta_y := y1 - y2
	delta_u := u1 - u2
	delta_v := v1 - v2

	return math.Sqrt(delta_y*delta_y + delta_u*delta_u + delta_v*delta_v)
}

func GetConfig() {
	filenameIn = os.Args[1]
	for index, arg := range os.Args {
		if arg == "-o" {
			filenameOut = os.Args[index+1]
		}
		if arg == "-l" {
			watch = true
		}
		if arg == "-w" {
			tmp, _ := strconv.Atoi(os.Args[index+1])
			width = int8(tmp)
		}
		if arg == "-h" {
			tmp, _ := strconv.Atoi(os.Args[index+1])
			height = int8(tmp)
		}
		if arg == "-a" {
			algo = os.Args[index+1]
		}
	}
	if height <= 0 && width <= 0 {
		panic("Please provide width or height using -w or -h flag")
	}

	resizeAlgo = resize.Bicubic
	switch algo {
	case "NearestNeighbor":
		resizeAlgo = resize.NearestNeighbor
	case "Bilinear":
		resizeAlgo = resize.Bilinear
	case "MitchellNetravali":
		resizeAlgo = resize.MitchellNetravali
	case "Lanczos2":
		resizeAlgo = resize.Lanczos2
	case "Lanczos3":
		resizeAlgo = resize.Lanczos2
	}

	if filenameOut == "" {
		filenameOut = filenameIn[:strings.LastIndex(filenameIn, ".")] + "_" + algo + outputExtention
	}
}
