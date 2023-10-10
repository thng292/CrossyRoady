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

// // Standard color find on the internet
// var consoleColorMap = [...]color.RGBA{
// 	(color.RGBA{12, 12, 12, 1}),
// 	(color.RGBA{0, 55, 218, 1}),
// 	(color.RGBA{19, 161, 14, 1}),
// 	(color.RGBA{58, 150, 221, 1}),
// 	(color.RGBA{197, 15, 31, 1}),
// 	(color.RGBA{136, 23, 152, 1}),
// 	(color.RGBA{193, 156, 0, 1}),
// 	(color.RGBA{204, 204, 204, 1}),
// 	(color.RGBA{118, 118, 118, 1}),
// 	(color.RGBA{59, 120, 255, 1}),
// 	(color.RGBA{22, 198, 12, 1}),
// 	(color.RGBA{97, 214, 214, 1}),
// 	(color.RGBA{231, 72, 86, 1}),
// 	(color.RGBA{180, 0, 158, 1}),
// 	(color.RGBA{249, 241, 165, 1}),
// 	(color.RGBA{242, 242, 242, 1}),
// 	(color.RGBA{0, 0, 0, 0}),
// }

// Standard console color
var consoleColorMap = [...]LabColor{
	ToLabColor(color.RGBA{12, 12, 12, 1}),
	ToLabColor(color.RGBA{0, 55, 218, 1}),
	ToLabColor(color.RGBA{19, 161, 14, 1}),
	ToLabColor(color.RGBA{58, 150, 221, 1}),
	ToLabColor(color.RGBA{197, 15, 31, 1}),
	ToLabColor(color.RGBA{136, 23, 152, 1}),
	ToLabColor(color.RGBA{193, 156, 0, 1}),
	ToLabColor(color.RGBA{204, 204, 204, 1}),
	ToLabColor(color.RGBA{118, 118, 118, 1}),
	ToLabColor(color.RGBA{59, 120, 255, 1}),
	ToLabColor(color.RGBA{22, 198, 12, 1}),
	ToLabColor(color.RGBA{97, 214, 214, 1}),
	ToLabColor(color.RGBA{231, 72, 86, 1}),
	ToLabColor(color.RGBA{180, 0, 158, 1}),
	ToLabColor(color.RGBA{249, 241, 165, 1}),
	ToLabColor(color.RGBA{242, 242, 242, 1}),
	ToLabColor(color.RGBA{0, 0, 0, 0}),
}

func (c ConsoleColor) ToLab() LabColor {
	return consoleColorMap[c]
}

func ToConsoleColor(rgb color.RGBA) ConsoleColor {
	if rgb.A == 0 {
		return C_TRANSPARENT
	}
	lab := ToLabColor(rgb)
	var minDis float32 = math.MaxFloat32
	res := BLACK
	for idx, co := range consoleColorMap {
		if idx == 16 {
			break
		}
		tmp := Distance(co, lab)
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
var algo = "Lanczos3"
var outputExtention = ".sprite"
var resizeAlgo = resize.Lanczos3

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
			r, g, b, a := color.RGBAModel.Convert(img.At(bb.Min.X+j, bb.Min.Y+i)).RGBA()
			tmp := byte(ToConsoleColor(color.RGBA{
				uint8(r),
				uint8(g),
				uint8(b),
				uint8(a),
			}))
			if tmp == byte(C_TRANSPARENT) {
				tmp = 31
			}
			buffer[2+i*cols+j] = tmp
		}
	}
	// fmt.Println(buffer)
	written, err := w.Write(buffer)
	if err != nil {
		panic(err)
	}
	fmt.Printf("Written %v bytes\n", written)
}

func Distance(c1, c2 LabColor) float32 {
	Sqr := func(x float32) float32 {
		return x * x
	}
	return Sqr(c1.a-c2.a) * Sqr(c1.b-c2.b) * Sqr(c1.L-c2.L)
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
	case "Bicubic":
		resizeAlgo = resize.Bicubic
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

type LabColor struct {
	L, a, b float32
}

func ToLabColor(c color.RGBA) LabColor {

	f := func(t float32) float32 {
		sigmol3 := float32(math.Pow(float64(6)/29, 3))
		if t > sigmol3 {
			return float32(math.Pow(float64(t), float64(1)/3))
		} else {
			return t/(float32(3*36)/(29*29)) + float32(4)/29
		}
	}

	X := float32(c.R)*0.4124 + float32(c.G)*0.3576 + float32(c.B)*0.1805
	Y := float32(c.R)*0.2126 + float32(c.G)*0.7152 + float32(c.B)*0.0722
	Z := float32(c.R)*0.0193 + float32(c.G)*0.1192 + float32(c.B)*0.9505

	var res LabColor
	res.L = 116*f(Y/100) - 16
	res.a = 500 * (f(X/95.0489) - f(Y/100))
	res.b = 200 * (f(Y/100) - f(Z/108.8840))
	return res
}
