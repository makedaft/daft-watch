{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    let
      shell =
        { pkgs }:
        pkgs.buildFHSEnv {
          name = "daft-watch-env";
          targetPkgs =
            pkgs: with pkgs; [
              (lib.hiPrio gcc)
              platformio-core
              avrdude
              pkgs.pkgsCross.avr.buildPackages.gcc
              openocd
              glibc
              pkgs.pkgsCross.avr.avrlibc

              # dev
              clang-tools
              unixtools.xxd
              just
              pkg-config
            ];
          multiPkgs = pkgs: (with pkgs; [ udev ]);

          runScript = "zsh";
          profile = ''
            export DIRENV_DISABLE_HOOK=1; # Dont recursively load direnv inside shell
            export WITH_NIX_PREFIX="fhs";
            export AVR_LIBC="${pkgs.pkgsCross.avr.avrlibc}";
          '';
        };
    in
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default = (shell { inherit pkgs; }).env;
      }
    );
}
