build:
  pio run -e attiny84;

check:
  pio check --skip-packages

upload:
  pio run -e attiny84 -t upload

clean:
  pio run -t clean

update:
  pio pkg update

monitor:
  pio device monitor

test:
  pio test -e test

format:
  find include/ src/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i

devsetup:
  # COMPILATIONDB_INCLUDE_TOOLCHAIN=true pio run -t compiledb
  ./setup_compiledb.sh

