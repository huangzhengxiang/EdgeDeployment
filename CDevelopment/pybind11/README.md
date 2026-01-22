## Pybind11

### I. Module Definition

#### 1.1 Module
In pybind11 cpp definition, it essentially define a python module.
```cpp
PYBIND11_MODULE(module_name, m) {
}
```
`module_name` → the name Python will use to import the module by:
```python
import module_name
```
`m` → a variable name you use inside the C++ code to refer to the Python module object.

#### 1.2 exported python objects
Inside that function, pybind11 builds Python objects: module, classes, functions, enums. Python stores them in the module namespace.
- classes
```cpp
pybind11::class_<PyMicroNN>(m, "MicroNN")
```
`PyMicroNN` is the underlying C class, `m` is the python module, and `"MicroNN"`, the quotion marked, is the exported name that can be referred in python.

Member functiona definition.
```cpp
pybind11::class_<PyMicroNN>(m, "MicroNN")
        .def(pybind11:::init<const std::string&, const std::vector<std::pair<std::string, MicroNNSchemaType>>&>(),
             pybind11::arg("db_file") = ":memory:",
             pybind11::arg("attribute_schema") = std::vector<std::pair<std::string, MicroNNSchemaType>>{})
        .def("insert_vector", &PyMicroNN::insertVector)
```
The parameter lists of `.def` are `function name`, function pointer, and default parameter lists.


- functions
Ues `m.def(` to substitute the class, while everything else just as the member function case.


- enums
For example, The following code defines a type of `"MicroNNSchemaType"` in python upon `MicroNNSchemaType` in cpp, which has 3 fields: `INT`, `FLOAT`, `TEXT`. And `export_values` translate these information for code.
```cpp
pybind11::enum_<MicroNNSchemaType>(m, "MicroNNSchemaType")
    .value("INT", MicroNN_Schema_Int)
    .value("FLOAT", MicroNN_Schema_Float)
    .value("TEXT", MicroNN_Schema_Text)
    .export_values();
```


#### 1.3 Types Conversion
pybind11 headers provides conversions from and to python objects and cpp classes.
```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
```

With `<pybind11/pybind11.h>`,  we bridge our interfaces. With `<pybind11/stl.h>`, we can direct interpret C functions of python. 
| C++ |	Python |
| ----- | ----- |
| std::vector |	list  |
| std::map |	dict |
| std::pair |	tuple |
| std::string |	str |
| int64_t | int |
| double  | float |

With #include `<pybind11/numpy.h>`, we can call `array_t` in our code to seemlessly process numpy ndarray.


Code Example:
```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "MicroNN.hpp"
#include <vector>
#include <string>

namespace py = pybind11;

class PyMicroNN {
public:
    std::unique_ptr<MicroNN> microNN;
    PyMicroNN(const std::string& db_file = ":memory:",
              const std::vector<std::pair<std::string, MicroNNSchemaType>>& attribute_schema = {});
    bool insertVector(int64_t partition_id, int64_t asset_id, int64_t vector_id, 
                      const std::vector<float>& vector);
    bool insertCentroid(int64_t partition_id, const std::vector<float>& centroid);
    std::vector<std::pair<int64_t, double>> knn_search(const std::vector<float>& query, int k = 1);
    std::vector<std::pair<int64_t, double>> ivf_search(const std::vector<float>& query, int k = 1, int nlists = 10);
};

PYBIND11_MODULE(pymicronn, m) {
    m.doc() = "MicroNN Python binding";

    py::enum_<MicroNNSchemaType>(m, "MicroNNSchemaType")
        .value("INT", MicroNN_Schema_Int)
        .value("FLOAT", MicroNN_Schema_Float)
        .value("TEXT", MicroNN_Schema_Text)
        .export_values();

    py::class_<PyMicroNN>(m, "MicroNN")
        .def(py::init<const std::string&, const std::vector<std::pair<std::string, MicroNNSchemaType>>&>(),
             py::arg("db_file") = ":memory:",
             py::arg("attribute_schema") = std::vector<std::pair<std::string, MicroNNSchemaType>>{})
        .def("insert_vector", &PyMicroNN::insertVector)
        .def("insert_centroid", &PyMicroNN::insertCentroid)
        .def("knn_search", &PyMicroNN::knn_search, 
             py::arg("query"), 
             py::arg("k") = 1)
        .def("ivf_search", &PyMicroNN::ivf_search,
             py::arg("query"), 
             py::arg("k") = 1, 
             py::arg("nlists") = 10);
}
```

### II. Compilation and wheel

#### 2.1 Cmake -> .so
In `CMakeLists.txt`, we shall first find `pybind11`, and use `pybind11_add_module` to add module source cpp file to the module target library (e.g., `pymicronn`, this lib module name shall be exactly **the same** as you defined in line `PYBIND11_MODULE(pymicronn, m)`).
```cmake
find_package(pybind11 REQUIRED)
if(pybind11_FOUND)
    pybind11_add_module(pymicronn micronn_pybind.cpp)
    target_link_libraries(pymicronn PRIVATE micronn)
    target_include_directories(pymicronn PRIVATE ../sqlite/sqlite-vec/vendor/)
endif()
```

When cmake, ensure that it can find pybind11. If you install it by python in virtual env, you shall use this to direct package search:
```bash
cmake -Dpybind11_DIR=$(python -m pybind11 --cmakedir) ..
```

#### 2.2 Wrap up -> .whl
whl is only for release version ready for distribution. 

After cmake compilation: `pymicronn.cpython-310-x86_64-linux-gnu.so`. This is already a valid Python module. You can do: `import pymicronn`, as long as Python can see the file. This is how CPython itself works internally.

A wheel adds: `pymicronn-0.1.0-cp310-cp310-manylinux_x86_64.whl`, which contains:
```
pymicronn/
    __init__.py
    pymicronn.cpython-310-x86_64-linux-gnu.so

pymicronn-0.1.0.dist-info/
    METADATA
    RECORD
    WHEEL
```
The .so is still there — just wrapped.


Use scikit-build-core (recommended) to wrap a whl. Write a `pyproject.toml`:
```
[build-system]
requires = ["scikit-build-core", "pybind11"]
build-backend = "scikit_build_core.build"

[project]
name = "pymicronn"
version = "0.1.0"
```

Then:
```
pip install build && python -m build
```

This will produce:
```
dist/
  pymicronn-0.1.0-cp310-cp310-manylinux_x86_64.whl
```


Internally it still runs CMake, and Python just orchestrates it. But now packaging is layered on top.
```
pip build →
  scikit-build-core →
    cmake configure →
    cmake build →
      .so →
    package into wheel
```

