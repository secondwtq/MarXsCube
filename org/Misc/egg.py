from setuptools import setup, find_packages

setup(
	name = "numpy",
	version = "0",
	packages = find_packages('numpy', exclude = ['mutagen']),
	include_package_data = True
)