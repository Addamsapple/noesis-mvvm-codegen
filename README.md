# noesis-mvvm-codegen

Code generation tools for [NoesisGUI](https://www.noesisengine.com/).

## Features
- **XML class definitions** - models and view models are declared as simple XML elements.
- **Jinja source templates** - code is generated from customizable Jinja templates.
- **MVVM synchronization** - model state is always reflected by view models.

## Requirements

### General
- NoesisGUI Native SDK
- C++17
- Python
- CMake
- ClangFormat

### Python
- Jinja2
- lxml
- case-converter
- f-yeah

## Architecture

### Core
Basic functionality is provided by a few core classes.
- `Model` - an observable base model.
- `ModelCollection` - a templated observable collection.
- `ViewModel` - a base view model with model synchronization interfaces.
- `ViewModelCollection` - the `ViewModel` counterpart to `ModelCollection`.

These are implemented in terms of a few helper classes, including a custom single-threaded `SharedPtr`.

### Codegen
Models and view models are described in **XML**, and converted to source files by a **Python** script that renders **Jinja** templates. These outputs are processed by **ClangFormat**, to ensure consistent formatting.

Templates have full access to the XML structure after it is converted to a Python `dict`, e.g.
```xml
<A b="B">
    <C d="D"/>
    <E/>
</A>
```
is supplied to Jinja as
```json
{
    "tag": "A",
    "props": {
        "b": "B"
    },
    "children": [
        {
            "tag": "C",
            "props": {
                "d": "D"
            },
            "children": []
        },
        {
            "tag": "E",
            "props": {},
            "children": []
        }
    ]
}
```

## Testing
Test suites `core` and `codegen` can be built and run via
```bash
cmake -B build && cd build && cmake --build . && ctest
```
after which some example models and view models can be found in `tests/codegen/generated`.

> [!NOTE]
> A (Linux) version of the NoesisGUI Native SDK is already packaged for internal use.

## Usage

### Integration
...

### Codegen
...

## TODO
See [TODO.md](TODO.md).
