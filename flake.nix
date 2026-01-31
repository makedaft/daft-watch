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
              openocd
              glibc

              # dev
              clang-tools
              unixtools.xxd
              just
            ];
          multiPkgs = pkgs: (with pkgs; [ udev ]);

          runScript = "zsh";
          profile = ''
            export DIRENV_DISABLE_HOOK=1; # Dont recursively load direnv inside shell
            export WITH_NIX_PREFIX="fhs";
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
