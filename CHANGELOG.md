# Changelog
All notable changes to this project will be documented in this file.

## Unreleased - ???
- Add `jpm rule-tree` subcommand.
- Add `--offline` flag to jpm to force use of the cache.
- Allow sending pointers and C functions across threads via `thread/send`.
- Fix bug in `getline`.
- Add `sh-rule` and `sh-phony` to jpm's dialect of Janet.
- Change C api's `janet_formatb` -> `janet_formatbv`, and add new function `janet_formatb` to C api.
- Add `edefer` macro to core.
- A struct/table literal/constructor with duplicate keys will use the last value given.
  Previously, this was inconsistent between tables and structs, literals and constructor functions.
- Add debugger to core. The debugger functions are only available
  in a debug repl, and are prefixed by a `.`.
- Add `sort-by` and `sorted-by` to core.
- Support UTF-8 escapes in strings via `\uXXXX` or `\UXXXXXX`.
- Add `math/erf`
- Add `math/erfc`
- Add `math/log1p`
- Add `math/next`
- Add os/umask
- Add os/perm-int
- Add os/perm-string
- Add :octal-permissions option for os/stat.
- Add `jpm repl` subcommand, as well as `post-deps` macro in project.janet files.
- Various bug fixes.

## 1.8.1 - 2020-03-31
- Fix bugs for big endian systems
- Fix 1.8.0 regression on BSDs

## 1.8.0 - 2020-03-29
- Add `reduce2`, `accumulate`, and `accumulate2`.
- Add lockfiles to `jpm` via `jpm make-lockfile` and `jpm load-lockfile`.
- Add `os/realpath` (Not supported on windows).
- Add `os/chmod`.
- Add `chr` macro.
- Allow `_` in the `match` macro to match anything without creating a binding
  or doing unification. Also change behavior of matching nil.
- Add `:range-to` and `:down-to` verbs in the `loop` macro.
- Fix `and` and `or` macros returning nil instead of false in some cases.
- Allow matching successfully against nil values in the `match` macro.
- Improve `janet_formatc` and `janet_panicf` formatters to be more like `string/format`.
  This makes it easier to make nice error messages from C.
- Add `signal`
- Add `fiber/can-resume?`
- Allow fiber functions to accept arguments that are passed in via `resume`.
- Make flychecking slightly less strict but more useful
- Correct arity for `next`
- Correct arity for `marshal`
- Add `flush` and `eflush`
- Add `prompt` and `return` on top of signal for user friendly delimited continuations.
- Fix bug in buffer/blit when using the offset-src argument.
- Fix segfault with malformed pegs.

## 1.7.0 - 2020-02-01
- Remove `file/fileno` and `file/fdopen`.
- Remove `==`, `not==`, `order<`, `order>`, `order<=`, and `order>=`. Instead, use the normal
  comparison and equality functions.
- Let abstract types define a hash function and comparison/equality semantics. This lets
  abstract types much better represent value types. This adds more fields to abstract types, which
  will generate warnings when compiled against other versions.
- Remove Emscripten build. Instead, use the amalgamated source code with a custom toolchain.
- Update documentation.
- Add `var-`
- Add `module/add-paths`
- Add `file/temp`
- Add `mod` function to core.
- Small bug fixes
- Allow signaling from C functions (yielding) via janet\_signalv. This
  makes it easy to write C functions that work with event loops, such as
  in libuv or embedded in a game.
- Add '%j' formatting option to the format family of functions.
- Add `defer`
- Add `assert`
- Add `when-with`
- Add `if-with`
- Add completion to the default repl based on currently defined bindings. Also generally improve
  the repl keybindings.
- Add `eachk`
- Add `eachp`
- Improve functionality of the `next` function. `next` now works on many different
  types, not just tables and structs. This allows for more generic data processing.
- Fix thread module issue where sometimes decoding a message failed.
- Fix segfault regression when macros are called with bad arity.

## 1.6.0 - 2019-12-22
- Add `thread/` module to the core.
- Allow seeding RNGs with any sequence of bytes. This provides
  a wider key space for the RNG. Exposed in C as `janet_rng_longseed`.
- Fix issue in `resume` and similar functions that could cause breakpoints to be skipped.
- Add a number of new math functions.
- Improve debugger experience and capabilities. See examples/debugger.janet
  for what an interactive debugger could look like.
- Add `debug/step` (janet\_step in the C API) for single stepping Janet bytecode.
- The built in repl now can enter the debugger on any signal (errors, yields,
  user signals, and debug signals). To enable this, type (setdyn :debug true)
  in the repl environment.
- When exiting the debugger, the fiber being debugged is resumed with the exit value
  of the debug session (the value returned by `(quit return-value)`, or nil if user typed Ctrl-D).
- `(quit)` can take an optional argument that is the return value. If a module
  contains `(quit some-value)`, the value of that module returned to `(require "somemod")`
  is the return value. This lets module writers completely customize a module without writing
  a loader.
- Add nested quasiquotation.
- Add `os/cryptorand`
- Add `prinf` and `eprinf` to be have like `printf` and `eprintf`. The latter two functions
  now including a trailing newline, like the other print functions.
- Add nan?
- Add `janet_in` to C API.
- Add `truthy?`
- Add `os/environ`
- Add `buffer/fill` and `array/fill`
- Add `array/new-filled`
- Use `(doc)` with no arguments to see available bindings and dynamic bindings.
- `jpm` will use `CC` and `AR` environment variables when compiling programs.
- Add `comptime` macro for compile time evaluation.
- Run `main` functions in scripts if they exist, just like jpm standalone binaries.
- Add `protect` macro.
- Add `root-env` to get the root environment table.
- Change marshalling protocol with regard to abstract types.
- Add `show-paths` to `jpm`.
- Add several default patterns, like `:d` and `:s+`, to PEGs.
- Update `jpm` path settings to make using `jpm` easier on non-global module trees.
- Numerous small bug fixes and usability improvements.

### 1.5.1 - 2019-11-16
- Fix bug when printing buffer to self in some edge cases.
- Fix bug with `jpm` on windows.
- Fix `update` return value.

## 1.5.0 - 2019-11-10
- `os/date` now defaults to UTC.
- Add `--test` flag to jpm to test libraries on installation.
- Add `math/rng`, `math/rng-int`, and `math/rng-uniform`.
- Add `in` function to index in a stricter manner. Conversely, `get` will
  now not throw errors on bad keys.
- Indexed types and byte sequences will now error when indexed out of range or
  with bad keys.
- Add rng functions to Janet. This also replaces the RNG behind `math/random`
  and `math/seedrandom` with a consistent, platform independent RNG.
- Add `with-vars` macro.
- Add the `quickbin` command to jpm.
- Create shell.c when making the amalgamated source. This can be compiled with
  janet.c to make the janet interpreter.
- Add `cli-main` function to the core, which invokes Janet's CLI interface.
  This basically moves what was init.janet into boot.janet.
- Improve flychecking, and fix flychecking bugs introduced in 1.4.0.
- Add `prin`, `eprint`, `eprintf` and `eprin` functions. The
  functions prefix with e print to `(dyn :err stderr)`
- Print family of functions can now also print to buffers
  (before, they could only print to files.) Output can also
  be completely disabled with `(setdyn :out false)`.
- `printf` is now a c function for optimizations in the case
  of printing to buffers.

## 1.4.0 - 2019-10-14
- Add `quit` function to exit from a repl, but not always exit the entire
  application.
- Add `update-pkgs` to jpm.
- Integrate jpm with https://github.com/janet-lang/pkgs.git. jpm can now
  install packages based on their short names in the package listing, which
  can be customized via an env variable.
- Add `varfn` macro
- Add compile time arity checking when function in function call is known.
- Added `slice` to the core library.
- The `*/slice` family of functions now can take nil as start or end to get
  the same behavior as the defaults (0 and -1) for those parameters.
- `string/` functions that take a pattern to search for will throw an error
  when receiving the empty string.
- Replace (start:end) style stacktrace source position information with
  line, column. This should be more readable for humans. Also, range information
  can be recovered by re-parsing source.

## 1.3.1 - 2019-09-21
- Fix some linking issues when creating executables with native dependencies.
- jpm now runs each test script in a new interpreter.
- Fix an issue that prevent some valid programs from compiling.
- Add `mean` to core.
- Abstract types that implement the `:+`, `:-`, `:*`, `:/`, `:>`, `:==`, `:<`,
  `:<=`, and `:>=` methods will work with the corresponding built-in
  arithmetic functions. This means built-in integer types can now be used as
  normal number values in many contexts.
- Allow (length x) on typed arrays an other abstract types that implement
  the :length method.

## 1.3.0 - 2019-09-05
- Add `get-in`, `put-in`, `update-in`, and `freeze` to core.
- Add `jpm run rule` and `jpm rules` to jpm to improve utility and discoverability of jpm.
- Remove `cook` module and move `path` module to https://github.com/janet-lang/path.git.
  The functionality in `cook` is now bundled directly in the `jpm` script.
- Add `buffer/format` and `string/format` format flags `Q` and `q` to print colored and
  non-colored single-line values, similar to `P` and `p`.
- Change default repl to print long sequences on one line and color stacktraces if color is enabled.
- Add `backmatch` pattern for PEGs.
- jpm detects if not in a Developer Command prompt on windows for a better error message.
- jpm install git submodules in dependencies
- Change default fiber stack limit to the maximum value of a 32 bit signed integer.
- Some bug fixes with `jpm`
- Fix bugs with pegs.
- Add `os/arch` to get ISA that janet was compiled for
- Add color to stacktraces via `(dyn :err-color)`

## 1.2.0 - 2019-08-08
- Add `take` and `drop` functions that are easier to use compared to the
  existing slice functions.
- Add optional default value to `get`.
- Add function literal short-hand via `|` reader macro, which maps to the
  `short-fn` macro.
- Add `int?` and `nat?` functions to the core.
- Add `(dyn :executable)` at top level to get what used to be
  `(process/args 0)`.
- Add `:linux` to platforms returned by `(os/which)`.
- Update jpm to build standalone executables. Use `declare-executable` for this.
- Add `use` macro.
- Remove `process/args` in favor of `(dyn :args)`.
- Fix bug with Nanbox implementation allowing users to created
  custom values of any type with typed array and marshal modules, which
  was unsafe.
- Add `janet_wrap_number_safe` to API, for converting numbers to Janets
  where the number could be any 64 bit, user provided bit pattern. Certain
  NaN values (which a machine will never generate as a result of a floating
  point operation) are guarded against and converted to a default NaN value.

## 1.1.0 - 2019-07-08
- Change semantics of `-l` flag to be import rather than dofile.
- Fix compiler regression in top level defs with destructuring.
- Add `table/clone`.
- Improve `jpm` tool with git and dependency capabilities, as well as better
  module uninstalls.

## 1.0.0 - 2019-07-01
- Add `with` macro for resource handling.
- Add `propagate` function so we can "rethrow" signals after they are
  intercepted. This makes signals even more flexible.
- Add `JANET_NO_DOCSTRINGS` and `JANET_NO_SOURCEMAPS` defines in janetconf.h
  for shrinking binary size.
  This seems to save about 50kB in most builds, so it's not usually worth it.
- Update module system to allow relative imports. The `:cur:` pattern
  in `module/expand-path` will expand to the directory part of the current file, or
  whatever the value of `(dyn :current-file)` is. The `:dir:` pattern gets
  the directory part of the input path name.
- Remove `:native:` pattern in `module/paths`.
- Add `module/expand-path`
- Remove `module/*syspath*` and `module/*headerpath*` in favor of dynamic
  bindings `:syspath` and `:headerpath`.
- Compiled PEGs can now be marshaled and unmarshaled.
- Change signature to `parser/state`
- Add `:until` verb to loop.
- Add `:p` flag to `fiber/new`.
- Add `file/{fdopen,fileno}` functions.
- Add `parser/clone` function.
- Add optional argument to `parser/where` to set parser byte index.
- Add optional `env` argument to `all-bindings` and `all-dynamics`.
- Add scratch memory C API functions for auto-released memory on next gc.
  Scratch memory differs from normal GCed memory as it can also be freed normally
  for better performance.
- Add API compatibility checking for modules. This will let native modules not load
  when the host program is not of a compatible version or configuration.
- Change signature of `os/execute` to be much more flexible.

## 0.6.0 - 2019-05-29
- `file/close` returns exit code when closing file opened with `file/popen`.
- Add `os/rename`
- Update windows installer to include tools like `jpm`.
- Add `jpm` tool for building and managing projects.
- Change interface to `cook` tool.
- Add optional filters to `module/paths` to further refine import methods.
- Add keyword arguments via `&keys` in parameter list.
- Add `-k` flag for flychecking source.
- Change signature to `compile` function.
- Add `module/loaders` for custom loading functions.
- Add external unification to `match` macro.
- Add static library to main build.
- Add `janet/*headerpath*` and change location of installed headers.
- Let `partition` take strings.
- Haiku OS support
- Add `string/trim`, `string/trimr`, and `string/triml`.
- Add `dofile` function.
- Numbers require at least 1 significant digit.
- `file/read` will return nil on end of file.
- Fix various bugs.

## 0.5.0 - 2019-05-09
- Fix some bugs with buffers.
- Add `trace` and `untrace` to the core library.
- Add `string/has-prefix?` and `string/has-suffix?` to string module.
- Add simple debugger to repl that activates on errors or debug signal
- Remove `*env*` and `*doc-width*`.
- Add `fiber/getenv`, `fiber/setenv`, and `dyn`, and `setdyn`.
- Add support for dynamic bindings (via the `dyn` and `setdyn` functions).
- Change signatures of some functions like `eval` which no longer takes
  an optional environment.
- Add printf function
- Make `pp` configurable with dynamic binding `:pretty-format`.
- Remove the `meta` function.
- Add `with-dyns` for blocks with dynamic bindings assigned.
- Allow leading and trailing newlines in backtick-delimited string (long strings).
  These newlines will not be included in the actual string value.

## 0.4.1 - 2019-04-14
- Squash some bugs
- Peg patterns can now make captures in any position in a grammar.
- Add color to repl output
- Add array/remove function
- Add meson build support
- Add int module for int types
- Add meson build option
- Add (break) special form and improve loop macro
- Allow abstract types to specify custom tostring method
- Extend C API for marshalling abstract types and other values
- Add functions to `os` module.

## 0.4.0 - 2019-03-08
- Fix a number of smaller bugs
- Added :export option to import and require
- Added typed arrays
- Remove `callable?`.
- Remove `tuple/append` and `tuple/prepend`, which may have seemed like `O(1)`
  operations. Instead, use the `splice` special to extend tuples.
- Add `-m` flag to main client to allow specifying where to load
  system modules from.
- Add `-c` flag to main client to allow compiling Janet modules to images.
- Add `string/format` and `buffer/format`.
- Remove `string/pretty` and `string/number`.
- `make-image` function creates pre compiled images for janet. These images
  link to the core library. They can be loaded via require or manually via
  `load-image`.
- Add bracketed tuples as tuple constructor.
- Add partition function to core library.
- Pre-compile core library into an image for faster startup.
- Add methods to parser values that mirror the api.
- Add janet\_getmethod to CAPI for easier use of method like syntax.
- Add get/set to abstract types to allow them to behave more
  like objects with methods.
- Add parser/insert to modify parser state programmatically
- Add debug/stacktrace for easy, pretty stacktraces
- Remove the status-pp function
- Update API to run-context to be much more sane
- Add :lflags option to cook/make-native
- Disallow NaNs as table or struct keys
- Update module resolution paths and format

## 0.3.0 - 2019-01-26
- Add amalgamated build to janet for easier embedding.
- Add os/date function
- Add slurp and spit to core library.
- Added this changelog.
- Added peg module (Parsing Expression Grammars)
- Move hand written documentation into website repository.
