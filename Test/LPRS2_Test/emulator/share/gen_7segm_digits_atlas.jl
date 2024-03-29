#!/usr/bin/env julia

using Printf

png_scale = 2

background_color = "grey"

digit_w = 32
digit_h = 64

atlas_w = 16*digit_w
atlas_h =  2*digit_h # Two colors.

file_name = "7segm_digit_atlas"

open("$file_name.svg", "w") do of
	p(args...) = println(of, args...)
	p("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
	p("<!DOCTYPE svg>")
	p("<svg version=\"1.1\" width=\"$atlas_w\" height=\"$atlas_h\">")

	function gen_digit(color, digit, x, y)
		pix_x = x*digit_w
		pix_y = y*digit_h
		
		p("<g transform=\"translate($pix_x, $pix_y)\">")
		l(d) = p("<path fill=\"$color\" d=\"$d\"/>")
		if digit in [0, 2, 3, 5, 6, 7, 8, 9, 10, 12, 14, 15] # segm a
			l("M5 3  L8 0  L23 0  L26 3  L23 6  L8 6  Z")
		end
		if digit in [0, 1, 2, 3, 4, 7, 8, 9, 10, 13] # segm b
			l("M28 5  L31 8  L31 27  L28 30  L25 27  L25 8  Z")
		end
		if digit in [0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13] # segm c
			l("M28 57  L31 54  L31 35  L28 32  L25 35  L25 54  Z")
		end
		if digit in [0, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14] # segm d
			l("M5 59  L8 62  L23 62  L26 59  L23 56  L8 56  Z")
		end
		if digit in [0, 2, 6, 8, 10, 11, 12, 13, 14, 15] # segm e
			l("M3 57  L6 54  L6 35  L3 32  L0 35  L0 54  Z")
		end
		if digit in [0, 4, 5, 6, 8, 9, 10, 11, 12, 14, 15] # segm f
			l("M3 5  L6 8  L6 27  L3 30  L0 27  L0 8  Z")
		end
		if digit in [2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 15] # segm g
			l("M5 31  L8 28  L23 28  L26 31  L23 34  L8 34  Z")
		end
		p("</g>")
	end
	
	
	function gen_digit_set(color, row)
		for d in 0:15
			gen_digit(color, d, d, row)
		end
	end

	gen_digit_set("#FF0000", 0) # red
	gen_digit_set("#00CC00", 1) # green
	
	p("</svg>")
end


png_w = atlas_w*png_scale
png_h = atlas_h*png_scale

run(Cmd([
	"inkscape",
	"--without-gui",
	"--export-width=$png_w",
	"--export-height=$png_h",
	"--export-background=$background_color",
	"$file_name.svg",
	"--export-png=$file_name.png",
]))
run(`rm $file_name.svg`)
run(`eog $file_name.png`)
