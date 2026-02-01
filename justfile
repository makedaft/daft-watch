set export
set unstable

default:
  just --choose || true

AVR_LIBC := env("AVR_LIBC", "/usr/lib/avr")
F_CPU := "1000000"
MCU := "attiny84a"
MCU_PART := "attiny84"
SRC := "./src/main.c"
OUTDIR := "./out"
PROGRAMMER := "arduino"
UPLOAD_SPEED := "19200"
SERIAL_PORT := "/dev/ttyUSB0"

CFLAGS := f"-g -Os -std=gnu99 -Isrc -mmcu={{MCU}} -DF_CPU={{F_CPU}}L \
-I{{AVR_LIBC}}/avr/include -L{{AVR_LIBC}}/avr/lib \
-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes"

AVRDUDE := f"avrdude -v -p {{MCU_PART}} -c {{PROGRAMMER}} -P {{SERIAL_PORT}} -b {{UPLOAD_SPEED}}"

build: compile_flags
  mkdir -p {{OUTDIR}}
  avr-gcc {{CFLAGS}} {{SRC}} -o {{OUTDIR}}/out.elf
  avr-objcopy -O ihex -R .eeprom {{OUTDIR}}/out.elf {{OUTDIR}}/out.hex

write:
  [ -f "{{OUTDIR}}/out.hex" ] || (echo "No out.hex. Run build"; exit 1);
  {{AVRDUDE}} -U "flash:w:{{OUTDIR}}/out.hex:i"

read:
  {{AVRDUDE}} -F -U flash:r:-:h

clean:
  rm -rf {{OUTDIR}}

format:
  find src/ -iname '*.h' -o -iname '*.c' | xargs clang-format -i

compile_flags:
  echo '{{CFLAGS}}' | tr ' ' '\n' > ./compile_flags.txt
