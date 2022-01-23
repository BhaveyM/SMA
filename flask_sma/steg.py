from PIL import Image

# ------------------help function----------------
lookUp = ['00','01','10','11','00','01','10','11','00','01']
def makeCompleteBinary(binary):
	if(len(binary) < 8):
		binary = makeCompleteBinary('0'+binary)
	return binary

def removeZ(binary):
	if(int(binary[0]) == 0):
		binary = removeZ(binary[1:len(binary)])
	return binary

def makeNewClrs(b):
	return [b[0]+b[1],b[2]+b[3],b[4]+b[5],b[6]+b[7]]


# -------------hide and unhide function----------
def hide(filename, imgname, xt, size):
	ext = xt
	file_length = size
	if(ext == '.txt'):
		txt = open(filename, "r").read()
		txt += "\0"
		fileDataArray = ' '.join(format(ord(x), 'b') for x in txt).split(" ")
	else:
		txt = open(filename, "rb").read()
		# temp = open("bla1.txt", "w").write(str(txt))
		# temp = open("bla2.txt", "wb+").write(txt)
		fileDataArray = ' '.join(format(int(x), 'b') for x in txt).split(" ")
	
	#ext = ' '.join(format(ord(x), 'b') for x in ext).split(" ")

	# Import an image from directory & Extracting pixel map::
	input_image = Image.open(imgname).convert('RGBA')
	pixel_map = input_image.load()
	width, height = input_image.size
	count = 0
	out = 0

	if width*height > file_length:
		for i in range(width):
			if out == 1:
				break
			for j in range(height):
				if count >= file_length:
					out = 1
					break
				pair = makeNewClrs(makeCompleteBinary(fileDataArray[count]))

				#getting rgba here
				r, g, b, p = input_image.getpixel((i, j))
				r = str(r)[0:len(str(r))-1]+str(lookUp.index(str(pair[0])))
				g = str(g)[0:len(str(g))-1]+str(lookUp.index(str(pair[1])))
				b = str(b)[0:len(str(b))-1]+str(lookUp.index(str(pair[2])))
				p = str(p)[0:len(str(p))-1]+str(lookUp.index(str(pair[3])))
				pixel_map[i, j] = (int(r), int(g), int(b), int(p))
				count += 1
		print("hide complete")
		input_image.save("uploads/hidden.png", format="png") #download hidden image
		#input_image.show()

def unhide(imgname, xt, size):
	ext = xt
	file_length = size
	fileDataArray = []
	out = 0
	count = 0
	
	# Import an image from directory & Extracting pixel map::
	input_image = Image.open(imgname).convert('RGBA')
	pixel_map = input_image.load()
	width, height = input_image.size
	
	for i in range(width):
		if out == 1:
			break
		for j in range(height):
			
			if count >= file_length:
				out = 1
				break

			r, g, b, p = input_image.getpixel((i, j))
			binary = lookUp[int(str(r)[len(str(r))-1])]+lookUp[int(str(g)[len(str(g))-1])]+lookUp[int(str(b)[len(str(b))-1])]+lookUp[int(str(p)[len(str(p))-1])]
			
			if ext == ".txt":
				fileDataArray.append(chr(int(binary, 2)))
			else:
				fileDataArray.append(int(binary, 2))
			count += 1

			# if(binary == '11111111'):
			# 	print(binary)
			# 	out = 1
			# 	break;
	print("unhide complete")
	if(ext == '.txt'):
		e = open("uploads/decoded.txt", "w")
		e.write("".join(fileDataArray))
	else:
		e = open("uploads/decoded"+ext, "wb+")
		#temp = open("bla.txt", "wb+").write(bytearray(fileDataArray))
		e.write(bytearray(fileDataArray))
	e.close()
	
	

	# 		fileDataArray.append(chr(int(binary, 2)))
	# e = open("uploads/decoded.txt", "w")
	# e.write("".join(fileDataArray))
	# e.close();
	#
	

