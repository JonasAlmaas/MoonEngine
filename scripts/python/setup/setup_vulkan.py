import os
from pathlib import Path

import utils


class VulkanConfiguration:
	required_vulkan_version = "1.3."
	install_vulkan_version = "1.3.216.0"
	vulkan_directory = "../../Moon-Engine/vendor/VulkanSDK"

	@classmethod
	def validate(cls):
		if (not cls.check_vulkan_SDK()):
			print("Vulkan SDK not installed correctly.")
			return

		if (not cls.check_vulkan_SDK_debug_libs()):
			print("\nNo Vulkan SDK debug libs found. Install Vulkan SDK with debug libs.")
			print("Debug configuration disabled.")

	@classmethod
	def check_vulkan_SDK(cls):
		vulkan_SDK = os.environ.get("VULKAN_SDK")
		if (vulkan_SDK is None):
			print("\nYou don't have the Vulkan SDK installed!")
			cls.__install_vulkan_SDK()
			return False
		else:
			print(f"\nLocated Vulkan SDK at {vulkan_SDK}")

		if (cls.required_vulkan_version not in vulkan_SDK):
			print(f"You don't have the correct Vulkan SDK version! (Moon Engine requires {cls.required_vulkan_version})")
			cls.__install_vulkan_SDK()
			return False

		print(f"Correct Vulkan SDK located at {vulkan_SDK}")
		return True

	@classmethod
	def __install_vulkan_SDK(cls):
		permission_granted = False
		while not permission_granted:
			reply = str(input("Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(cls.install_vulkan_version))).lower().strip()[:1]
			if reply == 'n':
				return
			permission_granted = (reply == 'y')

		vulkan_tnstall_URL = f"https://sdk.lunarg.com/sdk/download/{cls.install_vulkan_version}/windows/VulkanSDK-{cls.install_vulkan_version}-Installer.exe"
		vulkan_path = f"{cls.vulkan_directory}/VulkanSDK-{cls.install_vulkan_version}-Installer.exe"
		print("Downloading {0:s} to {1:s}".format(vulkan_tnstall_URL, vulkan_path))
		utils.download_file(vulkan_tnstall_URL, vulkan_path)
		print("Running Vulkan SDK installer...")
		os.startfile(os.path.abspath(vulkan_path))
		print("Re-run this script after installation!")
		quit()

	@classmethod
	def check_vulkan_SDK_debug_libs(cls):
		vulkan_SDK = os.environ.get("VULKAN_SDK")
		shadercd_lib = Path(f"{vulkan_SDK}/Lib/shaderc_sharedd.lib")

		return shadercd_lib.exists()


if __name__ == "__main__":
	VulkanConfiguration.validate()
