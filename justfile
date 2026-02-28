set export
set unstable

mod test

default:
  just --choose || true

AVR_LIBC := env("AVR_LIBC", "/usr/lib/avr")
F_CPU := "32768"
MCU := "attiny84a"
MCU_PART := "attiny84"
SRCS := "src/main.c src/display.c src/rtc.c src/button.c src/mode.c"
OUTDIR := "./out"
PROGRAMMER := "arduino"
UPLOAD_SPEED := "19200"
SERIAL_PORT := "/dev/ttyUSB0"

CFLAGS := f"-g -Os -std=gnu99 -Isrc -mmcu={{MCU}} -DF_CPU={{F_CPU}}L \
-I{{AVR_LIBC}}/avr/include -L{{AVR_LIBC}}/avr/lib \
-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall"

AVRDUDE := f"avrdude -v -p {{MCU_PART}} -c {{PROGRAMMER}} -P {{SERIAL_PORT}} -b {{UPLOAD_SPEED}} -F"

build: compile-flags
  mkdir -p {{OUTDIR}}
  avr-gcc {{CFLAGS}} {{SRCS}} -o {{OUTDIR}}/out.elf
  avr-objcopy -O ihex -R .eeprom {{OUTDIR}}/out.elf {{OUTDIR}}/out.hex
  du -bh "{{OUTDIR}}/out.hex"

write: build flash

flash: check-hexfile-exists
  {{AVRDUDE}} -U "flash:w:{{OUTDIR}}/out.hex:i"

read:
  {{AVRDUDE}} -F -U flash:r:-:h

write-fuses: check-hexfile-exists
  {{AVRDUDE}} -U lfuse:w:{{OUTDIR}}/out.elf:e -U hfuse:w:{{OUTDIR}}/out.elf:e -U efuse:w:{{OUTDIR}}/out.elf:e

read-fuses:
  {{AVRDUDE}} -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

clean:
  rm -rf {{OUTDIR}}

format:
  find src/ test/ -iname '*.h' -o -iname '*.c' | xargs clang-format -i

@check-hexfile-exists:
  [ -f "{{OUTDIR}}/out.hex" ] || (echo "No out.hex. Run build first"; exit 1);

@compile-flags:
  echo '{{CFLAGS}}' | tr ' ' '\n' > ./compile_flags.txt
