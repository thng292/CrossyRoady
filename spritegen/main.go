package main

import (
	"encoding/binary"
	"fmt"
	"image"
	"image/color"
	_ "image/gif"
	_ "image/jpeg"
	_ "image/png"
	"math"
	"os"
	"strconv"
	"strings"
	"time"
	"unicode"

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
var filePalette = ""
var width int = 0
var height int = 0
var algo = "Lanczos3"
var outputExtention = ".sprite"
var resizeAlgo = resize.Lanczos3
var mode = "single"
var fps uint8 = 0

func main() {
	if len(os.Args) == 1 || os.Args[1] == "-h" {
		// Print help message
		fmt.Println("Usage: spritegen <filename|dir> [options]")
		fmt.Println("Options:")
		fmt.Println("  -o <filename|dir>   Output filename")
		fmt.Println("  -m <mode>           Mode: dir | ani | single(default) | watch")
		fmt.Println("  -fps <mode>         Animation's FPS")
		fmt.Println("  -c <filename>       Color palette file")
		fmt.Println("  -w <width>          Width of the sprite")
		fmt.Println("  -h <height>         Height of the sprite")
		fmt.Println("  -a <algorithm>      Algorithm to use for resizing")
		fmt.Println("NearestNeighbor, Bilinear, Bicubic(default), MitchellNetravali, Lanczos2, Lanczos3")
		return
	}

	GetConfig()

	if filePalette != "" {
		LoadPalette(filePalette)
	}

	switch mode {
	case "single":
		ResizeImage(filenameIn, filenameOut)
	case "watch":
		fmt.Printf("Watching %v and outputing at %v.\n", filenameIn, filenameOut)
		ResizeImage(filenameIn, filenameOut)
		notifi := make(chan bool)
		go WatchFile(filenameIn, notifi)
		for <-notifi {
			time.Sleep(time.Millisecond * 500)
			ResizeImage(filenameIn, filenameOut)
		}
	case "dir":
		dir, err := os.ReadDir(filenameIn)
		if err != nil {
			panic(err)
		}
		_ = os.Mkdir(filenameOut, os.ModeDir)
		outfolder := "./" + strings.TrimFunc(filenameOut, IsOk) + "/"
		infolder := "./" + strings.TrimFunc(filenameIn, IsOk) + "/"
		for _, file := range dir {
			outfile := outfolder + file.Name()[:strings.LastIndex(file.Name(), ".")] + outputExtention
			infile := infolder + file.Name()
			ResizeImage(infile, outfile)
		}
	case "ani":
		dir, err := os.ReadDir(filenameIn)
		if err != nil {
			panic(err)
		}
		outFile, err := os.Create("out.anisprite")
		if err != nil {
			panic(err)
		}
		defer outFile.Close()

		folder := "./" + strings.TrimFunc(filenameIn, IsOk) + "/"
		tmpImg, err := os.Open(folder + dir[0].Name())
		if err != nil {
			panic(err)
		}
		img, _, err := image.Decode(tmpImg)
		if err != nil {
			panic(err)
		}
		imageRatio := float64(img.Bounds().Dx()) / float64(img.Bounds().Dy())
		// width / height
		if width <= 0 {
			width = int(float64(height) * imageRatio)
		}
		if height <= 0 {
			height = int(float64(width) / imageRatio)
		}
		tmpImg.Close()

		meta := make([]byte, 9)
		binary.BigEndian.PutUint16(meta, uint16(width))
		binary.BigEndian.PutUint16(meta[2:], uint16(height))
		binary.BigEndian.PutUint32(meta[4:], uint32(len(dir)))
		meta[8] = fps
		outFile.Write(meta)

		for index, file := range dir {
			fmt.Printf("%v - %.2f%%\n", file.Name(), float32(index)/float32(len(dir))*100)
			tmpImg, _ = os.Open(folder + file.Name())
			img, _, _ = image.Decode(tmpImg)
			resizedImg := resize.Resize(uint(width), uint(height), img, resizeAlgo)
			outFile.Write(Myformat_Encode(resizedImg)[4:])
			tmpImg.Close()
		}
	default:
		panic("Unknown mode")
	}
}

func IsOk(r rune) bool {
	return !(unicode.IsLetter(r) || unicode.IsDigit(r))
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

func ResizeImage(infile, outfile string) {
	fmt.Printf("[%v] %v -> %v\n", time.Now().Format(time.TimeOnly), infile, outfile)
	imgFile, err := os.Open(infile)
	if err != nil {
		panic(err)
	}
	defer imgFile.Close()
	outFile, err := os.Create(outfile)
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
		width = int(float64(height) * imageRatio)
	}
	if height <= 0 {
		height = int(float64(width) / imageRatio)
	}

	resizedImg := resize.Resize(uint(width), uint(height), img, resizeAlgo)
	outFile.Write(Myformat_Encode(resizedImg))
}

func Myformat_Encode(img image.Image) []byte {
	buffer := make([]byte, int(width)*int(height)+4)
	img.ColorModel()
	binary.BigEndian.PutUint16(buffer, uint16(width))
	binary.BigEndian.PutUint16(buffer[2:], uint16(height))
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
			buffer[4+i*cols+j] = tmp
		}
	}
	return buffer
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
		if arg == "-w" {
			tmp, _ := strconv.Atoi(os.Args[index+1])
			width = int(tmp)
		}
		if arg == "-fps" {
			tmp, _ := strconv.Atoi(os.Args[index+1])
			if tmp <= 0 || tmp > 255 {
				panic("Bad FPS value")
			}
			fps = uint8(tmp)
		}
		if arg == "-h" {
			tmp, _ := strconv.Atoi(os.Args[index+1])
			height = int(tmp)
		}
		if arg == "-a" {
			algo = os.Args[index+1]
		}
		if arg == "-c" {
			filePalette = os.Args[index+1]
		}
		if arg == "-m" {
			mode = os.Args[index+1]
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
		filenameOut = filenameIn[:strings.LastIndex(filenameIn, ".")] + outputExtention
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

func LoadPalette(filename string) {
	file, err := os.Open(filename)
	if err != nil {
		panic(err)
	}
	buff := make([]byte, 16*3)
	file.Read(buff)
	for i := 0; i < 16*3; i += 3 {
		consoleColorMap[i/3] = ToLabColor(color.RGBA{buff[i], buff[i+1], buff[i+2], 1})
	}
}
