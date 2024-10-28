# TOML To Makefile

TOML To Makefile is a tool written in C to convert `cproject.toml` files to
Makefiles for C/C++ projects.

## Why?

Because:

* I'm a Rust fanboy.
* I really like TOML.
* I really *don't* like writing a properly-structured Makefile every time.
* I really *really* don't like CMake.

## Example `cproject.toml` Format

```toml
[toml2make]
# This version number is for the toml2make SCHEMA, NOT toml2make itself.
version = "0.4.0"

[project]
name = "c-project"
version = "0.1.0"
license = "MIT"
description = "A project written in C."
author = "Some Guy"

[compiler]
# If you're building a C++ project instead, replace `gcc` with `g++` and `c17` with `c++17`.
cc = "gcc"
cflags = "-Wall -Wextra -Wpedantic"
cversion = "c17"

[build]
install = "~/.local/bin"
build = "build"
src = "src"
bin = "bin"

[makefile]
out = "Makefile"
```

## Installation

To compile, simply run `make` in the root directory of the project.

You can use `make install` to install TOML To Makefile to your `$HOME/.local/bin` directory.

To install the tool system-wide, run `make install prefix=/usr/local`.

## Usage

Create a file named `cproject.toml` in the root directory of your project.

This should be called `cproject.toml` - *not* `toml2make.toml`. or `Make.toml`
or anything else.

> [!NOTE]
> In a future version, this tool will be able to generate a project structure
> including a `cproject.toml` for you with the `toml2make init` command.

Populate the file with data, like so:

```toml
[toml2make]
version = "0.3.0"

[project]
name = "your-project-name"
version = "0.1.0"
license = "ISC" # or whatever licence you're using
description = "A short description of your project."
author = "Your Name"

[compiler]
cc = "gcc"
cflags = "-Wall"
cversion = "c11"
```

For a more complete sample, refer to the [example above.](#example-cprojecttoml-format)

When the file is created, create your source directory at `src`. Then, run
the following commands:

```bash
toml2make # Convert the `cproject.toml` file to a Makefile
make      # Build the C project
```

If you wish, you can add `Makefile` to your `.gitignore` file, since `toml2make`
will generate a `Makefile` from your `cproject.toml` anyway.

## Options

> [!WARNING]
> These are not implemented yet. Do not use them expecting them to work until
> at least toml2make version 1.0.0 is released.

* `-b`/`--build`: Build the C project automatically.
* `-r`/`--run`: Build and run the C project automatically.
* `-c`/`--clean`: Remove an existing Makefile and all generated files before generating the Makefile.
  Combine with `-b` or `-r` to remove the Makefile after building/running.
* `-v`/`--verbose`: Enable verbose output.
* `--version`: Print the TOML To Makefile version.

For example, `toml2make -rcv` will automatically build and run the C project and remove the Makefile when done, all with verbose output.

## Licence

TOML To Makefile is distributed under the [MIT License](LICENSE).
