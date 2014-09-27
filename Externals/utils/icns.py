import Image
import os

SOURCE_FILENAME = "marxscube_icon.png"

def get_filename(size, is_2x=False):
    if not is_2x:
        return "icon_%dx%d.png" % (size, size)
    else:
        return "icon_%dx%d@2x.png" % (size, size)

if __name__ == '__main__':
    image_original = Image.open(SOURCE_FILENAME)
    image_d = { }

    name_dir = '%s.iconset' % os.path.splitext(SOURCE_FILENAME)[0]
    if not os.path.exists(name_dir):
        os.mkdir(name_dir)

    image_d[1024] = image_original
    for sz in (512, 256, 128, 64, 32, 16):
        image_d[sz] = image_original.resize((sz,sz), Image.ANTIALIAS)
    for sz_save in (512, 256, 128, 32, 16):
        print("saving %s ..." % sz_save)
        image_d[sz_save].save(os.path.join(name_dir, get_filename(sz_save, False)))
        image_d[sz_save*2].save(os.path.join(name_dir, get_filename(sz_save, True)))

    os.system("iconutil --convert icns %s" % name_dir)

    print("done!")
