{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in {
    devShells.x86_64-linux.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        arduino-cli gnumake
        (python3.withPackages (ps: with ps; [ pyserial ]))
      ];
      shellHook = ''
        echo "Installing ESP32 core for Arduino"
        arduino-cli core install esp32:esp32
      '';
    };
  };
}
