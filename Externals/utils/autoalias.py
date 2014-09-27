import Image
import os

PATH_AA = r'/Volumes/Second Datke/Transfer/speeder_aa'
PATH_NOAA = r'/Volumes/Second Datke/Transfer/speeder_noaa'
IMAGE_NUM = 36
SOURCE_NAME = 'taradr_a'

def sepalpha(image_rgb, image_alpha):
	def _split (image, mode='RGBA'):
		return image.convert(mode).split()
	s_aa, s_noaa = _split(image_rgb), _split(image_alpha)
	bands = list(s_aa[0:3])
	bands.append(s_noaa[3])
	n_image = Image.merge('RGBA',  bands)
	return n_image

def getimagename(source, idx, ext='png'):
	return ('%s %04d.%s' % (source, idx, ext))

if __name__ == '__main__':
	names_aa = [os.path.join(PATH_AA, getimagename(SOURCE_NAME, i)) for i in xrange(0, IMAGE_NUM+1)]
	names_noaa = [os.path.join(PATH_NOAA, getimagename(SOURCE_NAME, i)) for i in xrange(0, IMAGE_NUM+1)]

	files_aa, files_noaa = map(Image.open, names_aa), map(Image.open, names_noaa)

	rets = map(sepalpha, files_aa, files_noaa)

	white_image = Image.new('RGB', files_aa[0].size, 'rgb(0, 0, 255)')
	rets = map(lambda x: Image.composite(x, white_image, x), rets)

	for i, image in enumerate(rets):
		image.save(os.path.join('test/', getimagename(SOURCE_NAME, i)))
	