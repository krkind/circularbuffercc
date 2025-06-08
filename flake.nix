{
  description = "Nix shell for Airolit Transport Protocol";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/release-24.11";
    flake-utils.url = "github:numtide/flake-utils";

    nix-formatter-pack.url = "github:Gerschtli/nix-formatter-pack";
    nix-formatter-pack.inputs.nixpkgs.follows = "nixpkgs";

  };

  outputs =
    { self, nixpkgs, nix-formatter-pack, flake-utils }:

    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };
    in
    {
      formatter = pkgs.nixpkgs-fmt;
      devShells.default = pkgs.mkShell {
        packages = with pkgs; [
          cmake
          ninja
          gcc
          doxygen
          graphviz
        ];

        shellHook = ''
          echo "Welcome to the C++ Ninja dev shell!"
        '';
      };
    });
}
