{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.ccls
    pkgs.cmake
    pkgs.pkg-config
    pkgs.libssh
  ];
}
