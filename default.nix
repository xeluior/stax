let
  pkgs = import <nixpkgs> {};
in
  pkgs.stdenv.mkDerivation {
    pname = "stax";
    version = "0.1.0";
    src = ./.;
    buildInputs = with pkgs; [SDL2 SDL2_ttf pkg-config meson ninja];
  }
