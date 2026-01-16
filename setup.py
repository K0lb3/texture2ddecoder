import os
import platform
import sys

from setuptools import Extension, setup
from wheel.bdist_wheel import bdist_wheel

USE_LIMITED_API = sys.version_info >= (3, 11) and os.getenv("CIBUILDWHEEL") is not None


class bdist_wheel_abi3(bdist_wheel):
    def get_tag(self):
        python, abi, plat = super().get_tag()

        if python.startswith("cp") and USE_LIMITED_API:
            # on CPython, our wheels are abi3 and compatible back to 3.11
            return "cp311", "abi3", plat

        return python, abi, plat


def get_extra_compile_args():
    system = platform.system()
    if system == "Windows":
        return ["/std:c++17"]
    elif system == "Darwin":
        return ["-std=c++17"]
    else:
        return ["-std=c++17", "-fms-extensions"]


setup(
    name="texture2ddecoder",
    packages=["texture2ddecoder"],
    package_data={"texture2ddecoder": ["*.py", "*.pyi", "py.typed"]},
    ext_modules=[
        Extension(
            "texture2ddecoder._texture2ddecoder",
            [
                os.path.join(root, f)
                for root, _dirs, files in os.walk("src")
                for f in files
                if f[-3:] in ["cpp", "cxx"]
                and f not in ["Texture2DDecoder.cpp", "AssemblyInfo.cpp"]
            ],
            language="c++",
            include_dirs=["src/Texture2DDecoder"],
            extra_compile_args=get_extra_compile_args(),
            py_limited_api=USE_LIMITED_API,
            define_macros=[
                ("Py_LIMITED_API", "0x030B0000"),
            ]
            if USE_LIMITED_API
            else [],
        )
    ],
    cmdclass={"bdist_wheel": bdist_wheel_abi3},
)
