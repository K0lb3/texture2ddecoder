from setuptools import setup, Extension, find_packages
import os

with open("README.md", "r") as fh:
    long_description = fh.read()

setup(
    name="texture2ddecoder",
    description="a python wrapper for Perfare's Texture2DDecoder",
    author="K0lb3",
    version="1.0.0",
    keywords=["astc", "atc", "pvrtc", "etc", "crunch", "dxt", "bcn", "eacr"],
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Intended Audience :: Developers",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Topic :: Multimedia :: Graphics",
    ],
    url="https://github.com/K0lb3/texture2ddecoder",
    download_url="https://github.com/K0lb3/texture2ddecoder/tarball/master",
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=[
        Extension(
            "texture2ddecoder",
            [
                os.path.join(root, f)
                for root, dirs, files in os.walk("src")
                for f in files
                if f[-3:] in ["cpp", "cxx"]
                and f not in ["Texture2DDecoder.cpp", "AssemblyInfo.cpp"]
            ],
            language="c++",
            include_dirs=["src/Texture2DDecoder"],
            extra_compile_args=["-std=c++11"],
        )
    ],
)
