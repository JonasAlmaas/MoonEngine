import os
import sys
import subprocess

# Fix up the current working directory
sys.path.insert(1, os.path.abspath('.'))

from setup_python import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.validate()

# Change directory from root/scripts/python to root
os.chdir('./../../')

from setup_vulkan import VulkanConfiguration as VulkanRequirements

VulkanRequirements.validate()

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
