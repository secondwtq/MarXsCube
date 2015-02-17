#coding=utf-8

EXEC = 'luajit'
SOURCE_EXT = '.lua'
OUTPUT_EXT = '.o'
PATH_SOURCE = '.'
PATH_OUTPUT = '.'

import os
import os.path
import subprocess
import argparse

parser_main = argparse.ArgumentParser(description='LuaJit automated compiler for object file to link into exectuable.')
parser_main.add_argument('-s', '--source', dest='source', help='Directory containing the source.', default='.')
parser_main.add_argument('-o', '--output', dest='output', help='Directory to storage the output files.', default='.')

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
	args = parser_main.parse_args()
	PATH_SOURCE = args.source
	PATH_OUTPUT = args.output

	file_pairs = pair_scripts(list_files(PATH_SOURCE))
	if not len(file_pairs):
		print('No script here, exiting.')
		return 0

	for source, output in file_pairs:
		output_path = os.path.join(PATH_OUTPUT, output)
		command = '%s -b %s %s' % (EXEC, source, output_path)
		print(command)
		p = subprocess.Popen(command, stdin=subprocess.PIPE, 
							stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
		print(p.stdout.read())

	print('Done.')
	return 0

if __name__ == '__main__':
	status = main()
	import sys
	sys.exit(status)
