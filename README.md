# adventofcode

Collection of my participation to [advent of code] puzzles with C++ language.

---

## Requirements

* CMake >= 3.22.1
* Make >= 4.3
* gcc >= 11.3.0

## Getting started

Here are the commands to build executables:

```
mkdir build
cmake -S . -B build
cd build && make
```

Run day XX
```
./<yyyy>/day<dd>_main <input_file>
```

## Add new day

1. Get cookie session from [advent of code] (need registration) & copy-paste it to `.cookie` file

2. Run `add_new_day.sh` bash script:
```
# Add current day
./add_new_day.sh

# Add custom day
./add_new_day.sh -y <yyyy> -d <dd> -s <summary>
```

If the targeted day already exists, the script does nothing.

[advent of code]: <https://adventofcode.com/> "advent of code"
