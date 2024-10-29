# TOML To Makefile

TOML To Makefile (`toml2make`) is a tool written in C to convert `cproject.toml`
files to Makefiles for C/C++ projects.

## Why?

Because:

* I'm a Rust fanboy.
* I really like TOML.
* I really *don't* like writing a properly-structured Makefile every time.
* I really *really* don't like CMake.

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
> including a `cproject.toml` for you with the `toml2make --init` command.

Populate the file with data, like so:

```toml
[toml2make.schema]
version = "0.5.0"

[project]
name = "your-project-name"
version = "0.1.0"
license = "ISC"
description = "A short description of your project."
author = "Your Name"

[compiler]
cc = "gcc"
# Change to "g++" for C++ projects
cflags = "-Wall"
cversion = "c11"
# Change to "c++11" for C++ projects; change to "gnu11` for GNU C (not ISO C) projects

[options]
file_extension = "c"
# Change to "cpp" for C++ projects
use_include = true
```

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

## Schema

TOML To Makefile's schema is present at [this own project's `cproject.toml` file.](https://github.com/AeriaVelocity/toml2make/blob/main/cproject.toml)

For a full list of schema versions, see [this project's `schema/` directory.](https://github.com/AeriaVelocity/toml2make/tree/main/schema)

Schema versions before v1.0.0 will not support backwards compatibility due to
immaturity, but backwards compatibility of schema versions past v1.0.0 is
planned.

(I have no idea what "schema" means but based on my experience with JSON, I have
a pretty good idea)

## Licence

TOML To Makefile is distributed under the [MIT License](LICENSE).
