# TOML To Makefile

TOML To Makefile (`toml2make`) is a tool written in C to convert `cproject.toml`
files to Makefiles for C/C++ projects.

It's structured after the Cargo package manager for Rust, with the
`cproject.toml` intentionally resembling `Cargo.toml`.

TOML (Tom's Obvious, Minimal Language) is a simple configuration file format for
structured data, while Makefiles are a text-based format for specifying
compilation and installation instructions for the `make` tool, typically for
C/C++ projects.

While Makefiles are very good at what they do, they can be a pain to write from
scratch sometimes.

TOML To Makefile aims to alleviate this by leveraging the TOML format
popularised by Rust, allowing C/C++ programmers to use a clearer and simpler
build configuration for their projects.

## Future Plans

### Program

* Support for project creation (`toml2make --new`) and initialisation (`toml2make --init`)
* Verbose output (`toml2make -v`/`toml2make --verbose`)
* Dependency management (like Cargo)
* Proper Windows support
* Integration with other tools (such as CMake)
  * Possibly export to CMakeLists.txt

### `cproject.toml` Schema

* Backwards compatibility
* Custom `make` targets other than the usual `all`, `clean`, and `install`
* Optionally split `[compiler]` into `[compiler.debug]` and `[compiler.release]`

### Extra

* Submission to package repositories (Fedora, Debian, Arch AUR, Homebrew)
* AppImage package
* Windows installer

## Installation

To compile, simply run `make` in the root directory of the project.

You can use `make install` to install TOML To Makefile to your `$HOME/.local/bin` directory.

To install the tool system-wide, run `make install prefix=/usr/local`.

## Usage

### Guided

In an empty directory, run `toml2make --init` to initialise a new project.

You'll be asked the following questions:

* **What's the name of your project?** Input the short name of your project in
  `hyphen-case` format. Default is `my-c-project`.
* **What license are you using for your project?** Input the SPDX identifier of
  the licence you're using for the project. Default is `ISC`.
* **Give a short description of your project.** Default is `A sample C project`.
* **Who's the author of this project?** If this is a solo project, just input
  your name. If there are multiple authors, input their names, separated by
  commas. Default is `Your Name`.
* **Is this a C or C++ project?** Input either `C` or `C++`. This question will
  be used to determine which compiler, standard library and file extension to
  use. Default is `C`.

When you've answered every question, a `cproject.toml` file will be created for
you, along with a basic project structure, including a README.md.
Open `cproject.toml` up in your favourite text editor and edit it to fit your
project's exact requirements.

When you're done editing `cproject.toml`, run the following commands:

```bash
git init  # Initialise a new Git repository
toml2make # Convert the `cproject.toml` file to a Makefile
make      # Build the C project
```

By default, the Makefile is added to your project's `.gitignore`. If you want to
track the Makefile, remove the `Makefile` entry **BEFORE** you make your first
commit.

### Manually

Create a file named `cproject.toml` in the root directory of your project.

This should be called `cproject.toml` - *not* `toml2make.toml`. or `Make.toml`
or anything else.

Populate the file with data, like so:

```toml
[toml2make.schema]
version = "0.5.1"

[project]
name = "your-project-name"
version = "0.1.0"
license = "ISC"
description = "A short description of your project."
author = "Your Name"

[compiler]
cc = "gcc" # Change to "g++" for C++ projects
cflags = "-Wall"
cversion = "c11" # Change to "c++11" for C++ projects; change to "gnu11` for GNU C (not ISO C) projects

[options]
file_extension = "c" # Change to "cpp" for C++ projects
use_include = true
```

When the file is created, create your source directory at `src`. Then, run
the following commands:

```bash
git init  # Initialise a new Git repository
toml2make # Convert the `cproject.toml` file to a Makefile
make      # Build the C project
```

If you wish, you can add `Makefile` to your `.gitignore` file, since `toml2make`
will generate a `Makefile` from your `cproject.toml` anyway.

## Schema

TOML To Makefile's `cproject.json` schema is present inside the
[`schema/0.x.x/cproject-0.5.1.toml` file](https://github.com/AeriaVelocity/toml2make/blob/main/schema/0.x.x/cproject-0.5.1.toml).

For a full list of schema versions, see [this project's `schema/` directory.](https://github.com/AeriaVelocity/toml2make/tree/main/schema)

Schema versions before v1.0.0 will not support backwards compatibility due to
immaturity, but backwards compatibility of schema versions past v1.0.0 is
planned.

## Licence

TOML To Makefile is dual-licensed under the
[MIT](https://github.com/AeriaVelocity/toml2make/blob/main/LICENSE.MIT) and
[GNU GPL 3.0](https://github.com/AeriaVelocity/toml2make/blob/main/LICENSE.GPL-3.0)
licences.
