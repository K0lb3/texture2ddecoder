from setuptools import setup, Extension
import os
import platform
from setuptools.command.build_ext import build_ext
from wheel.bdist_wheel import bdist_wheel


class bdist_wheel_abi3(bdist_wheel):
    def get_tag(self):
        python, abi, plat = super().get_tag()

        if python.startswith("cp"):
            # on CPython, our wheels are abi3 and compatible back to 3.7
            return "cp37", "abi3", plat

        return python, abi, plat


def get_extra_compile_args():
    system = platform.system()
    if system == "Windows":
        return ["/std:c++14"]
    elif system == "Darwin":
        return ["-std=c++11"]
    else:
        return ["-std=c++11", "-fms-extensions"]


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
            py_limited_api=True,
            define_macros=[
                ("Py_LIMITED_API", "0x03070000"),
            ],
        )
    ],
    cmdclass={"bdist_wheel": bdist_wheel_abi3},
)
