from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class LibGPSRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def layout(self) -> None:
        cmake_layout(self)

    def build_requirements(self) -> None:
        self.tool_requires("cmake/4.4.0")
        self.test_requires("catch2/3.15.2")

    def build(self) -> None:
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
