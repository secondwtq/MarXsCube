#coding=utf-8

EXEC = 'luajit'
SOURCE_EXT = '.lua'
OUTPUT_EXT = '.o'
PATH_SOURCE = '.'
PATH_OUTPUT = '.'

import os
import os.path
import subprocess

def merge_path_single(root, files):
	return [os.path.join(root, filename) for filename in files]

def list_files(srcpath):
	ret = []
	for root, dirs, files in os.walk(srcpath):
		ret.extend(merge_path_single(root, filter(
			lambda filename: os.path.splitext(filename)[-1].lower() == SOURCE_EXT, files)))
	return ret

def pair_scripts(source):
	return [(srcfile, os.path.splitext(os.path.basename(srcfile))[-2]+OUTPUT_EXT)
				for srcfile in source]

def main():
	file_pairs = pair_scripts(list_files(PATH_SOURCE))
	for source, output in file_pairs:
		output_path = os.path.join(PATH_OUTPUT, output)
		command = '%s -b %s %s' % (EXEC, source, output_path)
		print(command)
		p = subprocess.Popen(command, stdin=subprocess.PIPE, 
							stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
		print(p.stdout.read())

	print('Done.')


if __name__ == '__main__':
	status = main()
