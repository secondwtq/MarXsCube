from png import Reader, Writer
from numpy import concatenate

#	140426 EVE

SOURCE_NAME = 'Test'
DEST_NAME = 'TestDest.png'
EMPTY_FILE = 'Test 0001.png'
IMAGE_NUM = 45
DEST_ROWS = 7
DEST_COLS = 7

def getSrcFileName(idx):
	return ('%s %04d%s' % (SOURCE_NAME, idx, '.png'))

if __name__ == '__main__':
	print("\nPNGMerger by seCOnDatkE 2014.")
	print("This script is initially written for MarKsCube Prototype.")
	print("Any problem and suggestion goes to wtqs1ra2@gmail.com.")
	print('')

	print("\tReading %s..." % getSrcFileName(1)),
	imageMeta = Reader(getSrcFileName(1)).read()
	print('ok'); print ("\tGetting file size..."),
	size = (imageMeta[0], imageMeta[1])
	print('ok, %s: width %d, height %d.' % (getSrcFileName(1), size[0], size[1])); print('\tCreating png writer...'),
	destWriter = Writer(size[0] * DEST_ROWS, size[1] * DEST_COLS, alpha='RGBA', bitdepth=8)
	print('ok, width %d, height %d.' % (size[0] * DEST_ROWS, size[1] * DEST_COLS))

	src_images = []
	print("\n\t\tReading empty image data..."),
	src_images.append([data for data in Reader(EMPTY_FILE).asDirect()[2]])
	print('ok')
	for num in range(1, IMAGE_NUM+1):
		print('\t\tReading image data "%s"...' % getSrcFileName(num)),
		src_images.append([data for data in Reader(getSrcFileName(num)).asDirect()[2]])
		print('ok')

	ret_image = []
	getImage = (lambda x : src_images[(lambda : 0 if x > IMAGE_NUM else x)()])
	print('')
	for row in range(1, DEST_COLS * DEST_ROWS, DEST_ROWS):
		print("\t\tGenerating row %d..." % ((row-1) / DEST_ROWS + 1)),
		image_first = getImage(row)
		for col in range(0, size[0]):
			data = image_first[col]
			for col_image in range(row+1, row + DEST_ROWS):
				data = concatenate((data, getImage(col_image)[col]))
			ret_image.append(data)
		print('ok')

	print("\n\tOpening %s..." % DEST_NAME),
	DEST = open(DEST_NAME, 'wb')
	print('ok'); print("\tWriting file..."),
	destWriter.write(DEST, ret_image)
	print('ok')
