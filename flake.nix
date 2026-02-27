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
        pkgs.mkShell rec {
          buildInputs = with pkgs; [
            pkgs.pkgsCross.avr.buildPackages.gcc
            pkgs.pkgsCross.avr.avrlibc
            avrdude

            # dev
            openocd
            clang-tools
            unixtools.xxd
            just
            pkg-config
          ];

          AVR_LIBC = "${pkgs.pkgsCross.avr.avrlibc}";
          LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath buildInputs}";
        };
    in
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default = shell { inherit pkgs; };
      }
    );
}
