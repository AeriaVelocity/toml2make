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
[project]
name = "c-project"
version = "0.1.0"
license = "MIT"
description = "A sample C project to test TOML To Makefile."

[compiler]
cc = "gcc"
cflags = [
    "-Wall",
    "-Wextra",
    "-Wpedantic",
]
cversion = "c17"

[build]
prefix = "$HOME/.local"
```

## Installation

To compile, simply run `make` in the root directory of the project.

You can use `make install` to install TOML To Makefile to your `$HOME/.local/bin` directory.

To install the tool system-wide, run `make install prefix=/usr/local`.

## Usage

```bash
toml2make # Convert the `cproject.toml` file to a Makefile
make      # Build the C project
```

To skip the additional `make` command:

```bash
toml2make -b # Convert the `cproject.toml` file to a Makefile and automatically run `make`
```

If you wish, you can add `Makefile` to your `.gitignore` file, since `toml2make`
will generate a `Makefile` from your `cproject.toml` anyway.

## Options

* `-b`/`--build`: Build the C project automatically.
* `-r`/`--run`: Build and run the C project automatically.
* `-c`/`--clean`: Remove an existing Makefile and all generated files before generating the Makefile.
  Combine with `-b` or `-r` to remove the Makefile after building/running.
* `-v`/`--verbose`: Enable verbose output.
* `--version`: Print the TOML To Makefile version.

For example, `toml2make -rcv` will automatically build and run the C project and remove the Makefile when done, all with verbose output.

## Licence

TOML To Makefile is distributed under the [MIT License](LICENSE).
