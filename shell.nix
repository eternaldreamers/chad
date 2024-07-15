{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.clang
    pkgs.cmake
  ];
 
  shellHook = ''
    export CC=clang
    export CXX=clang++
  '';
}
