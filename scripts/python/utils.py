import sys
import os
import winreg

import requests
import time
import urllib

from zipfile import ZipFile


def get_system_environment_variable(name):
	key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, r"System\CurrentControlSet\Control\Session Manager\Environment")
	try:
		return winreg.QueryValueEx(key, name)[0]
	except:
		return None


def get_user_environment_variable(name):
	key = winreg.CreateKey(winreg.HKEY_CURRENT_USER, r"Environment")
	try:
		return winreg.QueryValueEx(key, name)[0]
	except:
		return None


def download_file(url, filepath):
	path = filepath
	filepath = os.path.abspath(filepath)
	os.makedirs(os.path.dirname(filepath), exist_ok=True)

	if (type(url) is list):
		for url_option in url:
			print("Downloading", url_option)
			try:
				download_file(url_option, filepath)
				return
			except urllib.error.URLError as e:
				print(f"URL Error encountered: {e.reason}. Proceeding with backup...\n\n")
				os.remove(filepath)
				pass
			except urllib.error.HTTPError as e:
				print(f"HTTP Error encountered: {e.code}. Proceeding with backup...\n\n")
				os.remove(filepath)
				pass
			except:
				print(f"Something went wrong. Proceeding with backup...\n\n")
				os.remove(filepath)
				pass
		raise ValueError(f"Failed to download {filepath}")

	if not(type(url) is str):
		raise TypeError("Argument 'url' must be of type list or string")

	with open(filepath, 'wb') as f:
		headers = {'User-Agent': "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36"}
		response = requests.get(url, headers=headers, stream=True)
		total = response.headers.get('content-length')

		if total is None:
			f.write(response.content)
		else:
			downloaded = 0
			total = int(total)
			start_time = time.time()

			for data in response.iter_content(chunk_size=max(int(total/1000), 1024*1024)):
				downloaded += len(data)
				f.write(data)
				done = int(50 * downloaded / total)
				percentage = (downloaded / total) * 100
				elapsed_time = time.time() - start_time
				avg_kb_per_second = (downloaded / 1024) / elapsed_time
				avg_speed_string = '{:.2f} KB/s'.format(avg_kb_per_second)
				if (avg_kb_per_second > 1024):
					avg_mb_per_second = avg_kb_per_second / 1024
					avg_speed_string = '{:.2f} MB/s'.format(avg_mb_per_second)
				sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, avg_speed_string))
				sys.stdout.flush()

	sys.stdout.write('\n')


def UnzipFile(filepath, delete_zip_file=True):
	zip_filepath = os.path.abspath(filepath) # get full path of files
	zip_file_location = os.path.dirname(zip_filepath)

	zip_file_content = dict()
	zip_file_content_size = 0

	with ZipFile(zip_filepath, 'r') as zip_file_folder:
		for name in zip_file_folder.namelist():
			zip_file_content[name] = zip_file_folder.getinfo(name).file_size

		zip_file_content_size = sum(zip_file_content.values())
		extracted_content_size = 0
		start_time = time.time()

		for zipped_file_name, zipped_file_size in zip_file_content.items():
			unzipped_file_path = os.path.abspath(f"{zip_file_location}/{zipped_file_name}")
			os.makedirs(os.path.dirname(unzipped_file_path), exist_ok=True)

			if os.path.isfile(unzipped_file_path):
				zip_file_content_size -= zipped_file_size
			else:
				zip_file_folder.extract(zipped_file_name, path=zip_file_location, pwd=None)
				extracted_content_size += zipped_file_size

			try:
				done = int(50 * extracted_content_size / zip_file_content_size)
				percentage = (extracted_content_size / zip_file_content_size) * 100
			except ZeroDivisionError:
				done = 50
				percentage = 100
				elapsed_time = time.time() - start_time

			try:
				avg_kb_per_second = (extracted_content_size / 1024) / elapsed_time
			except ZeroDivisionError:
				avg_kb_per_second = 0.0

			avg_speed_string = '{:.2f} KB/s'.format(avg_kb_per_second)
			if (avg_kb_per_second > 1024):
				avg_mb_per_second = avg_kb_per_second / 1024
				avg_speed_string = '{:.2f} MB/s'.format(avg_mb_per_second)
			sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, avg_speed_string))
			sys.stdout.flush()

	sys.stdout.write('\n')

	if delete_zip_file:
		os.remove(zip_filepath) # delete zip file
